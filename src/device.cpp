#include "../include/TerriBull/Devices/device.hpp"


device_callback_return_code motor_device_initialize(DeviceManager* _motherSys, motor_initialize_callback_data data) {
    // Dynamically allocate a new MotorDevice instance
    MotorDevice* device = new MotorDevice();
    device->header.pros_device_type = pros::c::v5_device_e_t::E_DEVICE_MOTOR;
    device->header.port = data.port;
    device->gear_set = static_cast<pros::motor_gearset_e_t>(data.GEAR_SET);
    device->break_mode = static_cast<pros::motor_brake_mode_e_t>(data.BREAK_MODE);

    // Initialize the motor using PROS library functions
    pros::c::motor_tare_position(device->header.port);
    pros::c::motor_set_brake_mode(device->header.port, device->break_mode);
    pros::c::motor_set_gearing(device->header.port, device->gear_set);

    // Retrieve and store the actual velocity
    device->velocity = pros::c::motor_get_actual_velocity(device->header.port);

    // Add the device to the DeviceManager
    _motherSys->add_device(&device->header); // Adjust this to your actual method for adding a device

    return SUCCESS;
}

device_callback_return_code motor_device_set_velocity(DeviceManager* _motherSys, motor_set_velocity_callback_data data) {
    // Retrieve the device using its port
    DeviceHeader* deviceHeader = _motherSys->get_device(data.port);
    if (deviceHeader == nullptr) return DEVICE_NOT_EXIST;
    if (deviceHeader->pros_device_type != pros::c::v5_device_e_t::E_DEVICE_MOTOR) return DEVICE_TYPE_MISMATCH;

    // Cast the DeviceHeader back to MotorDevice to access motor-specific fields
    MotorDevice* motorDevice = (MotorDevice*)(void*)deviceHeader;
    
    // Set the motor's velocity
    pros::c::motor_move_velocity(motorDevice->header.port, data.velocity);

    return SUCCESS;
}