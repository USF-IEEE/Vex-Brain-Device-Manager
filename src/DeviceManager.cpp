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

void DeviceManager::add_device(TerriBullDevices_DeviceHeader* device) {
    devices[device->port] = device;
}

TerriBullDevices_DeviceHeader* DeviceManager::get_device(uint32_t port) {
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
    // Example: if (message.which_callbackData == TerriBullDevices_FunctionCall_motorInitializeData_tag) {...}

    return true;
}