#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#include <PubSubClient.h>

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

//variables del LED
//int led = D8;

//Buffer de datos para JSON
char dataBuffer[300];

//Parametros MQTT
const char* mqtt_server = "iot.jobenas.com";
WiFiClient espClient;
PubSubClient mqttClient(espClient);
long lastMsg = 0;
int value = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Serial.println("[System] Starting program...");

  Serial.println("[System] Attempting WiFi Connection, Stand By...");

  setupWifi();

  Serial.println("[System] Starting Time module configuration");
  
  Serial.println("[Time] Starting UDP");  
  Udp.begin(localPort);
  Serial.print("[Time] Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("[Time] Waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  Serial.println("[System] Starting MQTT Configuration");
  
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback);
  reconnect();

  Serial.println("[System] All Configurations completed, starting normal operation");
}

void loop() {
  int val = digitalRead(button);

  if (val == HIGH && prevState == 0)
  {
    Serial.print("[System] Button status is: ");
    Serial.println(1);
    prevState = 1;
    String ts = genTimestamp();
    int buttonState = 1;
    genJson(buttonState, ts);
    mqttClient.publish("buttonStatus", dataBuffer);
//    sendHttpRequest();
  }
  else if (val == LOW && prevState == 1)
  {
    Serial.print("[System] Button status is: ");
    Serial.println(0);
    prevState = 0;
    String ts = genTimestamp();
    int buttonState = 2;
    genJson(buttonState, ts);
//    sendHttpRequest();
    mqttClient.publish("buttonStatus", dataBuffer);
  }

  if(!mqttClient.connected())
  {
    reconnect();
  }
  mqttClient.loop();
}

void setupWifi()
{
  delay(10);
  
  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("[WiFi] Connection established");
  
  Serial.print("[WiFi] IP address assigned by DHCP is ");
  Serial.println(WiFi.localIP());

  randomSeed(micros());
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("[MQTT] Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for(int i=0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if((char)payload[0] == '1')
  {
//    digitalWrite(led, HIGH);
    Serial.println("[Callback] LED ON");
  }
  else if((char)payload[0] == '0')
  {
//    digitalWrite(led, LOW);
    Serial.println("[Callback] LED OFF");
  }
}

void reconnect()
{
  while(!mqttClient.connected())
  {
    Serial.print("[MQTT] Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff),HEX);
    if(mqttClient.connect(clientId.c_str()))
    {
      Serial.println("[MQTT] Connected");
      mqttClient.publish("buttonStatus", "First Message");
      mqttClient.subscribe("ledStatus");
    }
    else
    {
      Serial.print("[MQTT] Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
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

//    Serial.println("Generated JSON: ");
//    data.prettyPrintTo(Serial);
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
