sudo cp -R Contents/Resources/assets/driver/BlackHole.driver /Library/Audio/Plug-Ins/HAL
cp -R "$(pwd)" /Applications/
xattr -r -d com.apple.quarantine /Applications/AudioFlow.app
sudo xattr -r -d com.apple.quarantine /Library/Audio/Plug-Ins/HAL/BlackHole.driver

