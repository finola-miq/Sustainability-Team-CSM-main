import { SerialPort } from "serialport";
import { ReadlineParser } from "@serialport/parser-readline";
import express from "express";
import http from "http";
import { WebSocketServer, WebSocket } from "ws";
import { fileURLToPath } from 'url';
import path from 'path';
import { exec } from 'child_process'; // We need this to run the echo command

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

function log(message) {
    const timestamp = new Date().toISOString();
    console.log(`[${timestamp}] ${message}`);
}

log('SERVER STARTING');

const app = express();
const port = 80;

app.use(express.static(__dirname));

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

const server = http.createServer(app);
const wss = new WebSocketServer({ server });

let primaryWs = null;
let messageQueue = [];
let arduinoSerial = null;
let currentPortPath = ""; // We need to remember the path (e.g., /dev/ttyACM0)

async function startSerial() {
    try {
        const ports = await SerialPort.list();
        const arduinoPort = ports.find(p => p.path.includes('/dev/ttyUSB') || p.path.includes('/dev/ttyACM'));

        if (!arduinoPort) {
            log("No Arduino found. Waiting...");
            setTimeout(startSerial, 5000);
            return;
        }

        currentPortPath = arduinoPort.path; // Save the path for the echo command
        
        arduinoSerial = new SerialPort({ path: currentPortPath, baudRate: 9600 });
        const parser = arduinoSerial.pipe(new ReadlineParser({ delimiter: "\n" }));

        log(`Connected to Arduino at: ${currentPortPath}`);

        parser.on("data", (data) => {
            const command = data.trim().toLowerCase();
            log(`[Arduino→Server] ${command}`);
            if (primaryWs && primaryWs.readyState === WebSocket.OPEN) {
                primaryWs.send(command);
                log(`[Server→Browser] Forwarded: ${command}`);
            } else {
                messageQueue.push(command);
            }
        });
        
        arduinoSerial.on('error', (err) => {
            log(`[Serial ERROR] ${err.message}`);
        });
        
        arduinoSerial.on('close', () => {
            log('[Serial] Connection closed temporarily');
        });
        
    } catch (e) { 
        log(`[Serial Error] ${e.message}`);
        setTimeout(startSerial, 5000);
    }
}

wss.on("connection", (ws) => {
    log("Browser connected via WebSocket");
    primaryWs = ws;

    ws.on("message", (data) => {
        const message = data.toString().trim();
        log(`[Browser→Server] Received: ${message}`);
        
        if (message === "RESTART") {
            log("Running system ECHO command to force reset...");
            
            // 1. Close the official connection so the file isn't "Busy"
            if (arduinoSerial && arduinoSerial.isOpen) {
                arduinoSerial.close((err) => {
                    // 2. Run the exact Linux command that worked for you
                    // We use currentPortPath (e.g. /dev/ttyACM0)
                    exec(`echo "RESTART" > ${currentPortPath}`, (error, stdout, stderr) => {
                        if (error) {
                            log(`[Echo Failed] ${error.message}`);
                        } else {
                            log(`[Echo Success] Command sent via shell.`);
                        }
                        
                        // 3. Reconnect to the Arduino immediately after
                        setTimeout(startSerial, 500); 
                    });
                });
            }
        } 
        else {
            // Normal behavior for other messages
            if (arduinoSerial && arduinoSerial.isOpen) {
                arduinoSerial.write(message + "\n");
                log(`[Server→Arduino] Forwarded: ${message}`);
            }
        }
    });

    while(messageQueue.length > 0) { 
        const msg = messageQueue.shift();
        ws.send(msg);
    }
});

server.listen(port, () => {
    log(`Server running on http://localhost:${port}`);
    startSerial();
});