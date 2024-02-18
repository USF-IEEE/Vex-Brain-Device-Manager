#ifndef __DEVICEMANAGER__
#define __DEVICEMANAGER__

#include "TerriBull.hpp"
#include <map>
#define DEVICE_MANAGER_INTERNAL_BUFFER_SIZE 1024

#ifdef __cplusplus

extern "C" {
    #include "../protos/vex.pb.h" // Adjust the path as necessary
}

#include "Devices/device.hpp"

#endif

#include <iostream>
#include <memory>
#include <string>

class DeviceManager { 
    public:

    DeviceManager() = default;
    ~DeviceManager(); // Implement cleanup in the CPP file

    // bool serializeMessage(const TerriBullDevices_FunctionCall* message, uint8_t* buffer, size_t buffer_size, size_t* message_length);
    void add_device(DeviceHeader* device);
    void update_devices(float delta);
    DeviceHeader* get_device(uint32_t port);
    bool processMessage(const uint8_t* buffer, size_t size);
    bool is_buffer_ready() const { return this->buffer_ready; }
    uint8_t* get_buffer() { return this->buffer; }
    private:
    std::map<uint32_t, DeviceHeader*> devices;
    uint8_t buffer[DEVICE_MANAGER_INTERNAL_BUFFER_SIZE];
    bool buffer_ready;
    void clear_buffer() {
        for (int i = 0; i < DEVICE_MANAGER_INTERNAL_BUFFER_SIZE; i++) this->buffer[i] = 0;
        this->buffer_ready = true;
    }
};


#endif