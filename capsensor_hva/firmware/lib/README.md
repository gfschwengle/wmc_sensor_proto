# Library Directory

This directory is for project-specific libraries.

## Purpose

Place custom libraries and modules here that are specific to this project.

## Structure

Each library should be in its own subdirectory:

```
lib/
├── MySensor/
│   ├── MySensor.h
│   ├── MySensor.cpp
│   └── library.json
└── MyComm/
    ├── MyComm.h
    ├── MyComm.cpp
    └── library.json
```

## Library Dependencies

For external libraries, add them to `platformio.ini` under `lib_deps`:

```ini
lib_deps = 
    adafruit/Adafruit Sensor@^1.0.0
    paulstoffregen/Time@^1.6.0
```

## Documentation

- Each library should have its own README
- Document the API and usage examples
- Include version information
