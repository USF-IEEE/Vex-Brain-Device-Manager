#ifndef __TERRIBULL_DEVICES__
#define __TERRIBULL_DEVICES__

#include "../TerriBull.hpp"

class DeviceManager;

/**
 * E_DEVICE_NONE = 0,
 * E_DEVICE_MOTOR = 2,
 * E_DEVICE_ROTATION = 4,
 * E_DEVICE_IMU = 6,
 * E_DEVICE_DISTANCE = 7,
 * E_DEVICE_RADIO = 8,
 * E_DEVICE_VISION = 11,
 * E_DEVICE_ADI = 12,
 * E_DEVICE_OPTICAL = 16,
 * E_DEVICE_GPS = 20,
 * E_DEVICE_SERIAL = 129,
 */
typedef struct {
    pros::c::v5_device_e_t pros_device_type;
    uint8_t port;
} DeviceHeader;

/**
 * E_MOTOR_GEARSET_36 = 0,  // 36:1, 100 RPM, Red gear set
 * E_MOTOR_GEAR_RED = E_MOTOR_GEARSET_36,
 * E_MOTOR_GEAR_100 = E_MOTOR_GEARSET_36,
 * E_MOTOR_GEARSET_18 = 1,  // 18:1, 200 RPM, Green gear set
 * E_MOTOR_GEAR_GREEN = E_MOTOR_GEARSET_18,
 * E_MOTOR_GEAR_200 = E_MOTOR_GEARSET_18,
 * E_MOTOR_GEARSET_06 = 2,  // 6:1, 600 RPM, Blue gear set
 * E_MOTOR_GEAR_BLUE  = E_MOTOR_GEARSET_06,
 * E_MOTOR_GEAR_600 = E_MOTOR_GEARSET_06,
 * E_MOTOR_GEARSET_INVALID = INT32_MAX
 */

typedef enum device_callback_return_code {
    FUNC_NOT_FOUND = 0,
    SUCCESS = 1,
    DEVICE_TYPE_MISMATCH = 2,
    DEVICE_NOT_EXIST = 3
};

typedef struct {
    DeviceHeader header; // First member is the DeviceHeader
    float velocity;
    float current_draw;
    pros::motor_gearset_e_t gear_set;
    pros::motor_brake_mode_e_t break_mode;
    // Other motor-specific members
} MotorDevice;


typedef struct {
    uint8_t port; int GEAR_SET; int BREAK_MODE;
} motor_initialize_callback_data;


/**
 * @brief Should set motors velocity to the proto data
 * 
 * @param port 
 * @param data contains {uint8_t port, int GEAR_SET, int BREAK_MODE}
 * @return device_callback_return_code 
 * This velocity corresponds to different actual speeds depending on the gearset used for the motor. This results in a range of +-100 for E_MOTOR_GEARSET_36, +-200 for E_MOTOR_GEARSET_18, and +-600 for blue. The velocity is held with PID to ensure consistent speed, as opposed to setting the motor’s voltage.
 */
typedef struct  {
    uint8_t port; 
    int16_t velocity;
} motor_set_velocity_callback_data;



typedef struct {
    DeviceHeader header; 
    float roll;
    float yaw;
    float pitch;
    pros::c::imu_accel_s_t accel;
} IMUDevice;

typedef struct {
    DeviceHeader header;
    float value;
} AnalogDevice;


#include "../DeviceManager.hpp"

// Callbacks and Function Definitions

/**
 * @brief Should Initialize motor device with Port and proto data
 * 
 * @param data contains {uint8_t port, int GEAR_SET, int BREAK_MODE}
 * @return device_callback_return_code 
 */
device_callback_return_code motor_device_initalize(DeviceManager* _motherSys, motor_initialize_callback_data data);
/**
 * @brief Should set motors velocity to the proto data
 * 
 * @param port 
 * @param data contains {uint8_t port, int GEAR_SET, int BREAK_MODE}
 * @return device_callback_return_code 
 * This velocity corresponds to different actual speeds depending on the gearset used for the motor. This results in a range of +-100 for E_MOTOR_GEARSET_36, +-200 for E_MOTOR_GEARSET_18, and +-600 for blue. The velocity is held with PID to ensure consistent speed, as opposed to setting the motor’s voltage.
 */
device_callback_return_code motor_device_set_velocity(DeviceManager* _motherSys, motor_set_velocity_callback_data data);


#endif