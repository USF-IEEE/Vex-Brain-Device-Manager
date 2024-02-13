/**
 * @file SerialController.cpp
 * @author John Koch jkoch21@usf.edu, Bill Gate <Email Redacted>
 * @brief Manages Serial communication between the V5 and other devices
 *
 * @version 0.1
 * @date 2023-03-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "../../include/TerriBull/SerialController.hpp"

std::string SerialController::input_buffer;
pros::Mutex SerialController::input_mutex;

SerialController::SerialController(DeviceManager *_motherSys) : motherSys(_motherSys), tagExchange(false), isCollectingTags(false)
{
    ::pros::c::serctl(SERCTL_DISABLE_COBS, nullptr);
    pros::Task input_task(SerialController::read_input_task);
}

void SerialController::ScheduleUpdate(std::string tag_name, float frequency) 
{
    CallbackItem* update = this->FindInternal(tag_name);
    if (update != nullptr) {
        ScheduledUpdate* scheduledUpdate = new ScheduledUpdate();
        scheduledUpdate->callbackItem = update;
        scheduledUpdate->frequency = frequency;
        this->ScheduledUpdates.push_back(scheduledUpdate);
    }
    else {
        throw new exception;
    }
}

void SerialController::Update(float delta)
{
    this->ReadBuffer();
    for (auto it = this->ScheduledUpdates.begin(); it != this->ScheduledUpdates.end(); ++it)
    {
        ScheduledUpdate *update = *it;
        update->sumTime += delta;
        if (update->sumTime >= update->frequency)
        {
            update->callbackItem->callback(this->motherSys, nullptr, 0, 0);
            update->sumTime = 0;
        }
    }
}

void SerialController::copyStringToBuffer(const std::string& input, uint8_t* buffer, size_t buffer_size) {
    if (buffer_size < input.size()) {
        std::cerr << "Buffer too small to hold string." << std::endl;
        return;
    }

    // Copy string to buffer
    std::copy(input.begin(), input.end(), buffer);

    // Optional: Null-terminate the buffer, if you want it to be a C-style string
    // Make sure your buffer is large enough to include the null terminator
    buffer[input.size()] = '\0';
}

bool SerialController::ReadBuffer()
{
    std::string input;
    std::unique_lock<pros::Mutex> lock(SerialController::input_mutex);
    if (!SerialController::input_buffer.empty())
    {
        input = SerialController::input_buffer;
        SerialController::input_buffer.clear();
    }
    lock.unlock();

    if (input.size() == 0)
        return false;

    size_t buffer_size = input.size() + 1;
    uint8_t buffer[buffer_size];

    copyStringToBuffer(input, buffer, buffer_size);
    this->motherSys->processMessage(buffer, buffer_size); 

    return true;
}

void SerialController::SendData(std::string data)
{
    for (auto c : SerialController::__packet_header)
    {
        std::cout << c;
    }
    std::cout << data;
    for (auto c : SerialController::__end_of_transmission)
    {
        std::cout << c;
    }

    // std::cout << (char)0 << (char)0 << (char)10 << (char)10;
    // std::cout << SerialController::__end_of_transmission;
}