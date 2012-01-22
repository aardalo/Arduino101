Simple impulse counter for house power meters
Based on LDR.
Calculations set ut to handle 10000/imp kWh meter.

Calibration code runs for first 10 seconds - as LDRs and resistors are different.
Calibration formula is: Determine min and max readout. Threshold is set in the middle.

Design:
1. LDR to 5V, other end to A0 and 470kOhm resistor to GND - voltage divider setup

Known problems:
1. If no impulse occurs during calibration cycle, everything breaks
2. Some misfires - don't know why yet

Improvements:
1. Get a more suitable LDR
2. Optimize range by manipulating AREF

v0.1	22.01.2012	Øyvind Aardal	Initial version
