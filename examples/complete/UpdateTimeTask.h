// BrightnessTask.h

#pragma once

#include "arduino.h"
#include <TimeLib.h>
#include <Task.h>
#include "NTPClient.h"
#include "State.h"


class UpdateTimeTask : public Task {
public:
    void setup(State* state)
    {
        Serial.println(F("UpdateTimeTask - Setup"));
        _state = state;        
        updateTime();
    }

    UpdateTimeTask() : Task(), _state(nullptr), ntpClient(ntpUDP) {};

protected:
    State* _state;
    WiFiUDP ntpUDP;
    NTPClient ntpClient;    

    bool shouldRun() {
        bool run = Task::shouldRun();

        // If only essentialTasks should run, UpdateTimeTask will tell the scheduler it should not run
        if (_state->onlyEssential) { return false; }
        return run;
    }


    void loop() 
    {   
        updateTime();
     
        // Update time to UTC time every approximate 10 seconds
        sleep(10147);
    }

    void updateTime() {
        if (WiFi.isConnected()) {
            Serial.println(F("UpdateTimeTask - update time from NTP"));
            time_t utc = ntpClient.updateTime();
            setTime(utc);
        }
        else {
            Serial.println(F("UpdateTimeTask - no Wi-Fi, delay time update from NTP"));
        }
    }

} ;


