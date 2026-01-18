# KiCAD Design Files

This directory will contain the KiCAD project files.

## Files

When the design is created, this directory will include:

- `*.kicad_pro` - KiCAD project file
- `*.kicad_sch` - Schematic file
- `*.kicad_pcb` - PCB layout file
- `fp-lib-table` - Footprint library table
- `sym-lib-table` - Symbol library table

## KiCAD Version

Recommended: KiCAD 7.0 or later

## Getting Started

1. Install KiCAD from [https://www.kicad.org/](https://www.kicad.org/)
2. Open the `.kicad_pro` project file
3. Schematic editor: Edit the schematic
4. PCB editor: Edit the PCB layout

## Design Guidelines

- Follow the design rules from PCB manufacturer
- Maintain proper clearances for high voltage
- Use appropriate trace widths for current requirements
- Include test points for debugging
- Add fiducials for assembly

## Generating Outputs

See the main hardware README for instructions on generating:
- Gerber files
- BOM
- Pick-and-place files
- Assembly drawings
