# ESP32 LED Brightness Control with OTA Firmware Updates

This project implements an LED brightness control system on an ESP32 microcontroller using Pulse Width Modulation (PWM) and integrates Over-the-Air (OTA) firmware updates. The LED brightness can be adjusted through a web interface, with real-time updates via WebSocket communication.

## Features

- **PWM-based LED Brightness Control**: Adjust the brightness of the LED using PWM on the ESP32.
- **Web Interface with Slider**: Control LED brightness through a user-friendly webpage with a slider.
- **Real-Time WebSocket Communication**: Send and receive brightness updates instantly between the web interface and ESP32.
- **Over-the-Air (OTA) Updates**: Update the firmware remotely without needing a physical connection to the device.

## Components Used

- **ESP32** microcontroller
- **LED** for brightness control (connected to a PWM-capable pin)

## Getting Started

### Prerequisites

- **Arduino IDE**: Ensure that you have installed the [Arduino IDE](https://www.arduino.cc/en/software).
- **ESP32 Board Manager**: Install the ESP32 board in the Arduino IDE. You can find instructions [here](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/).
- **Libraries**:
  - `WiFi.h`
  - `ESPAsyncWebServer.h`
  - `ArduinoOTA.h`
  - `AsyncTCP.h`
  - `ESPAsyncWebSocket.h`

### Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/your-username/ESP32-LED-Brightness-OTA-Control.git
