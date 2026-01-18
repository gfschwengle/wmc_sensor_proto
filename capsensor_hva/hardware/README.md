# Hardware Design Files

This directory contains the KiCAD PCB design files for the Capsensor HVA.

## File Structure

- `kicad/` - KiCAD project files (schematic, PCB layout)
- `outputs/` - Manufacturing outputs
  - `gerbers/` - Gerber files for PCB fabrication
  - `bom/` - Bill of Materials
  - `pick-place/` - Pick and place files for assembly
  - `drawings/` - Assembly drawings and documentation

## Design Overview

The Capsensor HVA hardware consists of:
- High voltage capacitive sensing circuit
- Microcontroller (ESP32-based)
- Power management
- Communication interfaces

## Opening the Design

1. Install KiCAD (version 7.0 or later recommended)
2. Open the project file in the `kicad/` directory
3. Schematic and PCB layout files will be available

## Generating Manufacturing Files

See the documentation in the `docs/` subdirectory for instructions on:
- Generating Gerber files
- Exporting BOM
- Creating pick-and-place files
- Preparing assembly documentation

## Design Documentation

- `docs/pcba.md` - PCB assembly instructions
- `docs/bringup.md` - Hardware bring-up and testing guide
- `docs/revisions.md` - Hardware revision history
