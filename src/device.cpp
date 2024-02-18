#include "../include/TerriBull/Devices/device.hpp"

//
//
// Motors:
//
//

device_callback_return_code motor_device_initialize(DeviceManager* _motherSys, motor_initialize_callback_data data, TerriBullDevices_ReturnData& r_data) {
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

    motor_device_update_internal(device);

    // Add the device to the DeviceManager
    _motherSys->add_device(&device->header); // Adjust this to your actual method for adding a device

    // Update Return Data:
    r_data.returnValue.deviceValue.which_device = TerriBullDevices_DeviceValue_motorDevice_tag;
    r_data.returnValue.deviceValue.device.motorDevice.header.pros_device_type = TerriBullDevices_DeviceType_DEVICE_MOTOR;
    r_data.returnValue.deviceValue.device.motorDevice.header.port = (uint32_t) device->header.port;
    r_data.returnValue.deviceValue.device.motorDevice.velocity = device->velocity;
    
    r_data.return_code = TerriBullDevices_DeviceCallbackReturnCode_SUCCESS;


    return SUCCESS;
}

device_callback_return_code motor_device_set_velocity(DeviceManager* _motherSys, motor_set_velocity_callback_data data, TerriBullDevices_ReturnData& r_data) {
    // Type Checking:
    // Retrieve the device using its port
    DeviceHeader* device_header = _motherSys->get_device(data.port);
    if (device_header == nullptr) return DEVICE_NOT_EXIST;
    if (device_header->pros_device_type != pros::c::v5_device_e_t::E_DEVICE_MOTOR) return DEVICE_TYPE_MISMATCH;

    // Cast the DeviceHeader back to MotorDevice to access motor-specific fields
    MotorDevice* motor_device = (MotorDevice*)(void*)device_header;
    
    // Set the motor's velocity
    pros::c::motor_move_velocity(motor_device->header.port, data.velocity);

    // Update Devices Internal State:
    motor_device_update_internal(motor_device);

    // Update Return Data:
    r_data.returnValue.deviceValue.which_device = TerriBullDevices_DeviceValue_motorDevice_tag;
    r_data.returnValue.deviceValue.device.motorDevice.header.pros_device_type = TerriBullDevices_DeviceType_DEVICE_MOTOR;
    r_data.returnValue.deviceValue.device.motorDevice.header.port = (uint32_t) motor_device->header.port;
    r_data.returnValue.deviceValue.device.motorDevice.velocity = motor_device->velocity;
    
    r_data.return_code = TerriBullDevices_DeviceCallbackReturnCode_SUCCESS;

    return SUCCESS;
}


void motor_device_update_internal(MotorDevice* motorDevice) {
    // Retrieve and store the actual velocity
    motorDevice->velocity = pros::c::motor_get_actual_velocity(motorDevice->header.port);
    //TODO: Add more data fields
}
//
//
// Rotational Sensors:
//
//

device_callback_return_code rotation_sensor_initialize(DeviceManager* _motherSys, rotation_sensor_initialize_callback_data data, TerriBullDevices_ReturnData& r_data) {
    RotationSensorDevice* device = (RotationSensorDevice*) malloc(sizeof(RotationSensorDevice));
    device->header.pros_device_type = pros::c::v5_device_e_t::E_DEVICE_ROTATION;
    device->header.port = data.port;
    pros::c::rotation_reset(device->header.port);
    rotation_sensor_update_internal(device);
    _motherSys->add_device(&device->header);
    
    
    // Update Return Data:
    r_data.returnValue.deviceValue.which_device = TerriBullDevices_DeviceValue_rotationSensorDevice_tag;
    r_data.returnValue.deviceValue.device.rotationSensorDevice.header.pros_device_type = TerriBullDevices_DeviceType_DEVICE_ROTATION;
    r_data.returnValue.deviceValue.device.rotationSensorDevice.header.port = (uint32_t) device->header.port;
    r_data.returnValue.deviceValue.device.rotationSensorDevice.position = device->position;
    
    r_data.return_code = TerriBullDevices_DeviceCallbackReturnCode_SUCCESS;

    return device_callback_return_code::SUCCESS;
}

device_callback_return_code rotation_sensor_set_position(DeviceManager* _motherSys, rotation_sensor_set_position_callback_data data, TerriBullDevices_ReturnData& r_data) {
    // Type Checking:
    DeviceHeader* device = _motherSys->get_device(data.port);
    if (device != nullptr) return device_callback_return_code::DEVICE_NOT_EXIST;
    if( ! device->pros_device_type == pros::c::v5_device_e_t::E_DEVICE_ROTATION) return device_callback_return_code::DEVICE_TYPE_MISMATCH;

    RotationSensorDevice* rotation_sensor = static_cast<RotationSensorDevice*>((void*)device);
    pros::c::rotation_set_position(rotation_sensor->header.port, data.position);

    // Update Values in Device:
    rotation_sensor_update_internal(rotation_sensor);


    // Update Return Data:
    r_data.returnValue.deviceValue.which_device = TerriBullDevices_DeviceValue_rotationSensorDevice_tag;
    r_data.returnValue.deviceValue.device.rotationSensorDevice.header.pros_device_type = TerriBullDevices_DeviceType_DEVICE_ROTATION;
    r_data.returnValue.deviceValue.device.rotationSensorDevice.header.port = (uint32_t) rotation_sensor->header.port;
    r_data.returnValue.deviceValue.device.rotationSensorDevice.position = rotation_sensor->position;
    
    r_data.return_code = TerriBullDevices_DeviceCallbackReturnCode_SUCCESS;

    return device_callback_return_code::SUCCESS;
}

device_callback_return_code rotation_sensor_reset_position(DeviceManager* _motherSys, rotation_sensor_reset_position_callback_data data, TerriBullDevices_ReturnData& r_data) {
    // Type Checking:
    DeviceHeader* device = _motherSys->get_device(data.port);
    if (device != nullptr) return device_callback_return_code::DEVICE_NOT_EXIST;
    if( ! device->pros_device_type == pros::c::v5_device_e_t::E_DEVICE_ROTATION) return device_callback_return_code::DEVICE_TYPE_MISMATCH;

    RotationSensorDevice* rotation_sensor = static_cast<RotationSensorDevice*>((void*)device);
    pros::c::rotation_reset(rotation_sensor->header.port);

    // Update Values in Device:
    rotation_sensor_update_internal(rotation_sensor);

    // Update Return Data:
    r_data.returnValue.deviceValue.which_device = TerriBullDevices_DeviceValue_rotationSensorDevice_tag;
    r_data.returnValue.deviceValue.device.rotationSensorDevice.header.pros_device_type = TerriBullDevices_DeviceType_DEVICE_ROTATION;
    r_data.returnValue.deviceValue.device.rotationSensorDevice.header.port = (uint32_t) rotation_sensor->header.port;
    r_data.returnValue.deviceValue.device.rotationSensorDevice.position = rotation_sensor->position;
    
    r_data.return_code = TerriBullDevices_DeviceCallbackReturnCode_SUCCESS;

    return device_callback_return_code::SUCCESS;
}

void rotation_sensor_update_internal(RotationSensorDevice* rotDevice) {
    rotDevice->position = pros::c::rotation_get_position(rotDevice->header.port);
}