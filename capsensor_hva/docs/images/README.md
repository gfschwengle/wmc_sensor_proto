# Images

This directory contains images for documentation.

## Purpose

Store images, diagrams, photos, and other visual assets used in the documentation.

## Organization

Organize by topic or document:
```
images/
├── hardware/
│   ├── pcb_photo.jpg
│   ├── schematic_overview.png
│   └── assembly_step1.jpg
├── firmware/
│   ├── architecture_diagram.png
│   └── flowchart.png
├── setup/
│   ├── connection_diagram.png
│   └── calibration_setup.jpg
└── results/
    ├── measurement_graph.png
    └── test_results.png
```

## File Formats

- **Photos**: JPG (for photographs)
- **Diagrams**: PNG (for diagrams with transparency)
- **Vector graphics**: SVG (for scalable diagrams)

## Image Guidelines

- Use descriptive filenames
- Keep file sizes reasonable (compress large images)
- Use consistent dimensions for similar images
- Add alt text in documentation for accessibility

## Referencing in Markdown

```markdown
![Description](images/filename.png)

Or with relative path from docs:
![Description](images/hardware/pcb_photo.jpg)
```

## Tools

Recommended tools for creating images:
- Screenshots: OS built-in tools
- Diagrams: Draw.io, Excalidraw
- Photo editing: GIMP, Photoshop
- Vector graphics: Inkscape, Illustrator
