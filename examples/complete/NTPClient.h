#pragma once

#include "Arduino.h"
#include <Udp.h>


#define SEVENZYYEARS 2208988800UL
#define NTP_PACKET_SIZE 48
#define NTP_DEFAULT_LOCAL_PORT 1337

class NTPClient {
  private:
    UDP*          _udp;
    const char*   _poolServerName = "pool.ntp.org"; // Default time server
    int           _port           = NTP_DEFAULT_LOCAL_PORT;

    unsigned long _currentTime    = 0;      // In s
    unsigned long _lastUpdate     = 0;      // In ms
    byte          _packetBuffer[NTP_PACKET_SIZE];
    void          sendNTPPacket();

  public:
    NTPClient(UDP& udp);
    bool updateTime();
};
