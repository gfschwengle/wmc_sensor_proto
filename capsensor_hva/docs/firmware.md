# Firmware Documentation

## Overview

The Capsensor HVA firmware is built using the Arduino framework with PlatformIO as the build system. It runs on an ESP32 microcontroller and handles sensor data acquisition, processing, and communication.

## Development Environment

### Prerequisites

- **PlatformIO**: Install PlatformIO Core or use the IDE extension
- **Git**: For version control
- **Serial Terminal**: For debugging (PlatformIO includes one)

### Setup

1. Clone the repository
2. Navigate to the `firmware/` directory
3. Build (dependencies will be installed automatically): `pio run`

## Architecture

### Main Components

1. **Sensor Interface**
   - ADC reading for capacitance measurement
   - Signal processing and filtering
   - Calibration routines

2. **Data Processing**
   - Raw sensor data conversion
   - Moisture content calculation
   - Statistical averaging and filtering

3. **Communication**
   - Serial debugging output
   - WiFi/Bluetooth connectivity (optional)
   - Data logging

4. **System Management**
   - Power management
   - Configuration storage
   - Error handling

## File Structure

```
firmware/
├── src/
│   └── main.cpp          # Main application entry point
├── include/              # Header files
├── lib/                  # Project-specific libraries
├── test/                 # Unit tests
└── platformio.ini        # PlatformIO configuration
```

## Building and Uploading

### Build Commands

```bash
# Clean build
pio run --target clean

# Build firmware
pio run

# Build and upload
pio run --target upload

# Build specific environment
pio run -e esp32dev
```

### Upload Methods

1. **USB Serial**: Standard method for development
2. **OTA (Over-The-Air)**: For deployed devices (if implemented)
3. **JTAG**: For debugging (requires hardware debugger)

## Configuration

### PlatformIO Configuration

Edit `platformio.ini` to configure:
- Target board and platform
- Build flags and defines
- Library dependencies
- Upload settings
- Serial monitor settings

Example configuration:
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
```

### Firmware Configuration

Configuration parameters should be defined in a config header or stored in non-volatile memory:
- Sensor calibration values
- Communication settings
- Operational parameters
- Debug levels

## Development

### Coding Standards

- Follow the existing code style
- Comment complex algorithms
- Use meaningful variable names
- Keep functions focused and modular

### Adding Features

1. Create feature branch
2. Implement and test locally
3. Document changes
4. Submit for review

### Debugging

#### Serial Debug Output

```cpp
Serial.println("Debug message");
Serial.printf("Value: %d\n", value);
```

#### Debug Levels

Set debug level in `platformio.ini`:
```ini
build_flags = 
    -DCORE_DEBUG_LEVEL=3  ; 0=None, 1=Error, 2=Warn, 3=Info, 4=Debug, 5=Verbose
```

#### Hardware Debugging

- Use JTAG debugger with ESP32
- Configure in PlatformIO for debugging support
- Set breakpoints and inspect variables

## Testing

### Unit Tests

Place test files in the `test/` directory:

```cpp
#include <unity.h>

void test_sensor_reading() {
    int value = readSensor();
    TEST_ASSERT_TRUE(value > 0);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_sensor_reading);
    UNITY_END();
}

void loop() {}
```

Run tests:
```bash
pio test
```

### Integration Testing

- Test on actual hardware
- Verify sensor readings with known references
- Test all communication interfaces
- Validate power consumption

## Sensor Algorithm

### Capacitance Measurement

1. **Signal Generation**: Create AC signal for capacitive sensing
2. **Measurement**: Read sensor response via ADC
3. **Processing**: Convert raw ADC values to capacitance
4. **Calibration**: Apply calibration factors

### Moisture Calculation

The moisture content is derived from capacitance measurements:
- Higher capacitance indicates higher moisture
- Calibration curve maps capacitance to moisture percentage
- Temperature compensation may be required

### Calibration Procedure

1. Measure dry reference sample
2. Measure wet reference samples at known moisture levels
3. Create calibration curve
4. Store calibration data in firmware or EEPROM

## Communication Protocols

### Serial Interface

- **Baud Rate**: 115200
- **Format**: 8N1 (8 data bits, no parity, 1 stop bit)
- **Commands**: TBD - define command protocol

### WiFi (Optional)

If WiFi functionality is implemented:
- Configure SSID and password
- Implement web server for data access
- Support OTA updates

### Data Format

Define consistent data output format:
```
Timestamp, Capacitance, Moisture%, Temperature, Status
```

## Power Management

### Sleep Modes

Implement power-saving features:
- Deep sleep between measurements
- WiFi off when not needed
- Peripheral power control

### Battery Operation

If battery-powered:
- Monitor battery voltage
- Implement low-battery warnings
- Optimize measurement intervals

## Troubleshooting

### Common Issues

1. **Upload Fails**
   - Check USB connection
   - Verify correct COM port
   - Hold BOOT button during upload (if needed)

2. **No Serial Output**
   - Check baud rate matches monitor_speed
   - Verify USB cable supports data (not charge-only)
   - Check serial monitor is connected to correct port

3. **Sensor Readings Incorrect**
   - Verify hardware connections
   - Check ADC configuration
   - Review calibration values

## API Reference

### Key Functions

Document main API functions:

```cpp
// Initialize sensor system
void sensorInit();

// Read raw sensor value
int readRawSensor();

// Get calibrated moisture reading
float getMoistureContent();

// Update configuration
void setConfig(Config* config);
```

## Contributing

When contributing firmware changes:
- Test thoroughly on hardware
- Update documentation
- Follow coding standards
- Add comments for complex code

## Resources

- [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [Arduino Framework Reference](https://www.arduino.cc/reference/en/)
