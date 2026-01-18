# Build Guide

This guide provides complete instructions for building a Capsensor HVA device from scratch.

## Overview

Building the Capsensor HVA involves three main stages:
1. PCB fabrication and assembly
2. Firmware compilation and programming
3. Testing and calibration

## Prerequisites

### Tools Required

#### Hardware Tools
- Soldering iron and solder
- Multimeter
- Oscilloscope (for advanced testing)
- ESD-safe work area
- Wire cutters and strippers
- Tweezers (for SMD components)
- Magnification (for inspecting solder joints)

#### Software Tools
- PlatformIO (for firmware development)
- KiCAD (for viewing/modifying hardware design)
- Serial terminal program
- Git (for version control)

### Safety Equipment
- Safety glasses
- ESD wrist strap
- Insulated probes (for high-voltage testing)
- Non-conductive work surface

## Stage 1: Hardware

### 1.1 PCB Fabrication

1. **Prepare Manufacturing Files**
   - Gerber files located in `hardware/outputs/gerbers/`
   - Submit to PCB manufacturer (e.g., JLCPCB, PCBWay, OSH Park)
   - Typical turnaround: 1-2 weeks

2. **PCB Specifications for Ordering**
   - Layers: As specified in design
   - Material: FR4
   - Thickness: 1.6mm
   - Copper weight: 1oz (or as specified)
   - Surface finish: ENIG or HASL
   - Silkscreen: Both sides (if applicable)

### 1.2 Component Procurement

1. **Get BOM**
   - Bill of Materials located in `hardware/outputs/bom/`
   - Export as CSV or Excel for ordering

2. **Order Components**
   - Major distributors: Digi-Key, Mouser, Arrow
   - Order 10-20% extra for common passives
   - Verify part availability and lead times

3. **Component Verification**
   - Check all components against BOM
   - Verify package types match PCB footprints
   - Organize components for assembly

### 1.3 PCB Assembly

Follow the detailed assembly instructions in `hardware/docs/pcba.md`.

**Quick Assembly Checklist:**
- [ ] Prepare workspace with proper lighting and ESD protection
- [ ] Start with smallest components (resistors, capacitors)
- [ ] Install ICs and active components
- [ ] Add connectors and mechanical components
- [ ] Visual inspection of all solder joints
- [ ] Clean flux residue if necessary

**Special Considerations:**
- High-voltage sections require extra care
- Maintain proper creepage/clearance distances
- Double-check polarity of electrolytic capacitors and ICs

### 1.4 Initial Testing

Follow the bring-up procedure in `hardware/docs/bringup.md`.

**Safety First:**
⚠️ Do NOT apply high voltage until low-voltage sections are verified!

**Pre-Power Checks:**
- [ ] Visual inspection for solder bridges
- [ ] Continuity test on power rails
- [ ] Check for shorts between power and ground
- [ ] Verify all components are correctly oriented

**Power-Up Sequence:**
1. Set power supply current limit to 100mA
2. Apply 3.3V/5V only
3. Monitor current draw (should be < 50mA)
4. Measure voltages at test points
5. Only proceed to HV if LV section is working

## Stage 2: Firmware

### 2.1 Development Environment Setup

1. **Install PlatformIO**
   ```bash
   # Using Python pip
   pip install platformio
   
   # Or install VS Code extension
   # Search for "PlatformIO IDE" in VS Code extensions
   ```

2. **Clone Repository** (if not already done)
   ```bash
   git clone <repository-url>
   cd capsensor_hva/firmware
   ```

3. **Install Dependencies**
   ```bash
   pio pkg install
   ```

### 2.2 Build Firmware

1. **Configure Build** (if needed)
   - Edit `platformio.ini` for your target board
   - Set appropriate build flags

2. **Build**
   ```bash
   cd firmware/
   pio run
   ```

3. **Verify Build Success**
   - Check for compilation errors
   - Note the firmware size

### 2.3 Upload Firmware

1. **Connect Hardware**
   - Use USB cable to connect ESP32 to computer
   - Install USB drivers if needed (CH340, CP210x, or FTDI)

2. **Upload**
   ```bash
   pio run --target upload
   ```

3. **Troubleshooting Upload**
   - Press BOOT button during upload if it fails
   - Verify correct COM port is selected
   - Check USB cable supports data transfer

### 2.4 Verify Firmware Operation

1. **Open Serial Monitor**
   ```bash
   pio device monitor
   ```
   Or use another serial terminal at 115200 baud.

2. **Check Output**
   - Should see initialization messages
   - Verify firmware version
   - Check for any error messages

## Stage 3: Testing and Calibration

### 3.1 Functional Testing

1. **Communication Test**
   - [ ] Serial communication working
   - [ ] Status messages received
   - [ ] Commands accepted (if applicable)

2. **Sensor Basic Test**
   - [ ] Sensor reads without errors
   - [ ] Values change with test inputs
   - [ ] No excessive noise in readings

3. **Full System Test**
   - [ ] All subsystems operational
   - [ ] Power consumption within spec
   - [ ] No excessive heating

### 3.2 Calibration

1. **Prepare Calibration Standards**
   - Dry wood sample (reference)
   - Known moisture content samples
   - Stable testing environment

2. **Run Calibration Procedure**
   - Measure dry reference (0% moisture)
   - Measure known moisture samples
   - Record capacitance vs. moisture data

3. **Update Calibration Data**
   - Calculate calibration coefficients
   - Update firmware with calibration values
   - Re-upload firmware

4. **Verify Calibration**
   - Test with unknown samples
   - Compare to reference measurements
   - Adjust if necessary

### 3.3 Final Validation

1. **Performance Validation**
   - [ ] Accuracy within specifications
   - [ ] Repeatability of measurements
   - [ ] Stable operation over time

2. **Environmental Testing**
   - [ ] Temperature variations
   - [ ] Humidity effects
   - [ ] Long-term stability

3. **Documentation**
   - [ ] Record serial number (if applicable)
   - [ ] Document calibration values
   - [ ] Note any issues or quirks

## Troubleshooting

### Hardware Issues

**Problem: No Power**
- Check power supply voltage and current
- Verify input polarity
- Test continuity of power traces
- Check for damaged voltage regulator

**Problem: High Current Draw**
- Power off immediately
- Check for solder bridges
- Look for reversed ICs
- Verify no damaged components

**Problem: No Sensor Reading**
- Verify high-voltage circuit is working (use scope)
- Check ADC input connections
- Verify microcontroller is running (LED blinking, serial output)
- Test sensor circuit separately

### Firmware Issues

**Problem: Upload Fails**
- Check USB cable and drivers
- Hold BOOT button during upload
- Verify correct board selection in platformio.ini
- Try different USB port

**Problem: No Serial Output**
- Verify baud rate (115200)
- Check if USB cable supports data
- Ensure correct COM port selected
- Try power cycling the board

**Problem: Sensor Data Incorrect**
- Verify hardware is functioning
- Check calibration values
- Review firmware sensor code
- Test with known reference

### System Integration Issues

**Problem: Unstable Readings**
- Check power supply stability
- Look for EMI/RFI sources
- Add filtering if needed
- Shield sensitive circuits

**Problem: Temperature Drift**
- Implement temperature compensation
- Allow warm-up time
- Consider active temperature control

## Next Steps

After successful build and testing:

1. **Enclosure** (if applicable)
   - Design or select appropriate enclosure
   - Ensure proper ventilation
   - Add access for connectors
   - Include safety warnings

2. **Deployment**
   - Document operating procedures
   - Create user manual
   - Train operators
   - Establish maintenance schedule

3. **Improvements**
   - Document any issues encountered
   - Suggest design improvements
   - Update revision notes
   - Share findings with development team

## Resources

- Hardware documentation: [hardware.md](hardware.md)
- Firmware documentation: [firmware.md](firmware.md)
- PCB assembly guide: [../hardware/docs/pcba.md](../hardware/docs/pcba.md)
- Bring-up procedure: [../hardware/docs/bringup.md](../hardware/docs/bringup.md)

## Support

For issues or questions:
- Check documentation first
- Review troubleshooting section
- Consult project repository issues
- Contact development team

---

**Safety Reminder**: Always follow proper safety procedures when working with high-voltage circuits. If unsure about any step, seek assistance from experienced personnel.
