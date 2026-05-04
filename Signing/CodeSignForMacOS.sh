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
cd ${AppZipPath}

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
JsonOutput=$(xcrun notarytool submit "${AppName}Signed.zip" ${Credentials[@]} --wait --output-format json)

NotarizationId=$(echo "$JsonOutput" | jq -r '.id')
NotarizationStatus=$(echo "$JsonOutput" | jq -r '.status')

echo "ID: $NotarizationId"
echo "Status: $NotarizationStatus"

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

#7. Sign the DMG
codesign --sign "Developer ID Application: ${DeveloperName} (${TeamID})" \
  --timestamp \
  "${AppName}.dmg"

#8. Notarize the DMG
JsonOutputDmg=$(xcrun notarytool submit "${AppName}.dmg" ${Credentials[@]} --wait --output-format json)

NotarizationIdDmg=$(echo "$JsonOutputDmg" | jq -r '.id')
NotarizationStatusDmg=$(echo "$JsonOutputDmg" | jq -r '.status')

echo "ID: $NotarizationIdDmg"
echo "Status: $NotarizationStatusDmg"

#9. Staple the DMG
xcrun stapler staple "${AppName}.dmg"
