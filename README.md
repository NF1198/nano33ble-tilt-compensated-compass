# Tilt-compensated Compass for Arduino Nano 33 BLE Sense

This project demonstrates using the [Arduino Nano 33 BLE Sense](https://docs.arduino.cc/hardware/nano-33-ble-sense)
as a tilt-compensated compass.

## Features
* Computes and reports heading in degrees
* Compensates for pitch or roll error
* Overridable offset for [magnetic declination correction](https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml#declination)
* [Low-pass filter](lib/butterworth_iir_2nd/butterworth_iir_2nd.h) to improve data quality
* Generates solutions at 120Hz

![Nano33BLE-Compass](doc/images/nano33ble-compass.png)

# Algorithm Overview
```
* Read ACC(X,Y,Z) and MAG(X,Y,Z) from the IMU.
* Apply low-pass filters to raw ACC and MAG readings
* Compute MagWest (mw) as cross_product(ACC, MAG);
* Compute MagNorth (mn) as cross_product(mw, ACC);
--> Heading components are mw_x and mn_x
* Apply low-pass filter to mw_x and mn_x
--> Return heading as atan2(mw_x, mn_x)
```
Based on concepts in Mahony AHRS 3D Fusion Filter.

# Hardware
* [Arduino Nano 33 BLE Sense](https://docs.arduino.cc/hardware/nano-33-ble-sense)

# Pre-requisites
* [Visual Studio Code](https://code.visualstudio.com/download)
* [Platform.io](https://platformio.org/) (Install from Visual Studio Code)
* [Python](https://www.python.org/downloads/) (Platform.io requires Python)

Note: This project is not intended to be used from the Arudino IDE.

![Nano33BLE-Compass-GUI](doc/images/nano33ble-compass-ui.png)

# Build instructions
1. Download and install Visual Studio Code and Python.
2. Install the Platform.io plugin in Visual Studio Code.
3. Open the project folder in Visual Studio Code. The project should be recognized as a Platform.io project.
4. Choose "Build", then "Upload" from the Platform.io toolset.

# Calibrating your Nano 33 BLE IMU
Each [LSM9DS1](https://www.arduino.cc/en/Reference/ArduinoLSM9DS1) sensor is unique and requires a unique
calibration. You will need to generate a calibration for your device and update the calibration
constants defined in [compass-support.h](src/compass-support.h).

The algorithm implmented in this project requires accelerometer and magnetometer readings to be nomalized
to a sphere of radius 1. The can be accomplished using the included calibration tool in the support project, or 
by using any other method of [fitting a sphere](https://jekel.me/2015/Least-Squares-Sphere-Fit/).

![Nano33BLE-Calibration](doc/images/nano33ble-calibration-ui.png)