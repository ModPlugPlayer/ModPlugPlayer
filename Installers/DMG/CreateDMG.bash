#!/usr/bin/env bash
set -euo pipefail

if [[ "${BASH_SOURCE[0]}" != "${0}" ]]; then
  echo "Do not source this script. Run it directly."
  return 1
fi

command -v create-dmg >/dev/null 2>&1 || {
  echo "create-dmg is required but not installed."
  echo "Install it with: brew install create-dmg"
  exit 1
}

if [[ -z "${1:-}" ]]; then
  echo "Usage: $0 <path-to-app>"
  exit 1
fi

AppPath="$1"

AppPath="$(cd "$(dirname "$AppPath")" && pwd)/$(basename "$AppPath")"

if [[ ! -d "$AppPath" ]]; then
  echo "Error: App not found at $AppPath"
  exit 1
fi

if [[ "$(echo "$AppPath" | tr '[:upper:]' '[:lower:]')" != *.app ]]; then
  echo "Error: Provided path is not a .app bundle"
  exit 1
fi

AppName="$(basename "$AppPath" .app)"

AppDir="$(dirname "$AppPath")"
AppFile="$(basename "$AppPath")"

ScriptDir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

OutputDMG="${AppDir}/${AppName} for macOS.dmg"

rm -f "$OutputDMG"

cd "$ScriptDir"

if [[ ! -f "MPP Disk Icon.icns" || ! -f "MPP Installer Background.png" ]]; then
  echo "Error: DMG assets not found in script directory"
  exit 1
fi

create-dmg \
--volname "${AppName} Installer" \
--volicon "$ScriptDir/MPP Disk Icon.icns" \
--background "$ScriptDir/MPP Installer Background.png" \
--window-pos 200 120 \
--window-size 600 420 \
--icon-size 128 \
--icon "${AppFile}" 148 185 \
--hide-extension "${AppFile}" \
--app-drop-link 455 184 \
"${OutputDMG}" \
"${AppPath}"

echo "DMG created successfully: $OutputDMG"