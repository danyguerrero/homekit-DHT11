#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <DHT11.h>

int err;
float temp, humi;

IPAddress server(192, 168, 0, 67); // IP de la raspberry Pi

const char* ssid     = "SSID"; // Your ssid
const char* pass = "PASSWORD"; // Your Password

int pin = 0;
int relay = 2;


#define BUFFER_SIZE 100

DHT11 dht11(pin);

WiFiClient wclient;
PubSubClient client(wclient, server);
#define BUFFER_SIZE 100

void callback(const MQTT::Publish& pub) {
    if(pub.payload_string() == "on")
    {
      digitalWrite(relay, LOW); // en caso de que el modulo rele funcione al reves, cambiarl LOW por HIGH
    }
    else
    {
      digitalWrite(relay, HIGH); // en caso de que el modulo rele funcione al reves, cambiarl HIGH por LOW
    }  
}

void setup() 
{
  pinMode(relay,OUTPUT);
  digitalWrite(relay, HIGH);
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  client.set_callback(callback);
  if (WiFi.status() != WL_CONNECTED) 
  {
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println("...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  return;
  Serial.println("WiFi connected");
  }
  
}

void loop() 
{
if (WiFi.status() == WL_CONNECTED)
get_temp(); 
{
  if (!client.connected()) 
  {
    if (client.connect("ESP8266: LuzSensor"))  
    {   
      client.subscribe("LuzSensor");
    }
  }
    if (client.connected())
    {
      Serial.println(String(temp)+"°")  ;
      client.publish("Temperatura",String(temp));
      Serial.println(String(humi)+"%")  ;
      client.publish("Humedad",String(humi));
    }
    client.loop();
  }
}

void get_temp() 
{
if ((err = dht11.read(humi, temp)) == 0)
{
  Serial.print("temperature:");
  Serial.print(temp);
  Serial.print(" humidity:");
  Serial.print(humi);
  Serial.println();
}
else
{
  Serial.println();
  Serial.print("Error No :");
  Serial.print(err);
  Serial.println();
}
delay(DHT11_RETRY_DELAY); //delay for reread
}
