#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Task.h>
#include "State.h"

#ifndef STASSID
#define STASSID "Thuis"
#define STAPSK  "aquariumvoor4vissen"
#endif

const char* ssid     = STASSID; // your network SSID (name)
const char* password = STAPSK;  // your network password

class WiFiTask : public Task {
public:
    WiFiTask() : _state(nullptr), _connected(false) {}

    void setup(State* state)
    {
        Serial.println(F("WiFiTask       - Setup"));
        _state = state;
        connectSetup();
    }

protected:
    State* _state;
    bool _connected;
    void connectSetup()
    {
        WiFi.setAutoReconnect(true);
    }

    void loop()
    {
        connect();        
    }

    void connect()
    {
        // if wifi is nu running, just try to keep reconnecting
        if (!WiFi.isConnected()) {
            // If was connected, give a message that the system is not connected anymore
            Serial.println(F("WiFiTask       - Wi-Fi is disconnected. (re)connecting"));
            //if (_connected) { Serial.println(F("WiFi is disconnected. (re)connecting")); _connected = false; }
            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid, password);
        } else {
            // If was not connected, give a message that the system is now connected 
            if (!_connected) { 
                Serial.print("WiFiTask       - Wi-Fi connected, IP: ");
                Serial.println(WiFi.localIP());
                _connected = true; 
            }
            else {
                Serial.println("WiFiTask       - Wi-Fi still connected");
            }
        }
        // *** Because this is a Task, this delay function will call the Scheduler and switch to a next task        
        sleep(5134); // Using strange delay to show ...
    }

    bool shouldRun() {
        bool run = Task::shouldRun();

        // If only essentialTasks should run, UpdateTimeTask will tell the scheduler it should not run
        if (_state->onlyEssential) { return false; }
        return run;
    }

};




