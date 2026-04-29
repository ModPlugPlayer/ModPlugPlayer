magick ./Installer_Icon_512x512.png \
  -background none \
  -define icon:auto-resize=256,128,64,48,32,24,16 \
  ModPlugPlayerInstaller.ico

magick ./Installer_Icon_512x512.png \
  -background none \
  -gravity center \
  -extent 560x560 \
  WizardSmallImageFile.png