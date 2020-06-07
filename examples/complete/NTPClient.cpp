#include "NTPClient.h"
#include <Scheduler.h>

NTPClient::NTPClient(UDP& udp) {
  _udp= &udp;
  _udp->begin(_port);
}

bool NTPClient::updateTime() {
  sendNTPPacket();
  
  // Wait till data is there or timeout...
  byte timeout   = 0;
  int packetBytes = 0;
  do {
    // **** This class is not a Task itself, but because it is being called from a Task the scheduler knows how to do a non-blocking delay     
    delay(100);
    packetBytes = _udp->parsePacket();
    if (timeout > 10) { return false; } // timeout after 1000 ms
    timeout++;
  } while (packetBytes == 0);


  _udp->read(_packetBuffer, NTP_PACKET_SIZE);

  unsigned long highWord = word(_packetBuffer[40], _packetBuffer[41]);
  unsigned long lowWord  = word(_packetBuffer[42], _packetBuffer[43]);
  // combine the four bytes (two words) into a long integer
  // this is NTP time (seconds since Jan 1 1900):
  unsigned long secsSince1900 = highWord << 16 | lowWord;

  _currentTime = secsSince1900 - SEVENZYYEARS;
  return _currentTime;
}

void NTPClient::sendNTPPacket() {
  // set all bytes in the buffer to 0
  memset(_packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  _packetBuffer[0]   = 0b11100011;   // LI, Version, Mode
  _packetBuffer[1]   = 0;     // Stratum, or type of clock
  _packetBuffer[2]   = 6;     // Polling Interval
  _packetBuffer[3]   = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  _packetBuffer[12]  = 49;
  _packetBuffer[13]  = 0x4E;
  _packetBuffer[14]  = 49;
  _packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  _udp->beginPacket(_poolServerName, 123); //NTP requests are to port 123
  _udp->write(_packetBuffer, NTP_PACKET_SIZE);
  _udp->endPacket();
}
