; ================================
; ModPlug Player Inno Setup
; Single setup: x64 + ARM64
; Automatic architecture selection
; ================================

#define AppName        "ModPlug Player"
#define AppPublisher   "Volkan Orhan"
#define AppURL         "https://www.modplugplayer.org"
#define AppVersion     "2.0.0 Alpha 5"
#define AppIdGuid      "{{86571A47-C047-49C6-9D70-D2D606E48C0F}}"

[Setup]
AppId={#AppIdGuid}
AppName={#AppName}
AppVersion={#AppVersion}
AppPublisher={#AppPublisher}
AppPublisherURL={#AppURL}
AppSupportURL={#AppURL}
AppUpdatesURL={#AppURL}
DefaultDirName={pf}\{#AppName}
DefaultGroupName={#AppName}
OutputDir=output
OutputBaseFilename={#AppName}-Setup-{#AppVersion}
ArchitecturesInstallIn64BitMode=x64 arm64
PrivilegesRequired=admin
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
DisableDirPage=no
DisableProgramGroupPage=no
UninstallDisplayIcon={app}\{#AppName}.exe
SetupIconFile=..\..\Components\Common\ui\Icons\MPP.ico
LicenseFile=..\..\LICENSE.txt
; Signing
; SignedUninstaller=true
; SignTool=signtool sign /fd sha256 /tr http://time.certum.pl /td sha256 /a /v $f

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; --- x64 payload
Source: "payload\x64\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Check: Is64BitIntel

; --- ARM64 payload
Source: "payload\arm64\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Check: IsARM64

; Common files that are the same on all the architectures
; Source: "payload\common\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\{#AppName}"; Filename: "{app}\{#AppName}.exe"
Name: "{commondesktop}\{#AppName}"; Filename: "{app}\{#AppName}.exe"; Tasks: desktopicon

[Run]
; Run after then the installation
Filename: "{app}\{#AppName}.exe"; Description: "{cm:LaunchProgram,{#AppName}}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
; To clean the user data (log/cache vs):
; Name: "{userappdata}\{#AppName}"; Type: filesandordirs

[Code]
function Is64BitIntel: Boolean;
begin
  Result := (ProcessorArchitecture = paX64);
end;

function IsARM64: Boolean;
begin
  Result := (ProcessorArchitecture = paARM64);
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssInstall then
  begin
    if IsARM64 then
      Log('Detected ARM64; installing ARM64 payload')
    else if Is64BitIntel then
      Log('Detected x64; installing x64 payload')
    else
      Log('Non-supported architecture for this package.');
  end;
end;
