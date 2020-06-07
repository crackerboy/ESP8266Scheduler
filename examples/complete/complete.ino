/// This application is re-implements the functionality of ES8266/NTPClient and ... using tasks



#include <FS.h>						// Filesystem wrapper. This needs to be first.
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <BlynkSimpleEsp8266.h>
#include <Scheduler.h>
#include <Task.h>
#include "State.h"
#include "WiFiTask.h"
#include "UpdateTimeTask.h"
#include "LedControlTask.h"

State             state;
WiFiTask          wifiTask;
UpdateTimeTask    updateTimeTask;
LedControlTask    ledControlTask;

void setup() {
    delay( 3000 ); // power-up safety delay
    // Application setup
    Serial.begin(115200);
    
    Serial.println(F("Application    - Setup"));

    // Set up tasks
    wifiTask      .setup(&state);
    updateTimeTask.setup(&state);
    ledControlTask.setup(&state);

     // put your setup code here, to run once:
    Scheduler.start(&wifiTask);
    Scheduler.start(&updateTimeTask);
    Scheduler.start(&ledControlTask);

    Serial.println("");
    Serial.println(F("Application    - Begin"));
    Scheduler.begin();
}

void loop() {
  // Do nothing here, work is called by scheduler
    Serial.println(F("Main Loop Should not be called"));
}

