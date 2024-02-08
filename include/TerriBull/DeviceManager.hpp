#ifndef __DEVICEMANAGER__
#define __DEVICEMANAGER__

#include "TerriBull.hpp"
#include "./Devices/device.hpp"
#include <map>

class TerriBull::DeviceManager { 
    private:
    std::map<uint8_t, DeviceHeader*> devices;
    public:

    DeviceManager() {}
    ~DeviceManager() {} // TODO: delete all devices
    /**
     * @brief Adds a device to the manager
     * 
     * @param device DeviceHeader* to keep track of on a port 
     */
    void add_device(DeviceHeader* device); // get port from device (device->port)
    DeviceHeader* get_device(uint8_t port);
};


#endif