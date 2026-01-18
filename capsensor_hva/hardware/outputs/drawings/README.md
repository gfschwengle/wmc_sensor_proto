# Assembly Drawings

This directory contains assembly drawings and documentation for PCB assembly.

## Contents

Assembly drawings typically include:

- **Assembly diagram**: Component placement diagram with reference designators
- **Top view**: Showing components on top side
- **Bottom view**: Showing components on bottom side (if any)
- **3D renders**: Visual representation of assembled board
- **Notes**: Special assembly instructions

## Generating from KiCAD

### 2D Assembly Drawing

1. Open PCB in KiCAD
2. File → Plot
3. Select PDF or SVG format
4. Choose F.Fab and B.Fab layers
5. Include reference designators

### 3D View

1. View → 3D Viewer in KiCAD
2. File → Export → PNG or JPEG
3. Capture top and perspective views

## Drawing Elements

Include on assembly drawings:
- Reference designators for all components
- Board outline and dimensions
- Mounting holes
- Connector pinouts
- Orientation markers
- Polarity indicators
- Version/revision number

## Assembly Notes

Document special requirements:
- Components requiring heat sinking
- Specific soldering temperatures
- Assembly order requirements
- Cleaning procedures
- Conformal coating areas

## Using the Drawings

- Provide to assembly technicians
- Include in assembly documentation package
- Use for quality inspection
- Reference during troubleshooting

## Files to Include

- `assembly_top.pdf` - Top side assembly
- `assembly_bottom.pdf` - Bottom side assembly
- `3d_view_top.png` - 3D rendering top view
- `3d_view_perspective.png` - 3D rendering perspective
- `assembly_notes.txt` - Special instructions
