#!/usr/bin/env bash
# WARNING:
# This script is for reference only and not intended to be executed as-is.
# Do NOT use as a production pipeline.
DeveloperName="Developer Name Surname"
AppleID="developer-mail-address"
TeamID="..."
ProfileName="..."
AppSpecificPassword="app-specific-password"
DMGName="ModPlug Player"
AppName="ModPlug Player"
DMGPath="..."
#Choose ONE:
Credentials=(
  --apple-id "${AppleID}"
  --team-id "${TeamID}"
  --password "${AppSpecificPassword}"
)
#or
Credentials=(
  --keychain-profile "${ProfileName}"
)

#You can store the credentials to keychain profile once instead of using --apple-id, --team-id and --password, you can use --keychain-profile instead.
#One time (stores credentials in the OS keychain permanently)
xcrun notarytool store-credentials ${ProfileName} \
  --apple-id "${AppleID}" \
  --team-id "${TeamID}" \
  --password "${AppSpecificPassword}"

#1. Unzip the .app to be signed
ditto -x -k ${AppName}.zip .
#2. Sign the .app
#3. Zip the signed .app (signed zip)
ditto -c -k --keepParent ${AppName}.app ${AppName}Signed.zip
#4. Notarize the signed zip
#5. Staple the signed .app
#6. Create DMG containing stapled .app
#7. Sign the DMG
#8. Notarize the DMG
#9. Staple the DMG

#############################################################################################
# NOTE:
# The entire DMG mount + sign + verify section below is incorrect and should be ignored.
# Correct flow:
# Sign .app → Notarize → Staple → THEN create DMG

#Go to the DMG Folder
cd ${DMGPath}

#Mount DMG
hdiutil attach ${DMGName}.dmg

#Sign the app inside DMG
codesign --deep --force --verify --verbose \
  --sign "Developer ID Application: ${DeveloperName} (${TeamID})" \
  --options runtime --timestamp \
  /Volumes/${DMGName}/${AppName}.app

#Verify the sign
codesign --verify --deep --strict --verbose=2 "/Volumes/${DMGName}/${AppName}.app"

#Unmount DMG
hdiutil detach "/Volumes/${DMGName}"
#############################################################################################

#Notarization (--wait is optional)
xcrun notarytool submit "${DMGName}.dmg" ${Credentials[@]} --wait

#Result -- take the notarization id from the result of notarization
#For example:
#Conducting pre-submission checks for AcademyScope.zip and initiating connection to the Apple notary service...
#Submission ID received
#  id: 1dbd6049-2ab7-4fc0-be92-30bd7ea9238a
#Upload progress: %100,00 (41,8 MB of 41,8 MB)
#Successfully uploaded file
#  id: 1dbd6049-2ab7-4fc0-be92-30bd7ea9238a
#  path: /Volumes/Projects/Build/Qt_6_9_2_for_macOS/YKSEpiSelecta/MinSizeRel/AcademyScope.zip
NotarizationID="..."

#Mount DMG for stapling
hdiutil attach "${DMGName}.dmg"

#Staple
xcrun stapler staple "/Volumes/${DMGName}/${AppName}.app"

#Verify
spctl --assess --type execute --verbose "/Volumes/${DMGName}/${AppName}.app"

#Unmount DMG
hdiutil detach "/Volumes/${DMGName}"

#Sign DMG
codesign --sign "Developer ID Application: ${DeveloperName} (${TeamID})" \
  --timestamp \
  "${DMGName}.dmg"

#If --wait is not used on notarization, check the result of notarization
xcrun notarytool info ${NotarizationID} ${Credentials[@]}

#Staple the DMG
xcrun stapler staple "${DMGName}.dmg"
