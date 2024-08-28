const { app, BrowserWindow } = require('electron')
const { spawn } = require('child_process');
const path = require('path')

const rootPath = path.resolve(path.dirname(__dirname), '..');
const backendProcess = spawn(rootPath + '/build/eq_cpp');

process.on('exit', (code) => {
    if (code !== 0) {
        console.error(`Process exited with code: ${code}`);
        app.quit();
    }
});

process.on('error', (error) => {
    console.error(`Error executing file: ${error}`);
    app.quit();
});

const createWindow = () => {
    win = new BrowserWindow({
        width: 600,
        height: 816,
        backGroundColor: '#0F172A',
        show: false,
        resizable: false,
        fullscreenable: false,
        webPreferences: {
            contextIsolation: true,
            nodeIntegration: false,
            sandbox: false,
            preload: path.join(__dirname, 'preload.js'),
        },
    });

    win.loadFile('src/index.html');

    win.once('ready-to-show', () => {
        win.show();
    });
}

app.on('ready', () => {
    createWindow();
});

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit();
    }
});

app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
        createWindow();
    }
});

app.on('before-quit', () => {
    backendProcess.kill();
});