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
const preampSlider = document.getElementById('preampSlider');
const preampGainBox = document.getElementById('preampGain');
preampSlider.value = configPreAmpValue;
preampGainBox.value = configPreAmpValue;
preampSlider.oninput = function () {
    preampGainBox.value = this.value;
    configJSON['amplifier']['g'] = parseFloat(this.value);
    writeConfigToFile();
};

preampGainBox.onkeydown = function(e) {
    if (e.keyCode == 13) {
        preampGainBox.blur();
        if (isNaN(parseFloat(this.value)) || this.value < -30 || this.value > 30) {
            this.value = configGValue;
            preampSlider.value = configGValue
        } else {
            preampSlider.value = this.value;
            configJSON['amplifier']['g'] = parseFloat(this.value);
            writeConfigToFile();
        }
    }
}

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
            if (isNaN(parseFloat(this.value)) || this.value <= 0 || this.value > 16000) {
                this.value = configFValue;
            } else {
                const entry = parseFloat(this.value);
                fBox.value = entry;
                configJSON['equalizer']['f'][i] = entry;
                writeConfigToFile();
            }
        }
    }

    qBox.onkeydown = function(e) {
        if (e.keyCode == 13) {
            qBox.blur();
            if (isNaN(parseFloat(this.value)) || this.value <= 0 || this.value > 10) {
                this.value = configQValue;
            } else {
                const entry = parseFloat(this.value);
                qBox.value = entry;
                configJSON['equalizer']['q'][i] = entry;
                writeConfigToFile();
            }
        }
    }

    gainBox.onkeydown = function(e) {
        if (e.keyCode == 13) {
            gainBox.blur();
            if (isNaN(parseFloat(this.value)) || this.value < -30 || this.value > 30) {
                this.value = configGValue;
                slider.value = configGValue;
            } else {
                const entry = parseFloat(this.value);
                gainBox.value = entry;
                slider.value = entry;
                configJSON['equalizer']['g'][i] = entry;
                writeConfigToFile();
            }
        }
    }
};

// Load config values and set event listeners for reverb unit
const configDrywetValue = configJSON['reverb']['dw'] * 100;
const drywetSlider = document.getElementById('drywetSlider');
const drywetBox = document.getElementById('drywetBox');
drywetSlider.value = configDrywetValue;
drywetBox.value = configDrywetValue;
drywetSlider.oninput = function () {
    drywetBox.value = this.value;
    configJSON['reverb']['dw'] = parseFloat(this.value) / 100;
    writeConfigToFile();
};

drywetBox.onkeydown = function(e) {
    if (e.keyCode == 13) {
        drywetBox.blur();
        if (isNaN(parseFloat(this.value)) || this.value < 0 || this.value > 100) {
            this.value = configDrywetValue;
            drywetSlider.value = configDrywetValue;
        } else {
            const entry = parseFloat(this.value);
            drywetBox.value = entry;
            drywetSlider.value = entry;
            configJSON['reverb']['dw'] = entry / 100;
            writeConfigToFile();
        }
    }
}

