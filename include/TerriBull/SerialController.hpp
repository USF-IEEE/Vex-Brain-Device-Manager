/**
 * @file SerialController.hpp
 * @author John Koch jkoch21@usf.edu, Bill Gate <Email Redacted>
 * @brief Manages Serial communication between the V5 and other devices
 *     
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H

#include <string>
#include "./DeviceManager.hpp"
#include "pros/apix.h" //added this in
#include <vector>
#include <iostream>
#include <string>
#include <mutex>
#include <cstring>

using namespace std;

class SerialController {
    public:
    typedef void (*PacketCallback) (DeviceManager* robot, char * array, int start_index, int length);

    typedef struct {
        PacketCallback callback;
        std::string friendly_name;
        int jetson_id;
    } CallbackItem;

    typedef struct {
        CallbackItem* callbackItem;
        float sumTime;
        float frequency;
    } ScheduledUpdate;

    private:
    
    static constexpr char __end_of_transmission[4] = { (char)0, (char)0, (char)10, (char)10 };
    vector<ScheduledUpdate*> ScheduledUpdates;
    DeviceManager* motherSys;

    void copyStringToBuffer(const std::string& input, uint8_t* buffer, size_t buffer_size);

    public:

    static std::string input_buffer;
    static pros::Mutex input_mutex;

    static void read_input_task(void* ignore)
    {
        char c;
        while(true) 
        {
            std::cin.get(c);
            std::unique_lock<pros::Mutex> lock(SerialController::input_mutex);
            SerialController::input_buffer += c;
            lock.unlock();
        }
    }
    SerialController(DeviceManager* _motherSys);
    
    void ScheduleUpdate(std::string tag_name, float frequency);
    void Update(float delta);
    bool ReadBuffer();
    void SendData(::std::string data);
};


#endif