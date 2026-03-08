# Smart Posture Corrector
A wearable psture monitoring prototype built with Arduino and sensors to detect poor posture and provide feedback in a way to correct it.

## Features
- Detects posture changes using IMU sensor data
- Provides vibration and LED feedback when poor posture is detected
- Includes calibration logic for baseline posture
- LED indicates system status
- 
## Hardware
- Arduino
- MPU 6050 IMU sensor
- Flex sensor
- Coin vibration motor
- RGB LED
- Push button

## How It Works
The device monitors posture using accelerometer data from an MPU-6050 IMU sensor. Forward leaning is detected by measuring changes in the IMU picth angle gained from the X/Z accelerometer axes. Additional downward movement is detected using the Y-axis acceleration, while a flex sensor helps identify upper-back bending.
If posture deviates from the baseline for a sustained period, the vibration motor and LED is triggered to alert the user and does not stop until the user is back in the calibrated "good" posture position. 
The system is able to be turned off/on with one button push and re-calibrated with a longer than 3 second button press. When the system is turned on, the user should be sitting in a straight posture because the device sets the current position as the baseline "good" posture.
