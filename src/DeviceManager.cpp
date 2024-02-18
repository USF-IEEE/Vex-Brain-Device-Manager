#include "../include/TerriBull/DeviceManager.hpp"

// bool DeviceManager::serializeMessage(const TerriBullDevices_FunctionCall* message, uint8_t* buffer, size_t buffer_size, size_t* message_length) {
//     pb_ostream_t stream = pb_ostream_from_buffer(buffer, buffer_size);
//     if (!pb_encode(&stream, TerriBullDevices_FunctionCall_fields, message)) {
//         // Log or handle encode error
//         std::cerr << "Encoding failed: " << PB_GET_ERROR(&stream) << std::endl;
//         return false;
//     }
//     *message_length = stream.bytes_written;
//     return true;
// }

void DeviceManager::add_device(DeviceHeader* device) {
    devices[device->port] = device;
}

void DeviceManager::update_devices(float delta) {
    for (auto it = this->devices.begin(); it!= this->devices.end(); ++it) {
        int d_port = it->first;
        DeviceHeader* d_header = it->second;
        switch(d_header->pros_device_type) {
            case pros::c::v5_device_e_t::E_DEVICE_NONE: {
                break;
            }
            case pros::c::v5_device_e_t::E_DEVICE_MOTOR: {
                MotorDevice* motorDevice = (MotorDevice*)(void*)d_header;
                motor_device_update_internal(motorDevice);
                break;
            }
            case pros::c::v5_device_e_t::E_DEVICE_ROTATION: {
                RotationSensorDevice* rotDevice = (RotationSensorDevice*)(void*)d_header;
                rotation_sensor_update_internal(rotDevice);
                break;
            }
            case pros::c::v5_device_e_t::E_DEVICE_IMU: {
                break;
            }
            case pros::c::v5_device_e_t::E_DEVICE_DISTANCE: {
                break;
            }
            case pros::c::v5_device_e_t::E_DEVICE_RADIO: {
                break;
            }
            case pros::c::v5_device_e_t::E_DEVICE_VISION: {
                break;
            }
            case pros::c::v5_device_e_t::E_DEVICE_ADI: {
                break;
            }
            case pros::c::v5_device_e_t::E_DEVICE_OPTICAL: {
                break;
            }
            case pros::c::v5_device_e_t::E_DEVICE_GPS: {
                break;
            }
            case pros::c::v5_device_e_t::E_DEVICE_SERIAL: {
                break;
            }
            case pros::c::v5_device_e_t::E_DEVICE_CONTROLLER: {
                break;
            }
        }
    }
}

DeviceHeader* DeviceManager::get_device(uint32_t port) {
    auto it = devices.find(port);
    if (it != devices.end()) {
        return it->second;
    }
    return nullptr; // Return nullptr if device not found
}

bool DeviceManager::processMessage(const uint8_t* buffer, size_t size) {
    TerriBullDevices_FunctionCall message = TerriBullDevices_FunctionCall_init_zero; // Initialize to defaults
    pb_istream_t stream = pb_istream_from_buffer(buffer, size);
    if (!pb_decode(&stream, TerriBullDevices_FunctionCall_fields, &message)) {
        std::cerr << "Decoding failed: " << PB_GET_ERROR(&stream) << std::endl;
        return false;
    }

    // Process the message based on its type
    device_callback_return_code r_code = FUNC_NOT_FOUND;
    TerriBullDevices_ReturnData r_data = TerriBullDevices_ReturnData_init_zero;

    switch (message.which_callbackData) {
        case TerriBullDevices_FunctionCall_motorInitializeData_tag: {
            motor_initialize_callback_data data;
            data.port = (uint8_t) message.callbackData.motorInitializeData.port;
            data.GEAR_SET = message.callbackData.motorInitializeData.GEAR_SET;
            data.BREAK_MODE = message.callbackData.motorInitializeData.BREAK_MODE;
            r_code = motor_device_initalize(this, data, r_data);
            break;
        }
        
        case TerriBullDevices_FunctionCall_motorSetVelocityData_tag: {
            motor_set_velocity_callback_data data;
            data.port = (uint8_t) message.callbackData.motorSetVelocityData.port;
            data.velocity = message.callbackData.motorSetVelocityData.velocity;
            r_code = motor_device_set_velocity(this, data, r_data);
            break;
        }

        case TerriBullDevices_FunctionCall_rotationSensorInitializeData_tag: {
            rotation_sensor_initialize_callback_data data;
            data.port = (uint8_t) message.callbackData.rotationSensorInitializeData.port;
            r_code = rotation_sensor_initialize(this, data, r_data);
            break;
        }

        case TerriBullDevices_FunctionCall_rotationSensorSetPositionData_tag: {
            rotation_sensor_set_position_callback_data data;
            data.port = (uint8_t) message.callbackData.rotationSensorSetPositionData.port;
            data.position = message.callbackData.rotationSensorSetPositionData.position;
            r_code = rotation_sensor_set_position(this, data, r_data);
            break;
        }

        case TerriBullDevices_FunctionCall_rotationSensorResetPositionData_tag: {
            rotation_sensor_reset_position_callback_data data;
            data.port = message.callbackData.rotationSensorResetPositionData.port;
            r_code = rotation_sensor_reset_position(this, data, r_data);
            break;
        }
        
        // case TerriBullDevices_FunctionCall_...

        default:
            break;
    }

    // TODO: CREATE A RETURN MESSAGE BASED ON EACH FUNCTION CALL


    if (! this->is_buffer_ready() )this->clear_buffer();
    pb_ostream_t stream_out = pb_ostream_from_buffer(this->buffer, sizeof(this->buffer));

    if (!pb_encode(&stream_out, TerriBullDevices_ReturnData_fields, &r_data)) {
        return false;
    }

    return true;
}