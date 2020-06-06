#include <Arduino.h>
#include <Scheduler.h>

class PrintTask : public Task {
protected:
    void loop()  {
        Serial.println("Print Loop Start");

        delay(5000);

        Serial.println("Print Loop End");

        delay(5000);
    }
} print_task;

class BlinkTask : public Task {
protected:
    void setup() {
        state = HIGH;

        pinMode(2, OUTPUT);
        pinMode(2, state);
    }

    void loop() {
        state = state == HIGH ? LOW : HIGH;
        pinMode(2, state);

        delay(1000);
    }

private:
    uint8_t state;
} blink_task;

class MemLibrary {
public:
    void PrintHeap() {
        Serial.print("Free Heap: ");
        Serial.print(ESP.getFreeHeap());
        Serial.println(" bytes");
        SchedulerClass::delay(1000);
    }
};

class MemTask : public Task {
private:
    MemLibrary memLibrary;
public:
    void loop() {
        memLibrary.PrintHeap();
    }
} mem_task;




void setup() {
    Serial.begin(115200);

    Serial.println("");

    delay(1000);

    Scheduler.start(&print_task);
    Scheduler.start(&blink_task);
    Scheduler.start(&mem_task);

    Scheduler.begin();
}

void loop() {}
