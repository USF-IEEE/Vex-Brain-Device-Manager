#ifndef __TERRIBULL_DEVICES__
#define __TERRIBULL_DEVICES__

#include "../TerriBull.hpp"
// namespace TerriBull {

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
typedef struct  {
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
    SUCCESS = 0,
    DEVICE_TYPE_MISMATCH = 1,
    DEVICE_NOT_EXIST = 2,
};

typedef struct {
    DeviceHeader header; // First member is the DeviceHeader
    float velocity;
    float current_draw;
    pros::motor_gearset_e_t gear_set;
    pros::motor_brake_mode_e_t break_mode;
    // Other motor-specific members
} MotorDevice;

/**
 * @brief Should Initialize device with Port and proto data
 * 
 * @param port 
 * @param data contains {uint8_t port, int GEAR_SET, int BREAK_MODE}
 * @return device_callback_return_code 
 */
typedef struct {
    uint8_t port; int GEAR_SET; int BREAK_MODE;
} motor_initialize_callback_data;

device_callback_return_code motor_device_initalize(TerriBull::DeviceManager* _motherSys, motor_initialize_callback_data data) {
    MotorDevice* device = (MotorDevice*) malloc(sizeof(MotorDevice));
    device->header.pros_device_type = pros::c::v5_device_e_t::E_DEVICE_MOTOR;
    device->header.port = data.port;
    device->gear_set = (pros::motor_gearset_e_t) data.GEAR_SET;
    device->break_mode = (pros::motor_brake_mode_e_t) data.BREAK_MODE;
    pros::c::motor_tare_position(device->header.port);
    pros::c::motor_set_brake_mode(device->header.port, device->break_mode);
    pros::c::motor_set_gearing(device->header.port, device->gear_set);
    device->velocity = pros::c::motor_get_actual_velocity(device->header.port);
    _motherSys->add_device(device); // pointer to device
    return device_callback_return_code::SUCCESS;
}

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

device_callback_return_code motor_device_set_velocity(TerriBull::DeviceManager* _motherSys, motor_set_velocity_callback_data data) {
    DeviceHeader* device = _motherSys->get_device(data.port);
    if (device != nullptr) return device_callback_return_code::DEVICE_NOT_EXIST;
    if( ! device->pros_device_type == pros::c::v5_device_e_t::E_DEVICE_MOTOR) return device_callback_return_code::DEVICE_TYPE_MISMATCH;

    MotorDevice* motor = static_cast<MotorDevice*>((void*)device);
    pros::c::motor_move_velocity(motor->header.port, data.velocity);
    return device_callback_return_code::SUCCESS;
}


typedef struct {
    DeviceHeader header; // First member is the DeviceHeader
    float roll;
    float yaw;
    float pitch;
    pros::c::imu_accel_s_t accel;
} IMUDevice;

typedef struct {
    DeviceHeader header; // First member is the DeviceHeader
    float value;
} AnalogDevice;

#endif