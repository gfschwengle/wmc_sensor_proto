# Include Directory

This directory contains header files for the firmware project.

## Purpose

Place `.h` header files here that declare:
- Function prototypes
- Class definitions
- Constant definitions
- Type definitions
- Macro definitions

## Usage

Files in this directory are automatically included in the build path by PlatformIO.

## Example Structure

```
include/
├── config.h          # Configuration constants
├── sensor.h          # Sensor interface
├── calibration.h     # Calibration routines
└── communication.h   # Communication protocols
```

## Best Practices

- Use include guards or `#pragma once`
- Keep headers focused and modular
- Document public APIs
- Avoid including implementation in headers (unless templates)
