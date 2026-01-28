# SIM Mateo - Clean Energy Exhibit

This project is an interactive **clean energy education exhibit** designed for public outreach and STEM events. Participants can visualize California and San Mateo County’s progress in renewable electricity, using physical controls (Arduino-powered LEDs) and an engaging web interface. It is intended for classroom, science fair, and outreach demonstrations.

---

## Table of Contents

- [Features](#features)
- [System Architecture](#system-architecture)
- [How It Works](#how-it-works)
- [Hardware Setup](#hardware-setup)
- [Software Installation & Usage](#software-installation--usage)
- [Project Structure](#project-structure)
- [Customization](#customization)
- [Troubleshooting](#troubleshooting)
- [License](#license)

---

## Features

- **Physical interactive controls:** Real buttons/cards with LEDs, powered by Arduino (Mega + multiple Nanos), allow participants to "guess" and reveal clean energy share.
- **Dynamic web UI:** Visually shows the clean/dirty electricity mix as cards, with progress milestones, fun sounds, and animations.
- **Educational feedback:** Audio and visual feedback for milestones and correct/incorrect guesses.
- **Real-time control:** Uses Node.js server and WebSockets for live updates and communication between the UI and Arduino hardware.

---

## System Architecture

**Overview:**


- **Arduinos:** NANO boards control specific pairs of LEDs; the MEGA is the master, handling all LED logic, group states, and serial comms.
- **Node.js (server.js):** Bridges the Arduino MEGA to the browser via SerialPort and WebSockets.
- **Browser UI (index.html):** Renders the interactive exhibit, receives real-time LED/card states, sends user actions to the server.

---

## How It Works

1. **Start the Node.js server:** Launches a static file server, serial port listener, and WebSocket server on port 80.
2. **Connect all Arduinos:** MEGA is connected to the PC via USB, and communicates with the NANOs.
3. **User interacts via the web UI:** User clicks/taps virtual cards, triggering commands (green/red) sent to the MEGA.
4. **MEGA updates LEDs:** Updates the LED states physically, and sends back updates for the UI.
5. **UI updates:** Browser receives LED/card state via WebSocket, updates the cards, shows messages, and plays sounds for feedback.

---

## Hardware Setup

**Boards:**  
- 1 x Arduino MEGA (central controller)
- 4 x Arduino NANO (A, C, D, E) for individual LED groups

**Wiring:**  
- Each "pair" (A1, A2, etc.) is connected to digital pins as per the `MEGA.ino` and `NANO_*.ino` files.
- LEDs: Red and Green per group, wired active-low (HIGH = off, LOW = on).
- Connect all boards via Serial (refer to `MEGA.ino` for which hardware Serial is used for each Nano).

---

## Software Installation & Usage

### Requirements

- Node.js (v16+ recommended)
- `serialport`, `ws`, and `express` NPM packages (see `package.json`)
- Arduino IDE for flashing firmware

### 1. Arduino Firmware

- Upload `MEGA.ino` to Arduino MEGA.
- Upload `NANO_A.ino`, `NANO_C.ino`, `NANO_D.ino`, `NANO_E.ino` to their respective NANO boards.

### 2. Node.js Server

Install dependencies:

```bash
npm install
Edit server.js to match your Arduino MEGA serial port (portName, e.g. COM9 for Windows or /dev/ttyACM0 for Linux).

Start the server (as admin/root if needed for port 80):
node server.js
```
3. Web Interface
Open a browser on the same machine and go to http://localhost/ (or your server’s LAN IP).
```bash
/index.html       <-- Main web interface
/server.js        <-- Node.js bridge/server
/package.json     <-- Node.js dependencies
/MEGA.ino         <-- Arduino MEGA firmware
/NANO_A.ino       <-- Arduino NANO (A) firmware
/NANO_C.ino       <-- Arduino NANO (C) firmware
/NANO_D.ino       <-- Arduino NANO (D) firmware
/NANO_E.ino       <-- Arduino NANO (E) firmware
/assets/          <-- Sound, images, and animation assets for UI
```

Customization
UI/UX: Edit index.html and /assets/ for your theme, sounds, and milestone messages.

Milestones: Logic for clean energy % milestones, sound triggers, and animations are in index.html JS.

LED mapping: Pin numbers and logic are in the Arduino .ino files.

Serial port name: Change in server.js if you use a different OS or port number.

Troubleshooting
Serial port not found: Check the portName in server.js matches your Arduino MEGA port.

Permission issues on port 80: Run server as administrator, or change the port to e.g. 8080 in server.js and update the WebSocket URL in index.html.

Sounds not playing: Make sure all .mp3 and other audio files are present in /assets/ and the file names match those in index.html.

LEDs not responding: Double-check wiring, correct firmware flashed, and Serial connections between boards.

License
MIT License.
Feel free to use, modify, and share for non-commercial educational purposes. Please credit the original author(s).

Acknowledgements
Developed for SIM Mateo, College of San Mateo and local STEM outreach.

Special thanks to all contributors, instructors, and students.

bash
Copy
Edit




Schematics and diagrams - https://miro.com/app/board/uXjVNsFsbDc=/

Add as green:
Received from Serial: Room 2: Card#: 73 28 1E 2A
Received from Serial: Room 2: Card#: 53 0E 5C 10
Received from Serial: Room 2: Card#: 73 48 AC E4

Add ad red:
Received from Serial: Room 2: Card#: A3 B9 6D 29
Received from Serial: Room 2: Card#: 43 6D 2D 10

restart:
Received from Serial: Room 2: Card#: 23 9D C2 0D
