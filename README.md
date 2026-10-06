# ☕ CoffeeMonitor

**CoffeeMonitor** is a C++/Qt desktop application for monitoring coffee pots using **Home Assistant**.

The application communicates with Home Assistant via its REST API and displays the current status of connected coffee pot sensors in a compact Windows desktop/taskbar-style interface.

## ✨ Features

CoffeeMonitor provides an easy overview of important coffee pot information, including:

- ☕ **Coffee level**
- 🌡️ **Temperature status**
  - ❄️ Cold
  - 🔆 Warm
  - 🔥 Hot
- 🔋 **Sensor battery level**
- Automatic status updates
- Integration with **Home Assistant**
- Communication via the **Home Assistant REST API**
- Compact display designed for everyday use

## 🏠 Home Assistant Integration

Sensor data is retrieved directly from Home Assistant. The required entities are registered in CoffeeMonitor and their states are updated automatically.

Example entities:

```text
sensor.kanne_1_fullstand
sensor.kanne_1_temperatur
sensor.kanne_1_batterie
```

This allows CoffeeMonitor to work with existing Home Assistant sensors without implementing the sensor logic directly in the application.

## 🛠️ Technologies

CoffeeMonitor is built using:

- **C++17**
- **Qt Widgets**
- **Qt Network**
- **Home Assistant REST API**
- **JSON**
- **Windows**

## 🎯 Purpose

The goal of CoffeeMonitor is simple: provide an immediate overview of coffee availability and the current status of monitored coffee pots.

Instead of opening Home Assistant every time, the most important information is displayed directly and unobtrusively on the Windows desktop.

**In short: one glance is enough to know whether there is still coffee available. ☕**
