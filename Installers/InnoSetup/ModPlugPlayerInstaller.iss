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
DefaultDirName={code:GetInstallDir}
DefaultGroupName={#AppName}
OutputDir=output
OutputBaseFilename=ModPlugPlayer-Setup-{#AppVersion}
ArchitecturesInstallIn64BitMode=x64compatible arm64
PrivilegesRequired=admin
PrivilegesRequiredOverridesAllowed=dialog
UsePreviousPrivileges=no
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
DisableDirPage=no
DisableProgramGroupPage=no
DisableReadyPage=no
UninstallDisplayIcon={app}\{#AppName}.exe
SetupIconFile=Icons\ModPlugPlayerInstaller.ico
WizardSmallImageFile=Icons\Installer_Icon_512x512.png
LicenseFile=..\..\LICENSE
SignedUninstaller=true
SignTool=signtool

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";

[Files]
; --- x64 payload
Source: "payload\x64\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Check: IsX64OS

; --- ARM64 payload
Source: "payload\arm64\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Check: IsARM64

[Icons]
Name: "{group}\{#AppName}"; Filename: "{app}\{#AppName}.exe"
Name: "{autodesktop}\{#AppName}"; Filename: "{app}\{#AppName}.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\{#AppName}.exe"; Description: "{cm:LaunchProgram,{#AppName}}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
; Name: "{userappdata}\{#AppName}"; Type: filesandordirs

[Code]
function InitializeSetup(): Boolean;
begin
  if not (IsX64OS or IsARM64) then
  begin
    MsgBox('This installer supports only x64 and ARM64 systems.', mbError, MB_OK);
    Result := False;
  end
  else
    Result := True;
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssInstall then
  begin
    if IsARM64 then
      Log('Architecture: ARM64')
    else if IsX64OS then
      Log('Architecture: x64');
  end;
end;

function GetInstallDir(Param: string): string;
begin
  if IsAdminInstallMode then
    Result := ExpandConstant('{autopf}\{#AppName}')
  else
    Result := ExpandConstant('{localappdata}\Programs\{#AppName}');
end;
