# Yggdrasill Drone Swarm Control System
# Yggdrasill Drone Swarm Control System

**Yggdrasill** is a drone swarm control system consisting of two main components:

- **`yggdrasill-ctrl`** – Control software running on a ground station (e.g., laptop or PC)  
- **`grein-ctrl`** – Onboard control software running on each drone's Raspberry Pi

The system is designed to communicate with **Betaflight-based** flight controllers using **UART** and a custom protocol called **Rot**.

---

## Project Structure

- **`yggdrasill-ctrl`** – Sends control commands to drones over **UDP**
- **`grein-ctrl`** – Receives UDP packets, translates them into **CRSF** (and future **SBUS**) signals, and transmits them to the flight controller

---

Installation & Bu---

## Installation & Build

To build the project:

1. Run the installation script:

```bash
./install.sh
```
This will install:
- **hidapi** (for joystick support)  
- **ARM cross-compiler**  
- **PlatformIO** (if required)

2. Compile the project:

```bash
make
```
3. The binaries will be located in the `build/bin/` directory:
   - `yggdrasill-ctrl`
   - `grein-ctrl`

---

## Hardware Setup

Each drone must have:
- **Raspberry Pi** (e.g., Pi Zero W)
- **UART connection** to the flight controller:
  - Pin 6 — GND
  - Pin 8 — TX
  - Pin 10 — RX
  - Refer to Raspberry Pi GPIO UART documentation for details
- **CSI Camera** (optional)
- **Wireless Antenna**: USB or custom-soldered (recommended)

---

## Rot Protocol

The custom control packet format used for communication is:

```c
struct Rot {
    uint8_t header;
    uint16_t throttle;
    uint16_t yaw;
    uint16_t pitch;
    uint16_t roll;
    uint16_t arm;
    bool manual;
};
```

## Grein Setup (On Drone)

1. Copy `grein-ctrl` to the Raspberry Pi:

```bash
scp build/bin/grein-ctrl opend@192.168.3.1:~
```

2. SSH into the Raspberry Pi:

```bash
ssh opend@192.168.3.1
```

3. Run the binary:
```bash
./grein-ctrl
```

4. (Optional) Configure auto-start with systemd. (TODO)

- Default UART port: `/dev/ttyAMA0`  
- Default UDP port: `8070`

The `grein-ctrl` binary listens for **Rot** packets over UDP, converts them to **CRSF** signals, and sends them to the Betaflight controller.

---

## Yggdrasill Control (Ground Station)

Supports:

### Keyboard Control:

| Key | Function      |
|------|--------------|
| r    | Throttle ↑   |
| f    | Throttle ↓   |
| c    | Throttle = 120 |
| q    | Yaw ←        |
| e    | Yaw →        |
| w    | Pitch ↑      |
| s    | Pitch ↓      |
| a    | Roll ←       |
| d    | Roll →       |
| z    | Disarm (1600)|
| x    | Arm (900)    |

## RealFlightRC Joystick (6-channel)

The joystick sends **8-byte packets** structured as follows:

| Byte # | Bits          | Description         |
|--------|---------------|---------------------|
| 1      | `YYYY YYYY`   | Yaw                 |
| 2      | `PPPP PPPP`   | Pitch               |
| 3      | `RRRR RRRR`   | Roll                |
| 4      | `YYTT PPRR`   | Yaw, Throttle, Pitch, Roll combined bits |
| 5      | `TTTT TTTT`   | Throttle            |
| 6      | `nnnn nnnn`   | Not used            |
| 7      | `nnnn nnnn`   | Not used            |
| 8      | `nnnn nnAA`   | Not used + Arm bits |

### Legend

| Symbol | Meaning    |
|--------|------------|
| **Y**  | Yaw        |
| **T**  | Throttle   |
| **P**  | Pitch      |
| **R**  | Roll       |
| **A**  | Arm        |
| **n**  | Not used   |

---
## Running the System

1. Power on the drone's Raspberry Pi and ensure it is connected to the network.  
2. Run `grein-ctrl` on the drone as described above.  
3. On your ground station, run:  
```bash
./build/bin/yggdrasill-ctrl
```

**The controller will connect to 192.168.3.1:8070 and begin transmitting control commands to the drone!**

## Enjoy flying with Yggdrasill!