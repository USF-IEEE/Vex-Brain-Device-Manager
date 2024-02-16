#ifndef __DEVICEMANAGER__
#define __DEVICEMANAGER__

#include "TerriBull.hpp"
#include <map>

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

    bool serializeMessage(const TerriBullDevices_FunctionCall* message, uint8_t* buffer, size_t buffer_size, size_t* message_length);
    void add_device(DeviceHeader* device);
    DeviceHeader* get_device(uint32_t port);
    bool processMessage(const uint8_t* buffer, size_t size);

    private:
    std::map<uint32_t, DeviceHeader*> devices;

};


#endif