
# Bot Soccer Project: Attacker & Defender Multi-Controller Guide

This guide provides the complete plans, component lists, and wiring diagrams for building a full, two-bot soccer team: the 2-motor **Attacker** and the 4-motor **Defender**. It covers three distinct control methods for each bot.

### **Design Philosophy**

  * **Attacker:** Built for speed and agility using 2 high-RPM Johnson motors.
  * **Defender:** Built for torque and pushing power using 4 high-torque geared motors.

### **SAFETY FIRST\! ⚠️**

  * **LiPo Batteries:** These are powerful. Always use a proper LiPo balance charger. Never use a swollen or damaged battery. Disconnect the battery before working on your bot.
  * **High Current:** The motors draw a lot of power. Wires can get hot. Double-check all wiring before connecting a battery.
  * **Test Safely:** When you first power on a bot, ensure its wheels are off the ground.

-----

-----

## **Version 1: The Direct-Wired Controller (Pure Hardware)**

This version uses no code, only heavy-duty switches to control the motors via a wired tether.

### **Attacker Bot (2-Motor)**

#### Component Sheet

| Component | Quantity | Key Specifications / Notes |
| :--- | :--- | :--- |
| **Motors** | 2 | Johnson DC Motor |
| **Control Switches**| 2 | **DPDT Toggle Switch (Center-Off)**, Rated for **15 Amps or higher**. |
| **Battery** | 1 | 3S (11.1V) LiPo Battery |
| **Wire** | \~5 meters | Heavy Gauge (14AWG). |
| **Chassis & Wheels**| 1 set | Small 2WD chassis, 2 wheels, 1 caster wheel. |

#### Wiring & Assembly

Your control box will have two of these circuits, one for each motor.

**Diagram: DPDT Switch Wiring for One Motor**

```
              DPDT SWITCH
           +-----------------+
           | (1)       (4) o----(Motor +)
           |  |         |  |
           |  o---------X--o         <-- Wires cross here
           |  |         |  |
(Battery +)----o (2)       (5) o----(Battery -)
           |  |         |  |
           |  o---------X--o
           |  |         |  |
(Motor -)----o (3)       (6)
           +-----------------+
```

-----

### **Defender Bot (4-Motor)**

#### Component Sheet

| Component | Quantity | Key Specifications / Notes |
| :--- | :--- | :--- |
| **Motors** | 4 | High-Torque Square Geared DC Motor |
| **Control Switches**| **4** | **DPDT Toggle Switch (Center-Off)**, Rated for **10 Amps or higher**. |
| **Battery** | 1 | 3S (11.1V) LiPo Battery |
| **Wire** | \~5 meters | Heavy Gauge (16AWG). |
| **Chassis & Wheels**| 1 set | Wide, heavy 4WD chassis, 4 wheels. |

#### Wiring & Assembly

The control box will be larger, containing four separate DPDT switches. Each switch is wired to a single motor exactly like the diagram for the Attacker.

**Diagram: Conceptual Layout of Defender Control Box**

```
+------------------------------------+
|                                    |
|  [Switch 1]        [Switch 3]      |
| (Front-Left)      (Front-Right)    |
|                                    |
|  [Switch 2]        [Switch 4]      |
|  (Rear-Left)       (Rear-Right)    |
|                                    |
+------------------------------------+
       | | | |------- To 4 Motors
```

-----

-----

## **Version 2: The ESP-NOW Custom Controller (DIY Wireless & Coding)**

This version uses two ESP32 boards for a custom-built, wireless remote control system. **This requires coding.**

### **Transmitter (Controller Build)**

The transmitter build is the **same** for both bots.

#### Wiring Diagram: Joystick to Transmitter ESP32

```
+-----------------+         +-----------------+
| Joystick Module |         |   ESP32 Board   |
|-----------------|         |-----------------|
|       VCC       | ------> |       3.3V      |
|       GND       | ------> |       GND       |
|       VRx       | ------> |     GPIO 34     | (X-axis)
|       VRy       | ------> |     GPIO 35     | (Y-axis)
+-----------------+         +-----------------+
```

-----

### **Attacker Bot (2-Motor Receiver)**

#### Component Sheet

| Component | Quantity | Key Specifications / Notes |
| :--- | :--- | :--- |
| ESP32 Board | 1 | ESP32 DEVKIT V1 |
| **Motor Driver** | **1** | **BTS7960** Dual Channel Motor Driver |
| Motors | 2 | Johnson DC Motor |
| Battery | 1 | 3S (11.1V) LiPo Battery |
| Chassis & Wheels| 1 set | Small 2WD chassis, 2 wheels, 1 caster wheel. |

#### Wiring Diagram: Receiver ESP32 to BTS7960 Driver

```
+---------------+      +---------------------------------+      +-------------+
|               |      |         BTS7960 Driver          |      |             |
| ESP32 Board   |      |---------------------------------|      | Left Motor  |
|               |      | L_PWM <---------------- GPIO 25 |      |-------------|
|               |------> R_PWM ----------------- GPIO 26 |----->|             |
|               |      |                                 |      | Right Motor |
|               |      | L_PWM <---------------- GPIO 27 |      |-------------|
|               |------> R_PWM ----------------- GPIO 32 |----->|             |
|               |      |                                 |      +-------------+
|               |      | B+ / B- (To 3S LiPo Battery)    |
|               |      | VCC / GND (To ESP32 5V/GND)     |
+---------------+      +---------------------------------+
```

-----

### **Defender Bot (4-Motor Receiver)**

#### Component Sheet

| Component | Quantity | Key Specifications / Notes |
| :--- | :--- | :--- |
| ESP32 Board | 1 | ESP32 DEVKIT V1 |
| **Motor Drivers** | **2** | **BTS7960** Dual Channel Motor Driver Modules |
| Motors | 4 | High-Torque Square Geared DC Motor |
| Battery | 1 | 3S (11.1V) LiPo Battery |
| Chassis & Wheels| 1 set | Wide, heavy 4WD chassis, 4 wheels. |

#### Wiring Diagram: Receiver ESP32 to Two BTS7960 Drivers

```
                     +-----------------+      +------------------------+
(ESP32 GPIOs for L)--->| BTS7960 Driver #1 |----->| Two Left Motors      |
                     |                 |      | (Wired in Parallel)    |
                     +-----------------+      +------------------------+
+-------------+
|             |
| ESP32 Board |
|             |
+-------------+      +-----------------+      +------------------------+
                     | BTS7960 Driver #2 |----->| Two Right Motors     |
(ESP32 GPIOs for R)--->|                 |      | (Wired in Parallel)    |
                     +-----------------+      +------------------------+

(Both Drivers connected to 3S LiPo Battery in Parallel)
```

-----

-----

## **Version 3: The Flysky RC Controller (Reliable Hobby Grade)**

This version uses off-the-shelf RC hardware for reliable, long-range control without coding.

### **Transmitter (Controller)**

The **Flysky Transmitter** and its internal **Channel Mixing** setup are the **same** for controlling both bots.

-----

### **Attacker Bot (2-Motor)**

#### Component Sheet

| Component | Quantity | Key Specifications / Notes |
| :--- | :--- | :--- |
| **RC System** | 1 | Flysky Receiver (e.g., FS-iA6B) |
| **Motor Driver** | **1** | **BTS7960** Dual Channel Motor Driver |
| Motors | 2 | Johnson DC Motor |
| Battery | 1 | 3S (11.1V) LiPo Battery |
| Chassis & Wheels| 1 set | Small 2WD chassis, 2 wheels, 1 caster wheel. |

#### Wiring Diagram: RC Receiver to BTS7960

This diagram shows the concept. You'll need to use jumper wires or a custom connector to adapt the 3-pin servo plugs to the driver's input pins.

```
+-----------------+        +------------------+
| Flysky Receiver |        |  BTS7960 Driver  |
|-----------------|        |------------------|
|     CH 1 Signal |------> | L-Motor PWM Pins |
|     CH 2 Signal |------> | R-Motor PWM Pins |
|     5V & GND    |------> | Driver VCC & GND |
+-----------------+        +------------------+
```

-----

### **Defender Bot (4-Motor)**

#### Component Sheet

| Component | Quantity | Key Specifications / Notes |
| :--- | :--- | :--- |
| **RC System** | 1 | Flysky Receiver (e.g., FS-iA6B) |
| **Motor Drivers** | **2** | **BTS7960** Dual Channel Motor Driver Modules |
| **Y-Splitters** | 2 | Servo "Y" Lead/Splitter Cables |
| Motors | 4 | High-Torque Square Geared DC Motor |
| Battery | 1 | 3S (11.1V) LiPo Battery |
| Chassis & Wheels| 1 set | Wide, heavy 4WD chassis, 4 wheels. |

#### Wiring Diagram: RC Receiver to Two Drivers via Y-Splitters

```
                         +-----> [Input Pins on BTS7960 #1]
(Flysky Rx CH 1)----Y---->|
                         +-----> [Input Pins on BTS7960 #2]


                         +-----> [Input Pins on BTS7960 #1]
(Flysky Rx CH 2)----Y---->|
                         +-----> [Input Pins on BTS7960 #2]

(Both Drivers connected to Motors and Battery as per previous diagrams)
```
