# Hardware Documentation

## Overview

The Capsensor HVA hardware is designed around a high-voltage capacitive sensing principle for measuring wood moisture content. The system uses a custom PCB that integrates all necessary circuitry.

## Hardware Components

### Main Components

1. **Microcontroller**: ESP32-based module
   - Dual-core processor
   - WiFi and Bluetooth connectivity
   - Multiple ADC channels for sensor input
   - GPIO for control and communication

2. **Capacitive Sensing Circuit**
   - High-voltage AC generation
   - Capacitance measurement circuitry
   - Signal conditioning and filtering
   - Safety isolation

3. **Power Management**
   - Input voltage regulation
   - Multiple voltage rails (3.3V, 5V, HV)
   - Power sequencing and protection
   - Low-power modes support

4. **Communication Interfaces**
   - UART for debugging
   - I2C/SPI for expansion
   - WiFi/Bluetooth via ESP32

## PCB Design

### Design Files

All hardware design files are located in the `hardware/` directory:
- KiCAD project files: `hardware/kicad/`
- Manufacturing outputs: `hardware/outputs/`

### PCB Specifications

- **Layers**: TBD (typically 2 or 4 layer)
- **Material**: FR4
- **Thickness**: 1.6mm (standard)
- **Copper Weight**: 1oz (standard), 2oz for high-current traces
- **Surface Finish**: ENIG or HASL

### Key Design Features

- **High Voltage Isolation**: Proper creepage and clearance distances
- **Ground Planes**: Separate analog and digital grounds
- **EMI Protection**: Filtering on sensitive inputs
- **ESD Protection**: TVS diodes on external interfaces

## Assembly

See `hardware/docs/pcba.md` for detailed assembly instructions.

### Manufacturing Process

1. **PCB Fabrication**
   - Send Gerber files to manufacturer
   - Typical lead time: 1-2 weeks

2. **Component Procurement**
   - Use BOM from `hardware/outputs/bom/`
   - Order from major distributors

3. **Assembly**
   - Manual assembly for prototypes
   - Pick-and-place files available for production

4. **Testing**
   - Follow bring-up procedure in `hardware/docs/bringup.md`

## Electrical Characteristics

### Power Requirements

- **Input Voltage**: 5-12V DC
- **Current Consumption**: 
  - Active mode: < 200mA
  - Sleep mode: < 10mA

### Operating Conditions

- **Temperature Range**: 0°C to 50°C (operating)
- **Humidity**: 10% to 90% non-condensing

### Sensor Specifications

- **Measurement Range**: TBD (depends on calibration)
- **Resolution**: TBD
- **Accuracy**: TBD (requires calibration)

## Safety Considerations

⚠️ **High Voltage Warning**: The capacitive sensing circuit operates at high voltages (typically > 100V AC). 

**Safety Measures**:
- Proper isolation barriers
- Warning labels on PCB
- Enclosure required for safe operation
- Follow all safety procedures in the bring-up guide

## Modifications and Customization

### Design Considerations

When modifying the hardware:
- Maintain high-voltage isolation requirements
- Verify power budget for added components
- Update documentation with changes
- Create new revision per `hardware/docs/revisions.md`

### Component Substitutions

- Document any component substitutions
- Verify electrical compatibility
- Test thoroughly before production

## Troubleshooting

### Common Issues

1. **No Power**
   - Check input voltage and polarity
   - Verify power supply current capability
   - Check fuses or protection circuits

2. **Sensor Not Reading**
   - Verify high-voltage circuit operation
   - Check ADC connections
   - Verify firmware configuration

3. **Communication Issues**
   - Check UART baud rate and settings
   - Verify pin connections
   - Test with loopback

## Additional Resources

- [PCB Assembly Guide](../hardware/docs/pcba.md)
- [Bring-Up Procedure](../hardware/docs/bringup.md)
- [Revision History](../hardware/docs/revisions.md)
