const { contextBridge } = require('electron');
const path = require('path');
const fs = require('fs');

// Paths
const rootPath = path.resolve(path.dirname(__dirname), '..');
const configPath = path.join(rootPath, 'config.json');
const assetsPath = path.join(rootPath, 'assets');
const equalizerPresetsPath = path.join(assetsPath, 'eq');
const reverbPresetsPath = path.join(assetsPath, 'ir');

// Expose protected methods to the renderer process
contextBridge.exposeInMainWorld('electronAPI', {
    node: () => process.versions.node,
    chrome: () => process.versions.chrome,
    electron: () => process.versions.electron,

    readConfig: () => {
        return JSON.parse(fs.readFileSync(configPath));
    },
    writeConfig: (configJSON) => {
        const tempFilePath = path.join(rootPath, 'config.tmp');
        fs.writeFileSync(tempFilePath, JSON.stringify(configJSON, null, 2));
        fs.renameSync(tempFilePath, configPath);
    },
    getEqualizerPresets: () => {
        const files = fs.readdirSync(equalizerPresetsPath);
        const filesDict = {};

        for (const file of files) {
            const presetJSON = JSON.parse(fs.readFileSync(path.join(equalizerPresetsPath, file)));
            const name = file.split('.')[0];
            filesDict[name] = presetJSON;
        }

        return filesDict;
    },
    getReverbPresets: () => {
        const files = fs.readdirSync(reverbPresetsPath);
        const filesDict = {};

        for (const file of files) {
            const name = file.split('.')[0];
            filesDict[name] = path.join('../assets/ir/', file);
        }

        return filesDict;
    }
});
