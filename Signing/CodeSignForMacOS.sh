#!/usr/bin/env bash
set -euo pipefail
command -v jq >/dev/null 2>&1 || { echo "jq is required"; exit 1; }
command -v xcrun >/dev/null || { echo "xcrun not found"; exit 1; }
command -v codesign >/dev/null || { echo "codesign not found"; exit 1; }
command -v spctl >/dev/null || { echo "spctl not found" >&2; exit 1; }

: "${AppleID:?AppleID is not set}"
: "${TeamID:?TeamID is not set}"
: "${AppSpecificPassword:?AppSpecificPassword is not set}"
: "${DeveloperName:?DeveloperName is not set}"
: "${AppZipFolderPath:?AppZipFolderPath is not set}"
: "${AppName:?AppName is not set}"
: "${ProjectPath:?ProjectPath is not set}"

security find-identity -v -p codesigning | grep -E "Developer ID Application: .* \(${TeamID}\)" >/dev/null \
  || { echo "Valid Developer ID Application identity not found for TeamID ${TeamID}" >&2; exit 1; }
  
cleanup() {
  echo "Cleaning up temporary files..."
  if [[ -n "${AppName:-}" ]]; then
    rm -f "${AppName}Signed.zip"
    rm -rf "${AppName} Verify.app"
    rm -rf "${AppName} for macOS Verify.dmg"
  fi
}
trap cleanup EXIT

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

echo "Going to the .app zip Folder"
cd "${AppZipFolderPath}" || { echo "Cannot cd to ${AppZipFolderPath}" >&2; exit 1; }

if [[ ! -f "${AppName}.zip" ]]; then
  echo "Error: ${AppName}.zip not found" >&2
  exit 1
fi

echo "Unzipping the .app to be signed"
ditto -x -k "${AppName}.zip" .

echo "Signing the .app"
codesign --deep --force --verify --verbose \
  --sign "Developer ID Application: ${DeveloperName} (${TeamID})" \
  --options runtime --timestamp \
  "${AppName}.app"

echo "Verifying the sign"
codesign --verify --deep --strict --verbose=2 "${AppName}.app"
#Output should be:
#--prepared: ..../..../...dylib
#--validated: ..../..../...dylib
#....
#${AppName}.app: valid on disk
#${AppName}.app: satisfies its Designated Requirement

echo "Zipping the signed .app (signed zip)"
rm -f "${AppName}Signed.zip"
ditto -c -k --keepParent "${AppName}.app" "${AppName}Signed.zip"

echo "Notarizing the signed zip"
JsonOutput=$(xcrun notarytool submit "${AppName}Signed.zip" "${Credentials[@]}" --wait --output-format json || {
  echo "Notarization submission of the .zip file failed"
  exit 1
})

AppNotarizationId=$(echo "$JsonOutput" | jq -r '.id // empty')
AppNotarizationStatus=$(echo "$JsonOutput" | jq -r '.status // empty')

echo "App Notarization ID: $AppNotarizationId"
echo "App Notarization Status: $AppNotarizationStatus"

if [[ -z "$AppNotarizationStatus" ]]; then
  echo "Failed to parse app notarization response"
  exit 1
fi

if [[ "${AppNotarizationStatus}" != "Accepted" ]]; then
  xcrun notarytool log "$AppNotarizationId" "${Credentials[@]}" || true
  echo "App notarization failed" >&2
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

echo "Stapling the signed .app"
xcrun stapler staple "${AppName}.app"

echo "Verifying the stapled app"
ditto "${AppName}.app" "${AppName} Verify.app"
spctl --assess --type execute --verbose=2 "${AppName} Verify.app"
# Output should be
#
# ${AppName} Verify.app: accepted
# source=Notarized Developer ID

echo "Creating the DMG containing the stapled .app"
"${ProjectPath}/Installers/DMG/CreateDMG.bash" "${AppName}.app"

echo "Signing the DMG"
codesign --sign "Developer ID Application: ${DeveloperName} (${TeamID})" \
  --timestamp \
  "${AppName} for macOS.dmg"

echo "Verifying the signed DMG"
codesign --verify --verbose=2 "${AppName} for macOS.dmg"
# Output:
# ${AppName} for macOS.dmg: valid on disk
# ${AppName} for macOS.dmg: satisfies its Designated Requirement

echo "Notarizing the signed DMG"
JsonOutputDmg=$(xcrun notarytool submit "${AppName} for macOS.dmg" "${Credentials[@]}" --wait --output-format json || {
  echo "Notarization submission of DMG failed" >&2
  exit 1
})

DmgNotarizationId=$(echo "$JsonOutputDmg" | jq -r '.id // empty')
DmgNotarizationStatus=$(echo "$JsonOutputDmg" | jq -r '.status // empty')

if [[ -z "$DmgNotarizationStatus" ]]; then
  echo "Failed to parse DMG notarization response" >&2
  exit 1
fi

if [[ "${DmgNotarizationStatus}" != "Accepted" ]]; then
  xcrun notarytool log "$DmgNotarizationId" "${Credentials[@]}" || true
  echo "DMG notarization failed" >&2
  exit 1
fi

echo "DMG Notarization ID: $DmgNotarizationId"
echo "DMG Notarization Status: $DmgNotarizationStatus"

echo "Stapling the signed DMG"
xcrun stapler staple "${AppName} for macOS.dmg"
# Output:
# Processing: ${AppZipFolderPath}/${AppName} for macOS.dmg
# Processing: ${AppZipFolderPath}/${AppName} for macOS.dmg
# The staple and validate action worked!

echo "Verifying the stapled DMG"
ditto "${AppName} for macOS.dmg" "${AppName} for macOS Verify.dmg"
spctl --assess --type open --verbose=2 "${AppName} for macOS Verify.dmg"
echo "DMG verification passed"

echo "Release pipeline completed successfully"
