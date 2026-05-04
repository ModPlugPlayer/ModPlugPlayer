#!/usr/bin/env bash

if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
  echo "Please source this script: source SetSigningVariables.sh"
  exit 1
fi

if [[ -z "${AppSpecificPassword}" ]]; then
  while [[ -z "${AppSpecificPassword}" ]]; do
    read -s -p "Enter Apple Code Signing password (App-Specific Password): " AppSpecificPassword
    echo ""
  done
fi

# AppSpecificPassword is provided at runtime (DO NOT HARDCODE)
export AppSpecificPassword

export DeveloperName="Name Surname"
export AppleID="Developer Mail Address"
export TeamID="Team ID"
export ProfileName="Profile Name"
export DMGName="Can be the same as AppName"
export AppName="App Name"
export AppZipFolderPath="Path to your zip folder"
export ProjectPath="Project path"
echo "Code signing environment loaded."