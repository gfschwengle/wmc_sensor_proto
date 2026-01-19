# Pick and Place Files

This directory contains pick-and-place files for automated PCB assembly.

## Contents

Pick-and-place (PnP) files specify the position and orientation of each component for automated assembly machines.

## Format

Common formats:
- CSV (most common)
- Text file
- Manufacturer-specific formats

## Data Fields

Typical columns:
- Reference designator
- X position (mm)
- Y position (mm)
- Rotation (degrees)
- Side (Top/Bottom)
- Part value
- Package

## Generating from KiCAD

1. Open PCB in KiCAD PCB Editor
2. File → Fabrication Outputs → Component Placement
3. Configure format and units
4. Generate for top and bottom sides

## Coordinate System

- Origin typically at board corner or center
- Units: millimeters (mm)
- Rotation: degrees (0-360)

## Usage

1. Export PnP file from KiCAD
2. Review and verify positions
3. Submit to assembly house with Gerbers and BOM
4. Assembly house may require format conversion

## Validation

Before sending:
- Check all components are listed
- Verify coordinate origin
- Confirm rotation angles
- Test with assembly house if possible

## Notes

- Different assembly houses may require different formats
- Some components may need manual adjustments
- Update when PCB design changes
