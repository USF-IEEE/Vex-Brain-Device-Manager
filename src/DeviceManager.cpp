#include "../include/TerriBull/DeviceManager.hpp"

bool DeviceManager::serializeMessage(const FunctionCall* message, uint8_t* buffer, size_t buffer_size, size_t* message_length) {
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, buffer_size);
    if (!pb_encode(&stream, FunctionCall_fields, message)) {
        // Handle encode error
        return false;
    }
    *message_length = stream.bytes_written;
    return true;
}

void DeviceManager::add_device(DeviceHeader* device) {
    // Assumes device->port is accessible and is the unique identifier for the device.
    devices[device->port] = device;
}

DeviceHeader* DeviceManager::get_device(uint8_t port) {
    auto it = devices.find(port);
    if (it != devices.end()) {
        return it->second;
    }
    return nullptr; // Return nullptr if device not found
}


bool TerriBull::DeviceManager::processMessage(const uint8_t* buffer, size_t size) {
    FunctionCall message = FunctionCall_init_zero; // Initialize to defaults
    pb_istream_t stream = pb_istream_from_buffer(buffer, size);
    if (!pb_decode(&stream, FunctionCall_fields, &message)) {
        std::cerr << "Decoding failed: " << PB_GET_ERROR(&stream) << std::endl;
        return false;
    }

    // Process the message, e.g., call appropriate methods based on message content
    // Example:
    if (message.has_motorInitializeData) {
        // motorInitializeCallback(message.motorInitializeData);
    } else if (message.has_motorSetVelocityData) {
        // motorSetVelocityCallback(message.motorSetVelocityData);
    }
    // Add more conditions for other types of messages

    return true;
}

DeviceManager::~DeviceManager() {
    for (auto& pair : devices) {
        delete pair.second; // Assuming dynamic allocation of DeviceHeader*
    }
    devices.clear();
}
