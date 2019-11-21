#include <TimeLib.h>
#include <NtpClientLib.h>

NTP.begin("pool.ntp.org", 1, true);
NTP.setInterval(60000);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
