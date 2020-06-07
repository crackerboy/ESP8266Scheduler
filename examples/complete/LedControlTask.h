#pragma once

#include "arduino.h"
#include <ESP8266WiFi.h>
#include <Task.h>
#include "State.h"


class LedControlTask : public Task {
public:
    LedControlTask() : 
        _state     (nullptr    ), 
        _led       (LED_BUILTIN) 
    {}
	
    void setup(State* state)
    {
        Serial.println(F("LedControlTask - Setup"));
        _state             = state;
        ledSetup();
    }

protected:
    State* _state;
    int _led;
    void ledSetup()
    {
        pinMode(_led, OUTPUT);
        analogWrite(_led, 0);
    }

    void loop()
    {
        // set onlyEssential to true. Other non-essential tasks will watch this
        // and not run anymore        
        _state->onlyEssential = true;
        
        // Perform fade
        fadeInFadeOut();
        
        // set onlyEssential to false. Other will start running agian        
        _state->onlyEssential = false;

        sleep(1928); // approx 2 sec, time modified to encourage varieing relative timing

    }
 
    void fadeInFadeOut()
    {
        // Even though that this is a function that takes a lot of time
        // before exiting the loop, the ESP8266 software watchdog will not
        // trigger because the delay function will reset the watchdog timeout counter
        
        // So even if animations need to happen really quickly, always use the
        // delay of yield function

        // Fade in
        Serial.println(F("LedControlTask - Fade in"));
        for (int brightness = 1; brightness < 20; brightness++)
        {
            pwmLed(brightness);
            sleep(0);
        }

        // fade out
        Serial.println(F("LedControlTask - Fade out"));
        for (int brightness = 20 - 1; brightness > 0; brightness--)
        {
            pwmLed(brightness);
            sleep(0);
        }
    }

    // Very quick & dirty software PWM to set the brightness of the led
    // Because timing is essential, we will use the blocking delay function
    void pwmLed(const int brightness)
    {
        const int off = 20-brightness;
        for (size_t i = 0; i < 5; i++)
        {
            pinMode(_led, HIGH);
            delay(brightness);
            pinMode(_led, LOW);
            delay(off);
        }
    }
};




