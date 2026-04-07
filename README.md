# Smart Posture Corrector System
An end-to-end posture monitoring system that combines embedded fimrware, a FastAPI backend, a posture event simulator, and a real-time web dashboard.

## Overview
This project started as a wearable Arduino posture corrector using an MPU6050 sensor. It was expanded into a full system that simulates real-time posture data, processes it through a backend API, and visualizes results on a dashboard.

## System Architecture
Arduino Firmware/ Python Simulator -> FastAPI Backend -> Browser Dashboard

## Features
- Detects posture changes using IMU sensor data
- Calibrates a baseline posture for personalized detection
- Provides vibration and LED feedback when poor posture is detected
- LED indicates system status
  
## Hardware
- Arduino microcontroller
- MPU6050 IMU sensor
- Coin vibration motor
- RGB LED
- Push button
- Flex sensor (as backup signal)

## How It Works
### Posture Detection
The device uses accelerometer data from an MPU6050 IMU sensor with a personalized baseline calibrated at startup. Two types of poor posture are detected:
- Forward lean — measured via pitch angle change from the X/Z accelerometer axes
- Downward shift — measured via Y-axis acceleration change
- Upper-back bend — detected as a backup signal from the flex sensor

### Feedback
When poor posture is held for a sustained period, the vibration motor and RGB LED activate and remain on until the user returns to their calibrated baseline position.

### Controls
- Short press — toggle the system on/off
- Long press (>3s) — recalibrate baseline posture

Calibrate while sitting straight — the device sets your current position as "good" posture at startup or on recalibration.

## Software
### Backend (FastAPI)
A FastAPI server that takes in posture events, keeps track of them, and returns session stats like total posture events, bad posture count, and slouch percentage.

### Simulator (send_events.py)
Simulates IMU sensor data by generating randomized pitch and Y-axis values, and sends data to the backend once per second so the system can be tested without the actual hardware.

### Dashboard (index.html)
A browser dashboard that polls the backend every 2 seconds and shows current posture status, session stats, slouch rate, and event history.
UI generated with Claude.
