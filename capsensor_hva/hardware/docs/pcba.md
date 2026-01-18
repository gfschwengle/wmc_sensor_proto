# PCB Assembly Guide

## Overview

This document provides instructions for assembling the Capsensor HVA PCB.

## Bill of Materials (BOM)

See the BOM file in `outputs/bom/` for a complete list of components.

## Assembly Steps

1. **Component Placement**
   - Start with low-profile components (resistors, capacitors)
   - Install ICs and sockets
   - Add connectors and high-profile components last

2. **Soldering**
   - Use appropriate temperature for lead-free solder (if applicable)
   - Inspect all joints for cold solder or bridges
   - Clean flux residue if using flux-core solder

3. **Inspection**
   - Visual inspection of all solder joints
   - Continuity testing of power rails
   - Check for shorts between adjacent pins

## Special Notes

- Handle high-voltage sections with care
- Ensure proper isolation between HV and LV sections
- Follow ESD precautions when handling sensitive components

## Testing

See `bringup.md` for initial testing and bring-up procedures.
