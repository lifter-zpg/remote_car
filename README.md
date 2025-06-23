# Remote‑Controlled Mini Car

## 1· Project Overview

This repository documents the Remote‑Controlled Mini Car – a palm‑sized rover driven by two DC gearmotors and controlled over a 2.4 GHz link built with nRF24L01+ modules.  The system consists of:

Transmitter (stick) – Arduino Uno with an XY‑joystick that streams the stick‑positions every 100 ms.

Receiver (car) – Arduino Nano that decodes packets, drives a dual H‑bridge and optional LEDs/buzzer.

The goal was to design an inexpensive, reproducible platform that can climb a 45 ° ramp while weighing under 200 g.

## 2· Features

Bidirectional 2.4 GHz link

nRF24L01+ @ 250 kb s⁻¹, custom packet (4 bytes) without auto‑ack.

Proportional throttle & steering
Maps joystick X/Y to motor PWM (0‑255) with dead‑zone and acceleration ramp.

Low‑voltage cut‑off
Protects Li‑Ion pack; flashes LED and disables motors below 6.4 V.

Modular code
Separate sketches /transmitter and /receiver; no external RTOS.

Diagnostics
Serial.print() telemetry and radio.printDetails() for both nodes.

## 3· Hardware Bill of Materials

3.1 Transmitter

1 × Arduino Uno R3 (or clone)

1 × XY joystick (10 kΩ)

1 × nRF24L01+ module + 10 µF capacitor

1 × PP3 9 V battery + step‑down to 5 V (or USB power‑bank)

Assorted wires

3.2 Receiver / Rover

1 × Arduino Nano (ATmega328P)

2 × TT DC gearmotors 1 : 12 with 65 mm wheels

1 × Dual H‑bridge (TB6612FNG or L298N)

1 × nRF24L01+ module + AMS1117‑3.3 V regulator + 10 µF cap

2 × 14500 Li‑Ion cells in series (≈ 7.4 V) or 2S Li‑Po 450 mAh

## 4· Future Work

Auto‑ack and packet counter for lost‑frame detection.

Gyro‑‑based stability and closed‑loop speed control.

Swap TT motors for metal‑gear 37 mm to boost torque.

Web‑based dashboard via ESP‑01 as transparent Serial‑bridge.
