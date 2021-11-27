/*
  Arduino Nano 33 BLE Tilt Compensated Compass

  This example computes a tilt-compensated heading based 
  on accelerometer and magnetometer values from the 
  Nano 33 BLE IMU.

  The circuit:
  - Arduino Nano 33 BLE Sense

  created 2021.11.27
  by Nicholas Folse

  Algorithm overview:
    Read ACC(X,Y,Z) and MAG(X,Y,Z) from the IMU.
    Apply low-pass filters to raw ACC and MAG readings
    Compute MagWest (mw) as cross_product(ACC, MAG);
    Compute MagNorth (mn) as cross_product(mw, ACC);
    --> Heading components are mw_x and mn_x
    Apply low-pass filter to mw_x and mn_x
    Return heading as atan2(mw_x, mn_x)

  Based on examples in Mahony AHRS 3D Fusion Filter.

  Serial output format:
  Any of the following may be generated at any time. Data is generated
  upon measurment.
    acc,<float:x>,<float:y>,<float:z>\n
    mag,<float:x>,<float:y>,<float:z>\n
    gyr,<float:x>,<float:y>,<float:z>\n
    hdg,<float:0.0 - 360.0deg>,<float:mw_x>,<float:mn_x>\n

  This example code is in the public domain.
*/

#include <LSM9DS1.h>
#include <butterworth_iir_2nd.h>
#include "compass-support.h"

// global variables
float ax = 0, ay = 0, az = 0;
float mx = 0, my = 0, mz = 0;
float gx = 0, gy = 0, gz = 0;
float mw_x = 0, mw_y = 0, mw_z = 0;
float mn_x = 0, mn_y = 0, mn_z = 0;

// create a low-pass filter for the output heading
Butterworth2ndOrderIIRFilterParams<float> imu_filter_params(PASSBAND, IMU_SAMPLE_RATE);
Butterworth2ndOrderIIRFilterParams<float> mag_filter_params(PASSBAND, MAG_SAMPLE_RATE);
IIR_2nd_order_temporal_filter<float> ax_filter(imu_filter_params, ax),
    ay_filter(imu_filter_params, ay),
    az_filter(imu_filter_params, az),
    mx_filter(mag_filter_params, mx),
    my_filter(mag_filter_params, my),
    mz_filter(mag_filter_params, mz),
    mw_x_filter(imu_filter_params, mw_x),
    mn_x_filter(imu_filter_params, mn_x);

void setup()
{
  Serial.begin(500000);
  // Pause until serial connection has been established...
  // while (!Serial)
  //   ;
  // Serial.println("Started");

  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }
  Serial.print("Magnetic field sample rate = ");
  Serial.print(IMU.magneticFieldSampleRate());
}

template <typename T>
void print_measurement(const char *type, const T &x, const T &y, const T &z)
{
  Serial.print(type);
  Serial.print("\t");
  Serial.print(x);
  Serial.print('\t');
  Serial.print(y);
  Serial.print('\t');
  Serial.println(z);
}

void loop()
{
  if (IMU.accelerationAvailable())
  {
    IMU.readAcceleration(ax, ay, az);
    ax_filter << (ax + OFFSET_ACC_X) * R_ACC;
    ay_filter << (ay + OFFSET_ACC_Y) * R_ACC;
    az_filter << (az + OFFSET_ACC_Z) * R_ACC;
    print_measurement("acc", (float)ax_filter, (float)ay_filter, (float)az_filter);

    if (IMU.magneticFieldAvailable())
    {
      IMU.readMagneticField(mx, my, mz);
      mx = -mx; // mx is reversed in Nano33BLE
      mx_filter << (mx + OFFSET_MAG_X) * R_MAG;
      my_filter << (my + OFFSET_MAG_Y) * R_MAG;
      mz_filter << (mz + OFFSET_MAG_Z) * R_MAG;
      print_measurement("mag", (float)mx_filter, (float)my_filter, (float)mz_filter);
    }

    fast_cross_product((float)ax_filter, (float)ay_filter, (float)az_filter,
                       (float)mx_filter, (float)my_filter, (float)mz_filter,
                       &mw_x, &mw_y, &mw_z);
    fast_cross_product(mw_x, mw_y, mw_z,
                       (float)ax_filter, (float)ay_filter, (float)az_filter,
                       &mn_x, &mn_y, &mn_z);

    mw_x_filter << mw_x;
    mn_x_filter << mn_x;

    float heading = div_remainder((atan2((float)mw_x_filter, (float)mn_x_filter) * (180.0 / PI) + HDG_OFFSET), 360.0);
    print_measurement("hdg", heading, (float)mw_x_filter, (float)mn_x_filter);

    delay(1);
  }
  else
  {
    delay(1);
  }
}
