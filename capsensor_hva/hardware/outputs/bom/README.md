# Bill of Materials (BOM)

This directory contains the bill of materials for the PCB.

## Contents

BOM files typically include:
- Component reference designators
- Component values
- Part numbers
- Manufacturer names
- Descriptions
- Quantities
- Package types

## Format

Common formats:
- CSV (Comma-separated values)
- Excel (.xlsx)
- HTML

## Generating from KiCAD

1. Open schematic in KiCAD
2. Tools → Generate BOM
3. Choose BOM plugin/script
4. Export in desired format

## BOM Structure

Typical columns:
```
Ref | Value | Footprint | MPN | Manufacturer | Description | Quantity
```

## Using the BOM

1. Import into distributor's BOM tool (Digi-Key, Mouser, etc.)
2. Check stock availability
3. Find alternative parts if needed
4. Order with extra quantities for common parts (10-20%)

## Notes

- Keep BOM updated with design changes
- Document any approved alternates
- Note any specific component requirements
- Include assembly notes if needed
