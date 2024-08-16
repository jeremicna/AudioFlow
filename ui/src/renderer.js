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

// Load config values and set event listeners for toggles
const autoPreampToggle = document.getElementById('autoPreampToggle');
const configEqualizerToggle = configJSON['equalizer']['toggle'];
const configReverbToggle = configJSON['reverb']['toggle'];
const equalizerToggle = document.getElementById('equalizerToggle');
const reverbToggle = document.getElementById('reverbToggle');
equalizerToggle.checked = configEqualizerToggle;
reverbToggle.checked = configReverbToggle;
autoPreampToggle.oninput = function () {
    if (autoPreampToggle.checked) {
        const preamp = -Math.max(0, ...configJSON['equalizer']['g']);
        preampSlider.value = preamp;
        preampGainBox.value = preamp;
        configJSON['amplifier']['g'] = preamp;
        writeConfigToFile();
    }
}

equalizerToggle.oninput = function () {
    equalizerToggle.checked = this.checked;
    configJSON['amplifier']['toggle'] = this.checked;
    configJSON['equalizer']['toggle'] = this.checked;
    writeConfigToFile();
}

reverbToggle.oninput = function () {
    equalizerToggle.value = this.checked;
    configJSON['reverb']['toggle'] = this.checked;
    writeConfigToFile();
}


// Load config values and set event listeners for preamp
const configPreampValue = configJSON['amplifier']['g'];
const preampSlider = document.getElementById('preampSlider');
const preampGainBox = document.getElementById('preampGain');
preampSlider.value = configPreampValue;
preampGainBox.value = configPreampValue;
preampSlider.oninput = function () {
    autoPreampToggle.checked = false;
    preampGainBox.value = this.value;
    configJSON['amplifier']['g'] = parseFloat(this.value);
    writeConfigToFile();
};

preampGainBox.onkeydown = function(e) {
    if (e.keyCode == 13) {
        preampGainBox.blur();
        if (isNaN(parseFloat(this.value)) || this.value < -30 || this.value > 30) {
            this.value = configJSON['amplifier']['g'];
            preampSlider.value = configJSON['amplifier']['g']
        } else {
            const preamp = -Math.max(0, ...configJSON['equalizer']['g']);
            preampSlider.value = preamp;
            preampGainBox.value = preamp;
            configJSON['amplifier']['g'] = preamp;
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

        if (autoPreampToggle.checked) {
            const preamp = -Math.max(0, ...configJSON['equalizer']['g']);
            preampSlider.value = preamp;
            preampGainBox.value = preamp;
            configJSON['amplifier']['g'] = preamp;
        }

        writeConfigToFile();
    }

    fBox.onkeydown = function(e) {
        if (e.keyCode == 13) {
            fBox.blur();
            if (isNaN(parseFloat(this.value)) || this.value <= 0 || this.value > 16000) {
                this.value = configJSON['equalizer']['f'][i];
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
                this.value = configJSON['equalizer']['q'][i];
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
                this.value = configJSON['equalizer']['g'][i];
                slider.value = configJSON['equalizer']['g'][i];
            } else {
                const entry = parseFloat(this.value);
                gainBox.value = entry;
                slider.value = entry;
                configJSON['equalizer']['g'][i] = entry;

                if (autoPreampToggle.checked) {
                    const preamp = -Math.max(0, ...configJSON['equalizer']['g']);
                    preampSlider.value = preamp;
                    preampGainBox.value = preamp;
                    configJSON['amplifier']['g'] = preamp;
                }

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

