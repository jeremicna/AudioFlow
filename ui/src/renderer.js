// Config
let configJSON = window.electronAPI.readConfig();

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
    window.electronAPI.writeConfig(configJSON);
};

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

    selectReverbPreset.value = configJSON['reverb']['ir'].split('/').pop().split('.')[0]
}

const loadPresets = function () {
    equalizerPresets = window.electronAPI.getEqualizerPresets();
    reverbPresets = window.electronAPI.getReverbPresets();
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
        configJSON['equalizer']['f'] = [...equalizerPresets[selectEqualizerPreset.value]['f']];
        configJSON['equalizer']['q'] = [...equalizerPresets[selectEqualizerPreset.value]['q']];
        configJSON['equalizer']['g'] = [...equalizerPresets[selectEqualizerPreset.value]['g']];
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

        selectEqualizerPreset.value = 'custom';

        writeConfigToFile();
        renderConfig();
    }

    fBox.onkeydown = function(e) {
        if (e.keyCode == 13) {
            fBox.blur();
            if (!(isNaN(parseFloat(this.value)) || this.value <= 0 || this.value > 16000)) {
                configJSON['equalizer']['f'][i] = parseFloat(this.value);

                selectEqualizerPreset.value = 'custom';

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

                selectEqualizerPreset.value = 'custom';

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

                selectEqualizerPreset.value = 'custom';

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

const init = () => {
    loadPresets();
    renderConfig();
};

init();