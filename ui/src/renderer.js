const path = require('path');
var fs = require('fs');

// Config
const rootPath = path.resolve(path.dirname(__dirname), '..');
const configPath = rootPath + '/config.json';
const assetsPath = rootPath + '/assets';
const equalizerPresetsPath = assetsPath + '/eq';
const reverbPresetsPath = assetsPath + '/ir';
let configJSON = JSON.parse(fs.readFileSync(configPath));

// DOM Elements
const autoPreampToggle = document.getElementById('autoPreampToggle');
const equalizerToggle = document.getElementById('equalizerToggle');
const reverbToggle = document.getElementById('reverbToggle');

const preampSlider = document.getElementById('preampSlider');
const preampGainBox = document.getElementById('preampGain');

const selectEqualizerPreset = document.getElementById('selectEqualizerPreset')
const sliderContainers = document.getElementsByClassName('eqSliderContainer');
let eqSliders = [];
let eqFBoxes = [];
let eqQBoxes = [];
let eqGainBoxes = [];

const selectReverbPreset = document.getElementById('selectReverbPreset')
const drywetSlider = document.getElementById('drywetSlider');
const drywetBox = document.getElementById('drywetBox');

// Presets
let equalizerPresets = {};
let reverbPresets = {};

const writeConfigToFile = function() {
    const tempFilePath = path.join(rootPath, 'config.tmp');
    fs.writeFileSync(tempFilePath, JSON.stringify(configJSON, null, 2));
    fs.renameSync(tempFilePath, configPath);
}

const renderConfig = function () {
    equalizerToggle.checked = configJSON['equalizer']['toggle'];
    reverbToggle.checked = configJSON['reverb']['toggle'];
    preampSlider.value = configJSON['amplifier']['g']
    preampGainBox.value = configJSON['amplifier']['g'];

    for (let i = 0; i < sliderContainers.length; i++) {
        eqSliders[i].value = configJSON['equalizer']['g'][i];
        eqFBoxes[i].value = configJSON['equalizer']['f'][i];
        eqQBoxes[i].value = configJSON['equalizer']['q'][i];
        eqGainBoxes[i].value = configJSON['equalizer']['g'][i];
    }

    drywetSlider.value = Math.round(configJSON['reverb']['dw'] * 100);
    drywetBox.value = Math.round(configJSON['reverb']['dw'] * 100);
}

const getEqualizerPresets = function () {
    const files = fs.readdirSync(equalizerPresetsPath);
    const filesDict = {};

    for (const file of files) {
        const presetJSON = JSON.parse(fs.readFileSync(path.join(equalizerPresetsPath, file)));
        const name = file.split('.')[0];
        filesDict[name] = presetJSON;
    }

    return filesDict;
}

const getReverbPresets = function () {
    const files = fs.readdirSync(reverbPresetsPath);
    const filesDict = {};

    for (const file of files) {
        const name = file.split('.')[0];
        filesDict[name] = '../assets/ir/' + file;
    }

    return filesDict;
}

const loadPresets = function () {
    equalizerPresets = getEqualizerPresets();
    reverbPresets = getReverbPresets();
    for (const key in equalizerPresets) {
        let node = document.createElement('option');
        node.value = key;
        node.text = key.charAt(0).toUpperCase() + key.slice(1);
        selectEqualizerPreset.appendChild(node);
    }
    for (const key in reverbPresets) {
        let node = document.createElement('option');
        node.value = key;
        node.text = key.charAt(0).toUpperCase() + key.slice(1);
        selectReverbPreset.appendChild(node);
    }
}

// Set event listeners for presets
selectEqualizerPreset.addEventListener('change', function () {
    if (selectEqualizerPreset.value != 'custom') {
        configJSON['equalizer']['f'] = equalizerPresets[selectEqualizerPreset.value]['f'];
        configJSON['equalizer']['q'] = equalizerPresets[selectEqualizerPreset.value]['q'];
        configJSON['equalizer']['g'] = equalizerPresets[selectEqualizerPreset.value]['g'];
        writeConfigToFile();
    }
    renderConfig();
})
selectReverbPreset.addEventListener('change', function () {
    configJSON['reverb']['ir'] = reverbPresets[selectReverbPreset.value];
    writeConfigToFile();
    renderConfig();
})

// Set event listeners for toggles
autoPreampToggle.oninput = function () {
    if (autoPreampToggle.checked) {
        const preamp = -Math.max(0, ...configJSON['equalizer']['g']);
        configJSON['amplifier']['g'] = preamp;
        writeConfigToFile();
    }
    renderConfig();
}

equalizerToggle.oninput = function () {
    configJSON['amplifier']['toggle'] = this.checked;
    configJSON['equalizer']['toggle'] = this.checked;
    writeConfigToFile();
    renderConfig();
}

reverbToggle.oninput = function () {
    configJSON['reverb']['toggle'] = this.checked;
    writeConfigToFile();
    renderConfig();
}

// Load config values and set event listeners for preamp
preampSlider.oninput = function () {
    autoPreampToggle.checked = false;
    configJSON['amplifier']['g'] = parseFloat(this.value);
    writeConfigToFile();
    renderConfig();
};

preampGainBox.onkeydown = function(e) {
    if (e.keyCode == 13) {
        preampGainBox.blur();
        if (!(isNaN(parseFloat(this.value)) || this.value < -30 || this.value > 30)) {
            autoPreampToggle.checked = false;
            configJSON['amplifier']['g'] = parseFloat(this.value);
            writeConfigToFile();
        }
        renderConfig();
    }
}

// Set event listeners for all equalizer bands
for (let i = 0; i < sliderContainers.length; i++) {
    const slider = document.getElementById('eqSlider' + i);
    const fBox = document.getElementById('fBox' + i);
    const qBox = document.getElementById('qBox' + i);
    const gainBox = document.getElementById('gainBox' + i);
    eqSliders.push(slider);
    eqFBoxes.push(fBox);
    eqQBoxes.push(qBox);
    eqGainBoxes.push(gainBox);

    slider.oninput = function() {
        configJSON['equalizer']['g'][i] = parseFloat(this.value);

        if (autoPreampToggle.checked) {
            const preamp = -Math.max(0, ...configJSON['equalizer']['g']);
            configJSON['amplifier']['g'] = preamp;
        }

        writeConfigToFile();
        renderConfig();
    }

    fBox.onkeydown = function(e) {
        if (e.keyCode == 13) {
            fBox.blur();
            if (!(isNaN(parseFloat(this.value)) || this.value <= 0 || this.value > 16000)) {
                configJSON['equalizer']['f'][i] = parseFloat(this.value);

                writeConfigToFile();
            }
            renderConfig();
        }
    }

    qBox.onkeydown = function(e) {
        if (e.keyCode == 13) {
            qBox.blur();
            if (!(isNaN(parseFloat(this.value)) || this.value <= 0 || this.value > 10)) {
                configJSON['equalizer']['q'][i] = parseFloat(this.value);

                writeConfigToFile();
            }
            renderConfig();
        }
    }

    gainBox.onkeydown = function(e) {
        if (e.keyCode == 13) {
            gainBox.blur();
            if (!(isNaN(parseFloat(this.value)) || this.value < -30 || this.value > 30)) {
                configJSON['equalizer']['g'][i] = parseFloat(this.value);

                if (autoPreampToggle.checked) {
                    const preamp = -Math.max(0, ...configJSON['equalizer']['g']);
                    configJSON['amplifier']['g'] = preamp;
                }

                writeConfigToFile();
            }
            renderConfig()
        }
    }
};

// Set event listeners for reverb unit
drywetSlider.oninput = function () {
    configJSON['reverb']['dw'] = parseFloat(this.value) / 100;
    writeConfigToFile();
    renderConfig();
};

drywetBox.onkeydown = function(e) {
    if (e.keyCode == 13) {
        drywetBox.blur();
        if (!(isNaN(parseFloat(this.value)) || this.value < 0 || this.value > 100)) {
            configJSON['reverb']['dw'] = parseFloat(this.value) / 100;
            writeConfigToFile();
        }
        renderConfig();
    }
}

// Run
loadPresets();
renderConfig();
