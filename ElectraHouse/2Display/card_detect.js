// in this file, set up the environment and websocket

import { SerialPort } from "serialport";
import { ReadlineParser } from "@serialport/parser-readline";
import express from "express";
import http from "http";
import { WebSocketServer, WebSocket } from "ws";
import path from "path";
import { fileURLToPath } from "url";

const app = express();
const __dirname = path.dirname(fileURLToPath(
    import.meta.url));

// Serve static files (CSS, JS, videos, etc.)
app.use(express.static(path.join(__dirname, "public")));

// Serve display1.html at /display1
app.get("/display1", (req, res) => {
    res.sendFile(path.join(__dirname, "public", "display1.html"));
});

// Serve display2.html at /display2
app.get("/display2", (req, res) => {
    res.sendFile(path.join(__dirname, "public", "display2.html"));
});
const port = 3000;

// ✅ Correct the serial port name for Windows
const portName = "/dev/ttyUSB0"; // Ensure this matches your detected port on Raspberry Pi
const serialPort = new SerialPort({
    path: portName,
    baudRate: 9600,
    autoOpen: false, // Prevent automatic opening to handle errors manually
});

// ✅ Handle serial port errors
serialPort.open((err) => {
    if (err) {
        console.error(`Failed to open ${portName}:`, err.message);
        process.exit(1); // Stop the script if the serial port cannot open
    } else {
        console.log(`Serial port ${portName} opened successfully`);
    }
});

// ✅ Add error handling for serial port
serialPort.on("error", (err) => {
    console.error("Serial port error:", err.message);
});

// ✅ Handle port disconnection
serialPort.on("close", () => {
    console.error(`Serial port ${portName} was closed.`);
});

// ✅ Ensure parser is correctly attached
const parser = serialPort.pipe(new ReadlineParser({ delimiter: "\n" }));

// WebSocket setup
const server = http.createServer(app);
const wss = new WebSocketServer({ server });

// Store all connected clients
let clients = [];

wss.on("connection", (ws) => {
    console.log("WebSocket client connected");
    clients.push(ws);

    ws.on("message", (message) => {
        console.log(`Message Received => ${message}`);
    });

    ws.on("close", () => {
        clients = clients.filter(client => client !== ws);
    });

    ws.send("Welcome to WebSocket server");
});

parser.on("data", (id) => {
    console.log(`Received from Serial: ${id}`);
    clients.forEach(ws => {
        if (ws.readyState === WebSocket.OPEN) {
            ws.send(id);
        }
    });
});

// ✅ Prevent COM port locking on restart
process.on("SIGINT", () => {
    console.log("Closing serial port...");
    serialPort.close(() => {
        console.log("Serial port closed. Exiting process.");
        process.exit(0);
    });
});

// Start the server
server.listen(port, () => {
    console.log(`Server is running at http://localhost:${port}/`);
});
