# Smart Vending Machine Prototype (MSP430)

## Overview
This repository contains the C++ source code and configuration files for a Smart Vending Machine prototype. The system is built around the Texas Instruments **MSP430F5529** microcontroller. It features a proximity-based wake-up system, precise motor control for product dispensing, and a UART-based interface for both user interaction and administrative tasks.

## Features & System Architecture

The firmware is designed with a dual-role architecture (User and Administrator) and integrates several hardware peripherals:

* **Proximity Wake-up System:** Utilizes an ultrasonic sensor to constantly monitor the area. When a user approaches within a 30 cm threshold, the microcontroller triggers an interrupt/flag to wake up the system and initialize the user interface options.
* **Precision Dispensing:** Uses a stepper motor to ensure exact rotation for dispensing the selected product reliably.
* **Serial Communication Interface (UART):** The system communicates with a host PC via USB. The PC terminal acts as the input console.
* **Dual Operating Modes:**
    * **User Mode:** Allows customers to view the menu and input their desired product selection.
    * **Admin Mode:** A restricted access state that allows the machine owner to perform inventory restocks and generate a cash register cutoff (sales report and revenue audit).

## Hardware Requirements
* **Microcontroller:** TI MSP430F5529 LaunchPad
* **Sensors:** Ultrasonic Distance Sensor (e.g., HC-SR04)
* **Actuators:** Stepper Motor + Driver (e.g., ULN2003 or A4988)
* **Communication:** USB Cable for UART Serial Console

## Software & Tools
* **IDE:** Code Composer Studio (CCS)
* **Language:** C / C++
* **Terminal:** Any serial monitor (Putty, TeraTerm, or Arduino Serial Monitor) configured at the correct Baud Rate (e.g., 9600 bps).

## How to Run the Project
1. Clone this repository to your local machine.
2. Open **Code Composer Studio**.
3. Go to `File > Import > C/C++ > CCS Projects` and select the cloned folder.
4. Build the project (`Project > Build Project`).
5. Flash the firmware onto the MSP430F5529 LaunchPad.
6. Open a Serial Terminal on your PC, connect to the corresponding COM port, and interact with the vending machine interface.
