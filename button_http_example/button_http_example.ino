#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>

const uint8_t fingerprint[20] = {0x39, 0x8e, 0x01, 0xa5, 0x0c, 0x66, 0x8a, 0x74, 0xf0, 0x10, 0x4a, 0x83, 0x60, 0x15, 0xa2, 0x6e, 0x21, 0x55, 0x4c, 0xce};

ESP8266WiFiMulti WiFiMulti;

//Variables para WiFi
const char ssid[] = "redpucp";
const char pass[] = "C9AA28BA93";

//Servidor NTP
static const char ntpServerName[] = "us.pool.ntp.org";

//Zona horaria Lima
const int timeZone = -5;

//Parametros UDP
WiFiUDP Udp;
unsigned int localPort = 8888;

//Parametros de tiempo
time_t getNtpTime();

//Variables del boton
int button = D7;
int prevState = 0;

//Buffer de datos para JSON
char dataBuffer[300];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(button, INPUT);
  Serial.println("Starting program...");

  Serial.println("Attempting WiFi Connection, Stand By...");
  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("IP address assigned by DHCP is ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting Time module configuration");
  
  Serial.println("Starting UDP");  
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = digitalRead(button);

  if (val == HIGH && prevState == 0)
  {
    Serial.print("Button status is: ");
    Serial.println(1);
    prevState = 1;
    String ts = genTimestamp();
    int buttonState = 1;
    genJson(buttonState, ts);
    sendHttpRequest();
  }
  else if (val == LOW && prevState == 1)
  {
    Serial.print("Button status is: ");
    Serial.println(0);
    prevState = 0;
    String ts = genTimestamp();
    int buttonState = 2;
    genJson(buttonState, ts);
    sendHttpRequest();
  }
}

String genTimestamp()
{
  String ts = String(year()) + "-" + String(month()) + "-" + String(day()) + " " + String(hour()) + ":" + String(minute()) + ":" + String(second());

  return ts;
}

void genJson(int buttonState, String ts)
{
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& data = JSONBuffer.createObject();

    data["devId"] = 1;
    data["buttonState"] = buttonState;
    data["timestamp"] = ts;

    Serial.println("Generated JSON: ");
    data.prettyPrintTo(Serial);
    data.prettyPrintTo(dataBuffer, sizeof(dataBuffer));
}


void sendHttpRequest()
{

  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setFingerprint(fingerprint);
    
  HTTPClient https;

  Serial.println("[HTTPS] begin...");
  if(https.begin(*client, "https://jobenasbuttonhttpexample.azurewebsites.net/api/buttonHttpExample"))
  {
    Serial.println("[HTTPS] POST...");
    https.addHeader("Content-Type", "application/json");

    int httpCode = https.POST(dataBuffer);

    if(httpCode > 0)
    {
      Serial.print("[HTTPS] POST... code: ");
      Serial.println(httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String payload = https.getString();
        Serial.println(payload);
      }
      else
      {
        Serial.print("[HTTPS} POST... failed, error: ");
        Serial.println(https.errorToString(httpCode).c_str());
      }
      https.end();
    }
    else
    {
      Serial.println("[HTTPS] Unable to connect");
    }
  }
}
