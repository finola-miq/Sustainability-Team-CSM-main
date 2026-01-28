// in this file, set up the environment and websocket

import { SerialPort } from "serialport";
import { ReadlineParser } from "@serialport/parser-readline";
import express from "express";
import http from "http";
import { WebSocketServer, WebSocket } from "ws";

const app = express();
const port = 80;

// ✅ Correct the serial port name for Windows
const portName = "COM10"; // Ensure this matches your detected port
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

// ✅ Prevent WebSocket from crashing if connection is lost
parser.on("data", (id) => {
  console.log(`Received from Serial: ${id}`);
  if (primaryWs && primaryWs.readyState === WebSocket.OPEN) {
    primaryWs.send(id);
  } else {
    console.warn("WebSocket is not open. Data not sent.");
  }
});

// WebSocket setup
const server = http.createServer(app);
const wss = new WebSocketServer({ server });

let primaryWs = null; // ✅ Initialize WebSocket reference properly

wss.on("connection", (ws) => {
  console.log("WebSocket client connected");

  ws.on("message", (message) => {
    console.log(`Message Received => ${message}`);
  });

  ws.send("Welcome to WebSocket server");

  primaryWs = ws; // ✅ Assign WebSocket object properly
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
