                       Data-Driven Vehicle Control Using CAN Protocol (LPC2129)

## 📝 Project Summary

This project implements a **robust and fault-tolerant vehicle control system** over the **CAN protocol** using **LPC2129 microcontrollers**. It features three functional nodes:

- 🟢 **Main Node (Dashboard)**
- 🔁 **Reverse Node (Distance Sensing)**
- 🪟 **Window Glass Control Node**

### ✅ Key Highlights
- **CAN-based communication** with node-specific messaging.
- **Main Node generates interrupt-driven requests**, and only the **intended node responds**.
- **Modular & fault-tolerant**: Main Node works **independently** and continues displaying temperature even if other nodes are removed.
- Simulates realistic vehicle operations using sensors, interrupts, LCDs, and LEDs.

---

## 🧠 System Architecture

### 📟 Main Node (Dashboard)
- Reads engine temperature from a **DS18B20 sensor** on `P0.12` and displays it continuously.
- Sends control or data request messages when external interrupts are triggered:
  - Reverse check request (on reverse switch press).
  - Window open/close request (on respective switch presses).
- Receives responses from other nodes and reacts accordingly:
  - Blinks a reverse warning LED on `P0.13` if an obstacle is detected within 20 cm.
  - Updates LED indicators or status displays when receiving window node responses.
- Operates **independently** in case of CAN or other node failure, ensuring core functionality.

### 🔁 Reverse Node (Distance Sensor Node)
- Waits for a request message from the Main Node.
- When requested, reads analog distance data using **GP2D12 IR sensor** connected to `P0.27` via ADC.
- Sends back the distance measurement to the Main Node.
- Makes **no autonomous decisions** — functions purely as a responder.

### 🪟 Window Glass Control Node
- Waits for a control message from the Main Node.
- Upon receiving a command, it:
  - Adjusts simulated window position (using LEDs `P0.0–P0.7`).
  - Responds back to the Main Node with status or confirmation.
- Movement is done in steps (e.g., one LED ON per open/close command).

---

## 💡 Design Philosophy: Fault-Tolerant Architecture

- Nodes act **only on valid messages intended for them**.
- **CAN messages are filtered** per node functionality.
- If any node is disconnected:
  - The rest of the system continues functioning.
  - The Main Node continues to operate and display temperature data.
- All inter-node logic is interrupt- and request-based — **no polling required**.

---

## 🔌 Hardware Connections

| Component | Pin(s)     | Node        | Description                              |
|----------|------------|-------------|------------------------------------------|
| LCD      | P0.0–P0.7  | Main Node   | LCD Data pins                            |
| RS       | P0.8       | Main Node   | LCD Register Select                      |
| EN       | P0.9       | Main Node   | LCD Enable                               |
| DS18B20  | P0.12      | Main Node   | Temperature Sensor                       |
| Switch 1 | EINT1/P0.14| Main Node   | Triggers window open command             |
| Switch 2 | EINT2/P0.15| Main Node   | Triggers window close command            |
| Switch 3 | EINT3/P0.16| Main Node   | Triggers reverse distance request        |
| GP2D12   | P0.27 (ADC)| Reverse Node| Distance sensor input                    |
| LED Warn | P0.13      | Main Node   | Reverse alert indicator (blinks on object detection) |
| LEDs     | P0.0–P0.7  | Window Node | Simulated window control with LEDs       |

---

## 🧪 Execution Flow

### 🔧 Initialization Phase
- All nodes initialize their respective modules:
  - Main Node sets up LCD, DS18B20, CAN, and interrupts.
  - Reverse Node sets up ADC and CAN.
  - Window Node sets up LEDs and CAN interface.

### ▶️ Operational Workflow
- **Temperature Monitoring**:
  - The Main Node continuously reads and displays the current engine temperature.
- **Reverse Distance Check**:
  - On pressing the reverse mode switch, the Main Node sends a request to the Reverse Node.
  - The Reverse Node reads distance from the GP2D12 sensor and sends it back.
  - If the received distance is less than 20 cm, the Main Node blinks the reverse warning LED.
- **Window Glass Control**:
  - When the window open or close switch is pressed, the Main Node sends a step control request.
  - The Window Node responds by turning on/off LEDs to simulate glass movement.
  - It sends an update back, which may be used for display or feedback.
- **Node-Specific Message Handling**:
  - Although messages are broadcast on the CAN bus, only the **intended node** processes the data and responds.

---

## 🔁 Fault Tolerance

| Node Removal Scenario | Outcome |
|------------------------|---------|
| 🔁 Reverse Node removed | Main Node and Window Node continue to function normally; reverse LED won't blink. |
| 🪟 Window Node removed | Main Node and Reverse Node continue to work; window control response is unavailable. |
| Both 🔁 Reverse Node and 🪟 Window Node removed | Main Node continues running and displays temperature on LCD. |
| Any CAN failure         | Temperature display remains functional on the Main Node. |
| Only Main Node present  | Displays temperature and handles interrupts gracefully. |

---

## 🔌 Hardware Requirements

- LPC2129 Microcontrollers (3 units)
- MCP2551 CAN Transceivers (3 units)
- DS18B20 Temperature Sensor
- GP2D12 IR Distance Sensor
- 16x2 Character LCD Module
- 8 General Purpose LEDs (for window simulation)
- 3 Push Button Switches (for interrupt inputs)
- USB to UART Converter (for flashing via Flash Magic)
- Regulated 5V Power Supply
- Wires, connectors, and breadboard or PCB

---

## 💻 Software Requirements

- Keil µVision IDE (with ARM compiler)
- Flash Magic (LPC2129 programmer)
- Embedded C libraries for LCD, ADC, CAN
- Interrupt-driven firmware design
- Fault-tolerant architecture implementation
- **Language**: Embedded C
- **Compiler**: Keil µVision (ARM)
- **Programmer**: Flash Magic
- **CAN Communication**: MCP2551-based transceiver

