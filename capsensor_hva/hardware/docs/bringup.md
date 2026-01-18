# Hardware Bring-Up Guide

## Overview

This document describes the process for bringing up and testing a newly assembled Capsensor HVA board.

## Safety Precautions

⚠️ **WARNING**: This device operates at high voltages. Take appropriate safety measures:
- Use isolated power supplies
- Wear appropriate PPE
- Work on non-conductive surfaces
- Keep one hand in pocket when probing live circuits

## Initial Inspection

1. **Visual Inspection**
   - Verify all components are correctly placed
   - Check solder joints for quality
   - Look for solder bridges or cold joints

2. **Continuity Tests**
   - Check power rail continuity
   - Verify ground connections
   - Test for shorts between power and ground

## Power-Up Sequence

1. **Low Voltage Power (3.3V/5V)**
   - Connect power supply (do not turn on yet)
   - Set current limit to 100mA
   - Turn on power and monitor current draw
   - Expected idle current: < 50mA
   - Measure voltage at key test points

2. **High Voltage Section**
   - Only proceed if low voltage section is working
   - Start with reduced HV (if possible)
   - Monitor current and voltage
   - Increase to operating voltage gradually

## Functional Testing

1. **Communication Test**
   - Connect to serial/debug interface
   - Verify firmware communication
   - Check status messages

2. **Sensor Testing**
   - Apply known test loads
   - Verify sensor readings
   - Calibrate if necessary

## Troubleshooting

### No Power
- Check power supply connections
- Verify polarity
- Check for shorts

### High Current Draw
- Power off immediately
- Inspect for component damage
- Check for solder bridges

### No Communication
- Verify UART/communication settings
- Check signal levels with scope
- Verify firmware is loaded

## Sign-Off

After successful bring-up, document:
- Date and operator
- Test results
- Any issues encountered
- Board serial number (if applicable)
