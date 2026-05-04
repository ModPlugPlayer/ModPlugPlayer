#!/usr/bin/env bash
set -euo pipefail
command -v jq >/dev/null 2>&1 || { echo "jq is required"; exit 1; }

Credentials=(
  --apple-id "${AppleID}"
  --team-id "${TeamID}"
  --password "${AppSpecificPassword}"
)
# or optionally
# Credentials=(
#   --keychain-profile "${ProfileName}"
# )

# You can store the credentials to keychain profile once instead of using --apple-id, --team-id and --password, you can use --keychain-profile instead.
# One time (stores credentials in the OS keychain permanently)
# Personally I omit this.
# xcrun notarytool store-credentials ${ProfileName} \
#   --apple-id "${AppleID}" \
#   --team-id "${TeamID}" \
#   --password "${AppSpecificPassword}"

##Go to the .app zip Folder
cd "${AppZipFolderPath}"

#1. Unzip the .app to be signed
ditto -x -k "${AppName}.zip" .

#2. Sign the .app
codesign --deep --force --verify --verbose \
  --sign "Developer ID Application: ${DeveloperName} (${TeamID})" \
  --options runtime --timestamp \
  "${AppName}.app"

#Verify the sign
codesign --verify --deep --strict --verbose=2 "${AppName}.app"
#Output should be:
#--prepared: ..../..../...dylib
#--validated: ..../..../...dylib
#....
#${AppName}.app: valid on disk
#${AppName}.app: satisfies its Designated Requirement

#3. Zip the signed .app (signed zip)
rm -f "${AppName}Signed.zip"
ditto -c -k --keepParent "${AppName}.app" "${AppName}Signed.zip"

#4. Notarize the signed zip (--wait is optional)
JsonOutput=$(xcrun notarytool submit "${AppName}Signed.zip" "${Credentials[@]}" --wait --output-format json || {
  echo "Notarization submission of the .zip file failed"
  exit 1
})

AppNotarizationId=$(echo "$JsonOutput" | jq -r '.id // empty')
AppNotarizationStatus=$(echo "$JsonOutput" | jq -r '.status // empty')

echo "App Notarization ID: $AppNotarizationId"
echo "App Notarization Status: $AppNotarizationStatus"

if [[ "${AppNotarizationStatus}" != "Accepted" ]]; then
  echo "App notarization failed"
  exit 1
fi

#Notarization verification (if --wait parameter is not used)
#xcrun notarytool info ${NotarizationId} ${Credentials[@]}
#Output should be
#Successfully received submission info
#  createdDate: 2026-05-04T05:33:26.398Z
#  id: ${NotarizationId}
#  name: ${AppName}Signed.zip
#  status: Accepted

#5. Staple the signed .app
xcrun stapler staple "${AppName}.app"

#Verify
ditto "${AppName}.app" "${AppName} Verify.app"
spctl --assess --type execute --verbose "${AppName} Verify.app"
rm -rf "${AppName} Verify.app"
# Output should be
#
# ${AppName} Verify.app: accepted
# source=Notarized Developer ID

#6. Create DMG containing stapled .app
"${ProjectPath}/Installers/DMG/CreateDMG.bash" "${AppName}.app"

#7. Sign the DMG
codesign --sign "Developer ID Application: ${DeveloperName} (${TeamID})" \
  --timestamp \
  "${AppName} for macOS.dmg"

#Verify the sign
codesign --verify --verbose=2 "${AppName} for macOS.dmg"
# Output:
# ${AppName} for macOS.dmg: valid on disk
# ${AppName} for macOS.dmg: satisfies its Designated Requirement

#8. Notarize the DMG
JsonOutputDmg=$(xcrun notarytool submit "${AppName} for macOS.dmg" "${Credentials[@]}" --wait --output-format json || {
  echo "Notarization submission of DMG failed"
  exit 1
})

DmgNotarizationId=$(echo "$JsonOutputDmg" | jq -r '.id // empty')
DmgNotarizationStatus=$(echo "$JsonOutputDmg" | jq -r '.status // empty')

if [[ "$DmgNotarizationStatus" != "Accepted" ]]; then
  echo "DMG notarization failed"
  exit 1
fi

echo "DMG Notarization ID: $DmgNotarizationId"
echo "DMG Notarization Status: $DmgNotarizationStatus"

#9. Staple the DMG
xcrun stapler staple "${AppName} for macOS.dmg"
# Output:
# Processing: ${AppZipFolderPath}/${AppName} for macOS.dmg
# Processing: ${AppZipFolderPath}/${AppName} for macOS.dmg
# The staple and validate action worked!

#Verify
ditto "${AppName} for macOS.dmg" "${AppName} for macOS Verify.dmg"
spctl --assess --type open --verbose "${AppName} for macOS Verify.dmg"
rm -rf "${AppName} for macOS Verify.dmg"

#cleanup
trap 'rm -f "${AppName}Signed.zip"' EXIT
