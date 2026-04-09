
# 🚗 CAN-Based Automotive Dashboard

A multi-node embedded system project using CAN (Controller Area Network) to simulate an automotive dashboard with real-time data communication between multiple ECUs.

In branches you will find 3 ECUS.
-
You have to complile 3 ECUS is sepatetely and you have to dump the codes in different Boards.

That 3 Boards must be connected to CAN Wires to communicate with each other.

🔹 ECU1 – RPM & Indicator Unit
-
Reads RPM using ADC (potentiometer)
Reads indicator input using digital keypad
Transmits:
RPM data
Indicator status (OFF / LEFT / RIGHT / HAZARD)

🔹 ECU2 – Speed & Gear Unit
-

Reads Speed using ADC
Reads Gear input using keypad
Transmits:
Speed data
Gear position (N, 1–5, R)

🔹 ECU3 – Dashboard Display Unit
-
Receives data from ECU1 and ECU2 via CAN
Displays on CLCD:
RPM
Speed
Gear
Indicator status
Controls LEDs for indicator visualization
