
/* Tilt-compesnated compass
*  for Nano 33 BLE
*/

// Calibration offsets for Nano33BLE IMU sensors
// These offsets must be empirically measured
// for each Nano33BLE. Offsets are derived by 
// fitting a sphere to raw measured data points.
// (search for "spherical fit python") for examples.
// 
// X, Y, Z offsets correspond to the fitted spherical center
// MAG is a multiplier that scales the fitted sphere to radius 1.0
// 
// It is assumed that each set of axes has a simliar gain profile.

#define OFFSET_MAG_X 29.363654
#define OFFSET_MAG_Y 7.947678
#define OFFSET_MAG_Z 84.274158
#define R_MAG 0.027106

#define OFFSET_ACC_X 0.025065
#define OFFSET_ACC_Y 0.012438
#define OFFSET_ACC_Z 0.018101
#define R_ACC 0.997446

// Note: Gyroscope offsets are not used for compass
// #define OFFSET_GYR_X 0.00
// #define OFFSET_GYR_Y 0.00
// #define OFFSET_GYR_Z 0.00

// HDG_OFFSET is the local magnetic declination
// see: https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml#declination
#define HDG_OFFSET (8.0+9.0/60.0)

// Low-pass filters are applied to raw sensor
// readings (accelerometer and magnetometer), and
// to the computed heading components mw_x and mn_x.

// low-pass filter parameters
// IMU sample rate is nomianlly 104Hz, but was measured 
// to be 116.0 Hz. Magnetometer sample rate was measured as
// 20.0 Hz. These definitions are not critical to obtain
// precicesly, but will affect the overall performance
// of the respective low-pass filters.
#define IMU_SAMPLE_RATE 116.0 //Hz
#define MAG_SAMPLE_RATE 20.0  //Hz

// Nano33BLE sensors will yield an absolute heading
// accuracy of about +-1.5 deg, even with a very low 
// passband (<0.1Hz). A passband greater than 3.5Hz 
// will result in a fairly noisy heading result.
// Adjust the passband based on application needs.
#define PASSBAND 2.0  //Hz

// div_remainder() returns the remainder of x/y
#define div_remainder(x, y) (x - (y * floor(x / y)))

// fast_cross_product() computes the cross product of 
// vectors a and b, storing the result in vector c.
template <typename T>
inline void fast_cross_product(const T&ax, const T&ay, const T&az,
                        const T&bx, const T&by, const T&bz,
                        T*cx, T*cy, T*cz) {
                            *cx = (ay * bz) - (az * by);
                            *cy = (az * bx) - (ax * bz); 
                            *cz = (ax * by) - (ay * bx);
                        }
