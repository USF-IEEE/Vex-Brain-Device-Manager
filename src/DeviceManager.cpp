#include "../include/TerriBull/DeviceManager.hpp"

bool DeviceManager::serializeMessage(const TerriBullDevices_FunctionCall* message, uint8_t* buffer, size_t buffer_size, size_t* message_length) {
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, buffer_size);
    if (!pb_encode(&stream, TerriBullDevices_FunctionCall_fields, message)) {
        // Log or handle encode error
        std::cerr << "Encoding failed: " << PB_GET_ERROR(&stream) << std::endl;
        return false;
    }
    *message_length = stream.bytes_written;
    return true;
}

void DeviceManager::add_device(DeviceHeader* device) {
    devices[device->port] = device;
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
    switch (message.which_callbackData) {
        case TerriBullDevices_FunctionCall_motorInitializeData_tag: {
            motor_initialize_callback_data data;
            data.port = message.callbackData.motorInitializeData.port;
            data.GEAR_SET = message.callbackData.motorInitializeData.GEAR_SET;
            data.BREAK_MODE = message.callbackData.motorInitializeData.BREAK_MODE;
            r_code = motor_device_initalize(this, data);
            break;
        }
            
        
        case TerriBullDevices_FunctionCall_motorSetVelocityData_tag: {
            motor_set_velocity_callback_data data;
            data.port = message.callbackData.motorSetVelocityData.port;
            data.velocity = message.callbackData.motorSetVelocityData.velocity;
            r_code = motor_device_set_velocity(this, data);
            break;
        }
        
        // case TerriBullDevices_FunctionCall_...

        default:
            break;
    }

    // TODO: CREATE A RETURN MESSAGE BASED ON EACH FUNCTION CALL

    return true;
}