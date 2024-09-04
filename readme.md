# AudioFlow
AudioFlow is a MacOS system audio equalizer designed to be simple, lightweight, and robust. AudioFlow currently features a 10 band equalizer and a convolution reverb unit.

## Table of Contents
- [Installation Instructions](#installation-instructions)
- [Uninstallation Instructions](#uninstallation-instructions)
- [Features](#features)
- [Demo](#demo)
- [Contributing](#contributing)
- [Credits](#credits)

## Installation Instructions
1. Download the most recent release from 
2. Navigate to:\
```cd {PATH_TO_AUDIOFLOW}/Contents/Resources```
3. Execute the installer:\
```sh scripts/installer.sh```\
Note: This script may prompt password input as it moves the virtual audio device driver from ```assets/driver``` to ```/Library/Audio/Plug-Ins/HAL``` which requires administrator privileges. Alternatively, it is possible to manually move it and subsequently execute ```sudo killall -9 coreaudiod``` to restart CoreAudio. 

## Uninstallation Instructions
1. Navigate to:\
   ```cd /Applications/AudioFlow/Contents/Resources```
2. Execute the uninstaller:
   ```sh scripts/uninstaller.sh```\

## Features
* Equalizer
  * 10 Bands
  * Simple presets
  * Auto preamplification
  * Adjustable gain, frequency, and quality factor (Q)

* Reverb unit
  * Convolution based
  * 135 impulse responses representing different audiospatial environments to choose from
  * Adjustable mixing (dry/wet)

## Demo

## Contributing
All contributions are welcome. Whether you're fixing a bug, adding a new feature, or have an issue, feel free to open a pull request/issue/etc.

## Credits
This project uses the BlackHole Audio Loopback Driver by @ExistentialAudio to capture system audio. https://github.com/ExistentialAudio/BlackHole
