#ifndef __DEVICEMANAGER__
#define __DEVICEMANAGER__

#include "TerriBull.hpp"
#include "./Devices/device.hpp"
#include <map>

#ifdef __cplusplus

extern "C" {
#include "../protos/vex.pb.h" // Adjust the path as necessary
}

#endif

#include <iostream>
#include <memory>
#include <string>

class DeviceManager { 
    private:
    std::map<uint8_t, DeviceHeader*> devices;
    public:

    DeviceManager() {}
    ~DeviceManager(); // TODO: delete all devices
    /**
     * @brief Adds a device to the manager
     * 
     * @param device DeviceHeader* to keep track of on a port 
     */
    void add_device(DeviceHeader* device); // get port from device (device->port)
    DeviceHeader* get_device(uint8_t port);
    bool processMessage(const uint8_t* buffer, size_t size);
    bool serializeMessage(const FunctionCall* message, uint8_t* buffer, size_t buffer_size, size_t* message_length);


};


#endif