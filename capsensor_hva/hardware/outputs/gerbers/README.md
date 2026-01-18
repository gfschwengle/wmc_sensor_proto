# Gerber Files

This directory contains Gerber files for PCB manufacturing.

## Contents

Gerber files include:
- Copper layers (*.Cu)
- Soldermask layers (*.Mask)
- Silkscreen layers (*.SilkS)
- Edge cuts (*.Edge_Cuts)
- Drill files (*.drl)
- Map files

## Generating from KiCAD

1. Open PCB in KiCAD PCB Editor
2. File → Plot
3. Select Gerber format
4. Choose layers to export
5. Plot
6. Generate drill files

## Standard Gerber Layers

- `*-F.Cu.gbr` - Front copper
- `*-B.Cu.gbr` - Back copper
- `*-F.Mask.gbr` - Front soldermask
- `*-B.Mask.gbr` - Back soldermask
- `*-F.SilkS.gbr` - Front silkscreen
- `*-B.SilkS.gbr` - Back silkscreen
- `*-Edge.Cuts.gbr` - Board outline
- `*.drl` - Drill file

## Validation

Before sending to manufacturer:
- Use Gerber viewer to verify
- Check board dimensions
- Verify hole sizes
- Confirm layer alignment

## Manufacturing

Compress all Gerber files into a ZIP archive and upload to PCB manufacturer's website.
