# Capsensor HVA Firmware

This directory contains the embedded firmware for the Capsensor HVA device.

## Development Environment

This project uses PlatformIO for development. Install PlatformIO Core or use the PlatformIO IDE extension for VS Code.

### Prerequisites

- PlatformIO Core or PlatformIO IDE
- ESP32 development board (or target hardware)

## Building

```bash
# Build the firmware
pio run

# Upload to device
pio run --target upload

# Open serial monitor
pio device monitor
```

## Project Structure

- `src/` - Main application source code
- `include/` - Header files
- `lib/` - Project-specific libraries
- `test/` - Unit tests
- `docs/` - Firmware-specific documentation

## Configuration

Edit `platformio.ini` to configure build settings, dependencies, and target platform.
