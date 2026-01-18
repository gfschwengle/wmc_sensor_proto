# Capacitive Sensing Data Logger Firmware (capboard-fw)

## What is This Project?

This is a **smart sensor device** that measures and records data continuously. Think of it as a data collection box that:

1. **Measures things** using sensors (capacitive touch/proximity sensors and environmental sensors)
2. **Stores the data** locally on an SD card (like a USB drive attached to the device)
3. **Sends the data** wirelessly to a computer or server via WiFi

The device is based on the **ESP32-C3**, which is a small, low-cost microcontroller (tiny computer) made by Espressif that has built-in WiFi and Bluetooth capabilities.

### What Problem Does It Solve?

If you need to monitor sensor data in a location where:
- You can't connect to the internet directly
- You want a backup copy of data stored locally
- You need real-time data streaming AND offline storage as a backup

This system provides all three capabilities.

### Key Features

- **Multi-Sensor Support**: Reads capacitive sensor data (measures proximity/touch without physical contact) and environmental data (temperature, humidity, pressure)
- **Wireless Connectivity**: Connects to your WiFi network and sends data to a server using MQTT (a lightweight messaging protocol)
- **Local Storage**: Also saves all data to an SD card (like a memory card) as a backup
- **Scheduler-Based Architecture**: Runs multiple tasks at the same time without freezing
- **UART CLI**: Allows you to configure the device via a USB serial cable
- **LED Status Indicators**: Two lights show you when the device is reading sensors and writing to the SD card

## Hardware Platform

**Target Board**: ESP32-C3-DevKitM-1

This is a development board (think of it like a "learning kit") that includes:
- The ESP32-C3 microcontroller (the "brain")
- USB-C port for connecting to your computer
- Built-in WiFi antenna
- Power regulator (converts power to the right voltage)
- Reset button and LED indicator

### What Are All These Sensors?

| Sensor | What It Does | Why You Need It |
|--------|-------------|-----------------|
| **FDC1004** | Measures capacitance (proximity/touch without contact) | Detects when something gets close to the device |
| **BME280** | Measures temperature, humidity, and pressure | Environmental monitoring (weather station capabilities) |

### GPIO Configuration Explained

GPIO stands for "General Purpose Input/Output" - basically, the pins on the microcontroller that can send and receive electrical signals.

| Component | GPIO | Purpose | Simple Explanation |
|-----------|------|---------|-------------------|
| I2C SCL | 3 | I2C clock signal | Synchronization line for talking to sensors |
| I2C SDA | 4 | I2C data signal | Data line for talking to sensors |
| LED Sense | 0 | Sensor reading indicator | Light blinks when reading sensors |
| LED SD | 8 | SD card write indicator | Light blinks when saving to SD card |
| Button | 9 | User input (reserved) | Not currently used, available for future use |
| SD MOSI | 7 | SD card data out | Sends data to SD card |
| SD MISO | 2 | SD card data in | Receives data from SD card |
| SD SCK | 6 | SD card clock | Timing signal for SD card |
| SD CS | 5 | SD card chip select | "Wake up" signal for SD card |
| UART TX | 12 | Serial data output | Sends messages to your computer |
| UART RX | 11 | Serial data input | Receives commands from your computer |

**What is I2C?** It's a standard way for microcontrollers to talk to multiple sensors using just 2 wires (SCL and SDA). Think of it like a "bus" where multiple passengers (sensors) share the same route.

## Project Architecture

### What Do All These Files Do?

Think of the project like a restaurant kitchen:
- **main.c** = The head chef that coordinates everything
- **sampler.c** = The person collecting ingredients (sensor readings)
- **wifi_svc.c** = The delivery driver sending packages (WiFi)
- **mqtt_svc.c** = The package receiving system (MQTT delivery)
- **sd_logger.c** = The filing system (SD card storage)
- **uart_cli.c** = The phone line for taking orders (serial commands)

### Directory Structure

```
├── src/                    # Source code
│   ├── main.c             # Application entry point and initialization
│   ├── sampler.c          # Sensor sampling logic
│   ├── bme280_drv.c       # BME280 environmental sensor driver
│   ├── fdc1004.c          # FDC1004 capacitive sensor driver (WIP)
│   ├── i2c_bus.c          # I2C bus initialization and communication
│   ├── scheduler.c        # Task scheduler for non-blocking operations
│   ├── timebase.c         # Time management and timing utilities
│   ├── wifi_svc.c         # WiFi connectivity service
│   ├── mqtt_svc.c         # MQTT publish/subscribe service
│   ├── sd_logger.c        # SD card logging (WIP)
│   ├── uart_cli.c         # Serial command-line interface
│   └── record.c           # Data record management
├── include/               # Header files (.h)
├── test/                  # Unit tests (PlatformIO Unity)
│   └── test_sampler/      # Sampler module tests
├── build/                 # Build output (generated)
└── platformio.ini         # PlatformIO configuration
```

### Core Services

| Service | Status | What It Does | In English |
|---------|--------|-------------|-----------|
| **I2C Bus** | ✓ Active | Talks to sensors | Handles all sensor communication |
| **BME280 Driver** | ✓ Active | Reads environmental data | Gets temperature, humidity, pressure |
| **Sampler** | ✓ Active | Collects all readings | Grabs sensor data every 1 second, averages 4 readings |
| **Scheduler** | ✓ Active | Runs jobs on a schedule | Like a calendar that triggers tasks at specific times |
| **WiFi Service** | ✓ Active | Connects to WiFi | Joins your home/office WiFi network |
| **MQTT Service** | ⚠ Partial | Sends data to server | Pushes sensor readings to a MQTT broker (needs completion) |
| **SD Logger** | ⚠ Partial | Saves to SD card | Writes data to file (needs completion) |
| **FDC1004 Driver** | ⚠ Partial | Reads capacitive sensor | Measures proximity (needs completion) |

**What is a "Service"?** It's a piece of code that handles one specific job continuously. Services run in the background and wait for their scheduled time to do their work.

## Configuration

### What is Configuration?

Configuration is like adjusting the settings on your device. Instead of changing the code and rebuilding, you can change some numbers to customize how the device behaves.

### Compile-Time Configuration

Edit [include/config.h](include/config.h) to customize. These are the main settings you might want to change:

```c
// How often do we take sensor readings?
#define SAMPLE_PERIOD_MS 1000           // 1000ms = 1 second

// How many readings do we average together?
#define SAMPLE_AVG_COUNT 4              // Average 4 readings for smoother data

// How often do we save data to the SD card?
#define SD_FLUSH_PERIOD_MS 5000         // 5 seconds

// How much data can we hold in memory before saving?
#define RECORD_RING_CAP 64              // 64 readings (buffer size)

// Capacitive sensor settings
#define FDC_RATE_HZ 100                 // Read FDC sensor at 100 times per second
#define FDC_CAP_RANGE_PF 15.0f          // Measure up to 15 picofarads

// WiFi Settings (change these to your network)
#define WIFI_SSID "YOUR_SSID"           // Your WiFi network name
#define WIFI_PSK "YOUR_PASS"            // Your WiFi password

// MQTT Settings (the server that receives your data)
#define MQTT_BROKER_URI "mqtt://raspberrypi.local"
#define MQTT_PORT 1883                  // Standard MQTT port
#define MQTT_CLIENT_ID "esp32c3-capboard-01"  // Unique name for this device
#define MQTT_BASE_TOPIC "capboard/esp32c3-01" // Where to send data
#define MQTT_PUB_PERIOD_MS 200          // Send data to server every 200ms
#define MQTT_QUEUE_DEPTH 128            // How many messages to queue
```

**What do these units mean?**
- **ms** = milliseconds (1000ms = 1 second)
- **Hz** = times per second (100 Hz = 100 readings per second)
- **pF** = picofarads (unit of capacitance)

### I2C Slave Addresses

I2C addresses are like house addresses for each sensor. The microcontroller needs to know which address to send commands to.

- **BME280** (Environmental sensor): 0x77 (or 0x76 if you have a special version)
- **FDC1004** (Capacitive sensor): 0x50

## Firmware Build & Upload

### What Does "Build" and "Upload" Mean?

- **Build** = Compile the code (convert human-readable C code into machine instructions)
- **Upload/Flash** = Send the compiled code to the device via USB

### Prerequisites (One-Time Setup)

Before you can build and upload code, you need to install some tools:

1. **PlatformIO CLI** - [Install here](https://platformio.org/install/cli)
   - This is the software that builds and uploads your code
   - It's like a "build system" - a tool that manages compilation

2. **USB-Serial Driver** - CH340 or similar for ESP32-C3 DevKit
   - This allows your computer to communicate with the device via USB
   - Install the driver for your specific board if the device isn't recognized

3. **USB Cable** - Connected to the DevKit's USB-C port
   - Make sure it's a data cable (not just a power-only cable)
   - You'll know it works if you can see the device in Device Manager

### Check Your Installation

Open a terminal/command prompt and type:
```bash
pio --version
```

If you see a version number, you're ready to go!

### Build (Step 1: Compile the Code)

```bash
# Build for ESP32-C3
pio run -e esp32-c3-wroom-02
```

**What happens?**
- Files are compiled into machine code
- A `.bin` file is created (the firmware binary)
- If there are errors, they'll be shown in red
- If successful, you'll see "SUCCESS" at the end

### Upload (Step 2: Send Code to Device)

```bash
# Flash the device
pio run -e esp32-c3-wroom-02 -t upload
```

**What happens?**
1. Automatically finds the USB port with your device
2. Device goes into "bootloader mode" (automatic, no button press needed)
3. Binary file is sent to the device's flash memory
4. Device verifies the upload was successful
5. Device automatically reboots and runs the new code

**What if the upload fails?**
- Check that the USB cable is connected
- Try a different USB port
- Manually enter bootloader mode by pressing BOOT button, then RESET, then release BOOT

### Monitor Serial Output (Step 3: Watch What Happens)

```bash
# Open serial monitor at 115200 baud
pio device monitor -b 115200
```

**What you'll see:**
- Log messages from the device showing what it's doing
- Temperature and humidity readings
- WiFi connection status
- MQTT messages being sent
- Any errors that occur

You can type commands here if the UART CLI is active.

**Exit the monitor:** Press `Ctrl+C`

Or use any serial terminal software (like PuTTY, Tera Term, or minicom) with these settings:
- **COM Port**: COMx (where x is a number, like COM3)
- **Baud Rate**: 115200
- **Data Bits**: 8
- **Stop Bits**: 1
- **Parity**: None
- **Flow Control**: None

### All-in-One: Build, Upload, and Monitor (Step 1-3 Together)

```bash
# Build, flash, and open monitor in one command
pio run -e esp32-c3-wroom-02 -t upload && pio device monitor -b 115200
```

This does all three steps automatically!

## Runtime Operation

### What Happens When You Power On the Device?

Think of this like a morning routine - the device goes through steps in order:

1. **I2C Bus Initialization** 
   - "Wake up, let's set up how to talk to sensors"
   - Configures the communication protocol

2. **BME280 Probe** 
   - "Is the temperature/humidity sensor there?"
   - Tries to find the sensor

3. **LED Configuration** 
   - "Set up the status lights"
   - Prepares the GPIO pins for LEDs

4. **Sampler Setup** 
   - "Get the data collection system ready"
   - Prepares buffers to hold sensor readings

5. **UART CLI Start** 
   - "Turn on the command interface"
   - You can now type commands via serial

6. **WiFi Init** 
   - "Prepare WiFi stack"
   - Doesn't connect yet - just prepares

7. **WiFi Connection Wait** 
   - "Try to connect to WiFi"
   - If you haven't set WiFi credentials, the device will wait and ask via serial terminal
   - You can provide WiFi username/password through the UART command interface

8. **MQTT Init** 
   - "Set up the data sending system"
   - Only happens after WiFi connects

9. **Sampling Start** 
   - "Begin reading sensors"
   - Starts collecting temperature, humidity, and capacitive data

### Scheduler Jobs (Regular Tasks)

The device runs these tasks on a schedule (like a calendar of events):

| Job | How Often | What It Does |
|-----|-----------|------------|
| `sampler_job` | Every 1000ms (1 second) | Read all sensors, average readings, store in memory |
| `mqtt_svc_job_drain` | Every 200ms | Take readings from memory and send to MQTT server |
| `wifi_svc_job_report` | Every 10000ms (10 seconds) | Check WiFi status and log it |
| `sdlog_job_flush` | Every 5000ms (5 seconds) | Save data from memory to SD card (currently disabled) |

**Why this design?** The scheduler allows all these tasks to run at their own pace without blocking each other. It's like a cooperative multi-tasking system.

### Serial Commands (UART CLI)

You can type commands in the serial monitor to control the device:
- Set WiFi credentials
- View current readings
- Change settings
- Get system diagnostics

(Full command list depends on implementation in `uart_cli.c`)

## Troubleshooting

### Device Won't Upload

**Problem**: "Could not auto-detect COMX port"

**Solutions**:
1. Check USB cable is connected to the USB-C port (not another port)
2. Install the CH340 USB-to-Serial driver if not already installed
3. Try different USB ports
4. In Device Manager (Windows), find the device and note the COM number, then:
   ```bash
   pio run -e esp32-c3-wroom-02 -t upload --upload-port COM3
   ```
   (Replace COM3 with your actual port)

### Serial Monitor Shows Garbage Text

**Problem**: Text looks like random symbols instead of readable words

**Solution**: Make sure baud rate is set to **115200** exactly. Common wrong values:
- 9600 (too slow)
- 230400 (too fast)

### Device Keeps Rebooting

**Problem**: You see "Starting system" message repeatedly

**Possible causes**:
- I2C sensors not connected or not working
- Power supply not stable
- Code has bugs causing crashes

**Debug**: Watch the serial monitor to see where it gets stuck

### WiFi Won't Connect

**Problem**: Device says "Still waiting for WiFi connection..."

**Solutions**:
1. Check WiFi SSID and password in `config.h` are correct
2. Make sure your WiFi network is on 2.4 GHz (not 5 GHz)
3. Try entering WiFi credentials via the UART CLI
4. Check that your WiFi router is nearby

### No Data Appearing in Serial Monitor

**Problem**: Serial monitor is blank

**Solutions**:
1. Check COM port is correct
2. Make sure device is powered (look for LED on board)
3. Try pressing the RESET button on the board
4. Check baud rate is 115200

## Known Issues & Work in Progress

| Component | Issue | Impact |
|-----------|-------|--------|
| **FDC1004 Driver** | Register configuration not yet implemented | Capacitive sensor won't work until completed |
| **SD Logger** | CSV file writer not yet implemented; SD logging disabled | Data only stored in memory, not on SD card |
| **MQTT Service** | JSON formatting not complete; needs testing | WiFi data sends but may not be formatted correctly |

**What does "WIP" mean?** "Work in Progress" - it's started but not finished yet.

## Known Issues & Work in Progress

## For Complete Beginners: Step-by-Step First Run

### What You Need

- ESP32-C3-DevKitM-1 board
- USB-C cable
- Computer with Windows/Mac/Linux
- WiFi router with 2.4 GHz band

### Step-by-Step

**Step 1: Install PlatformIO** (5 minutes)
- Go to https://platformio.org/install/cli
- Follow the installer for your operating system
- Restart your terminal when done

**Step 2: Connect Your Board** (1 minute)
- Plug ESP32-C3 into USB port
- Look in Device Manager (Windows) to find the COM port (e.g., COM3)
- Note the COM number

**Step 3: Edit WiFi Settings** (2 minutes)
- Open `include/config.h` in a text editor
- Find these lines:
  ```c
  #define WIFI_SSID "YOUR_SSID"    // change to your WiFi name
  #define WIFI_PSK "YOUR_PASS"     // change to your WiFi password
  ```
- Save the file

**Step 4: Build and Upload** (3-5 minutes)
- Open terminal in project folder
- Type:
  ```bash
  pio run -e esp32-c3-wroom-02 -t upload && pio device monitor -b 115200
  ```
- Wait for upload to complete

**Step 5: Watch It Work** (ongoing)
- You'll see log messages in the monitor
- Device will attempt to connect to WiFi
- Once connected, you'll see sensor readings
- Temperature and humidity values will appear

### What the Output Means

```
I (123) app: Starting system                   ← Device is powering up
I (456) app: I2C bus initialized               ← I2C communication ready
I (789) bme: BME280 detected at 0x77           ← Temperature sensor found
I (1000) wifi: WiFi initialized                ← WiFi is ready
I (5000) wifi: Waiting for WiFi connection...  ← Trying to join your network
I (8000) wifi: Connected! IP: 192.168.1.100    ← Success! Device is on WiFi
I (10000) mqtt: Publishing temperature: 22.5   ← Sending data to server
```

If you see `E (error)` messages, something is wrong - check Troubleshooting section above.

## Development & Testing

### Run Unit Tests

```bash
pio test -e esp32-c3-wroom-02
```

### Run Unit Tests

Tests verify that individual pieces of code work correctly before running on the device.

```bash
pio test -e esp32-c3-wroom-02
```

Tests are located in [test/test_sampler/](test/test_sampler/) and use PlatformIO's Unity framework.

## Logging Levels

**What is a "logging level"?** It controls how much information gets printed:

- **ESP_LOG_ERROR**: Only show when something breaks
- **ESP_LOG_WARN**: Show errors and warnings (like "sensor missing")
- **ESP_LOG_INFO**: Show normal operations (default - what you usually want)
- **ESP_LOG_DEBUG**: Show everything including detailed internal info

To change the level, edit [platformio.ini](platformio.ini):

```ini
build_flags = -D LOG_LOCAL_LEVEL=ESP_LOG_DEBUG   # Much more verbose
```

## Pin Reconfiguration (If Using Different Hardware)

If your board has sensors on different pins, edit [include/board.h](include/board.h) to match your hardware.

**Example:** If your temperature sensor is on GPIO 5 instead of GPIO 4:
```c
#define I2C_SDA_GPIO    5        // change this line
```

Then rebuild and upload.

## Understanding the Data Flow

Here's how data moves through the system:

```
Sensors (BME280, FDC1004)
         ↓
    I2C Bus
         ↓
   Sampler (reads every 1 second)
         ↓
   Memory Buffer (stores readings)
    ↙          ↘
SD Card      MQTT Broker
(local file) (cloud/server)
```

1. Sensors measure environment and capacitance
2. Every 1 second, sampler reads sensors via I2C
3. Readings go into a memory buffer
4. Data is either:
   - Saved to SD card file (every 5 seconds)
   - Sent to MQTT server (every 200ms)

This way you get:
- **Real-time cloud data** via MQTT
- **Backup local data** on SD card

## Next Development Steps

These are the things that still need to be done to complete the project:

1. **Finish FDC1004 Driver** 
   - What's needed: Complete sensor register configuration
   - What it does: Enables the capacitive proximity sensor
   - Where: `src/fdc1004.c`

2. **Complete SD Logger** 
   - What's needed: Implement CSV file writing
   - What it does: Saves all readings to a file on SD card
   - Where: `src/sd_logger.c`

3. **Finalize MQTT** 
   - What's needed: Complete JSON data formatting
   - What it does: Formats sensor readings as JSON before sending
   - Where: `src/mqtt_svc.c`

4. **Add WiFi Provisioning** 
   - What's needed: BLE or web interface for entering WiFi credentials
   - What it does: Let users set WiFi without editing code
   - This would make the device much more user-friendly

5. **Persistent Configuration Storage** 
   - What's needed: Save settings to NVS (Non-Volatile Storage)
   - What it does: Remember WiFi credentials after power loss
   - This prevents having to reconfigure every time

## Glossary (Technical Terms Explained Simply)

| Term | Explanation |
|------|-------------|
| **Firmware** | The software that runs on the microcontroller (like an OS for a tiny computer) |
| **Microcontroller** | A tiny computer chip that controls hardware (GPIO, sensors, etc.) |
| **GPIO** | General Purpose Input/Output - pins that can read or send electrical signals |
| **I2C** | A simple protocol (standard way) for microcontrollers to talk to multiple sensors using just 2 wires |
| **UART** | A serial communication protocol - lets you send text via USB cable |
| **MQTT** | A lightweight messaging system for sending data over internet/WiFi |
| **Broker** | A server that collects and distributes messages (like a post office) |
| **WiFi** | Wireless network communication at 2.4 GHz or 5 GHz |
| **SD Card** | Secure Digital card - physical storage device (like a USB drive) |
| **CSV** | Comma-Separated Values - a simple file format for storing data (opens in Excel) |
| **Baud Rate** | Speed of serial communication (115200 = 115,200 bits per second) |
| **Bootloader** | Special code that allows you to upload new firmware |
| **Interrupt** | When the processor stops what it's doing to handle something urgent |
| **Buffer** | Temporary memory storage for data |
| **Compile/Build** | Converting human-readable code into machine instructions |
| **Flashing** | Writing firmware to device's permanent storage |
| **I2C Address** | Like a house address for each sensor on the I2C bus |
| **Sensor** | A device that measures something (temperature, capacitance, etc.) |
| **Capacitive** | Related to detecting changes in electrical field (touch/proximity sensing) |

## Where to Get Help

- **PlatformIO Docs**: https://docs.platformio.org/ (how to use the build system)
- **ESP-IDF Guide**: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/ (ESP32-C3 programming)
- **FDC1004 Datasheet**: https://www.ti.com/product/FDC1004 (capacitive sensor specs)
- **BME280 Datasheet**: https://www.bosch-sensortec.com/products/environmental-sensors/humidity-sensors-bme280/ (temperature sensor specs)
