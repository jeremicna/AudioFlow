const path = require('path');
var fs = require('fs');

// Retrieve config
const rootPath = path.resolve(path.dirname(__dirname), '..');
const configPath = rootPath + '/config.json';
let configJSON = JSON.parse(fs.readFileSync(configPath));

const writeConfigToFile = function() {
    const tempFilePath = path.join(rootPath, 'config.tmp');
    fs.writeFileSync(tempFilePath, JSON.stringify(configJSON, null, 2));
    fs.renameSync(tempFilePath, configPath);
}

// Load config values and set event listeners for preamp
const configPreAmpValue = configJSON['amplifier']['g'];
const preAmpSlider = document.getElementById('preampSlider');
preAmpSlider.value = configPreAmpValue;
preAmpSlider.oninput = function () {
    configJSON['amplifier']['g'] = parseFloat(this.value);
    writeConfigToFile();
};



// Load config values and set event listeners for all equalizer bands
const sliderContainers = document.getElementsByClassName('eqSliderContainer');
for (let i = 0; i < sliderContainers.length; i++) {
    const configFValue = configJSON['equalizer']['f'][i];
    const configQValue = configJSON['equalizer']['q'][i];
    const configGValue = configJSON['equalizer']['g'][i];
    const slider = document.getElementById('eqSlider' + i);
    const fBox = document.getElementById('fBox' + i);
    const qBox = document.getElementById('qBox' + i);
    const gainBox = document.getElementById('gainBox' + i);
    fBox.value = configFValue;
    qBox.value = configQValue;
    gainBox.value = configGValue;
    slider.value = configGValue;

    slider.oninput = function() {
        gainBox.value = this.value;
        configJSON['equalizer']['g'][i] = parseFloat(this.value);
        writeConfigToFile();
    }

    fBox.onkeydown = function(e) {
        if (e.keyCode == 13) {
            fBox.blur();
            if (isNaN(parseFloat(this.value)) || this.value < 0 || this.value > 16000) {
                this.value = configFValue;
            } else {
                configJSON['equalizer']['f'][i] = parseFloat(this.value);
                writeConfigToFile();
            }
        }
    }

    qBox.onkeydown = function(e) {
        if (e.keyCode == 13) {
            qBox.blur();
            if (isNaN(parseFloat(this.value)) || this.value < 0 || this.value > 10) {
                this.value = configQValue;
            } else {
                configJSON['equalizer']['q'][i] = parseFloat(this.value);
                writeConfigToFile();
            }
        }
    }

    gainBox.onkeydown = function(e) {
        if (e.keyCode == 13) {
            gainBox.blur();
            if (isNaN(parseFloat(this.value)) || this.value < -30 || this.value > 30) {
                this.value = configGValue;
                slider.value = configGValue
            } else {
                slider.value = this.value;
                configJSON['equalizer']['g'][i] = parseFloat(this.value);
                writeConfigToFile();
            }
        }
    }
};

// IMPLEMENT: FIX SLIDER SPACING
// IMPLEMENT: DB BOX FOR PREAMP
// IMPLEMENT: REVERB UI AND OTHER STUFF
// IMPLEMENT: TOGGLES

