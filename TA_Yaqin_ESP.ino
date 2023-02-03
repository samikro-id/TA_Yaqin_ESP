#include <ESP8266WiFi.h>                  // Install esp8266 by ESP8266 Community version 2.7.1
#include <SoftwareSerial.h>
#include <PubSubClient.h>                 // Install Library by Nick O'Leary version 2.7.0

SoftwareSerial SerialSoft(14, 12);    // RX, TX
WiFiClient espClient;
PubSubClient client(espClient);

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "RND_Wifi";
char pass[] = "RND12345";

#define MQTT_ID         "7912c119-3a3c-4127-ac2e-440aadd221218"

#define MQTT_BROKER     "broker.emqx.io"            //
#define MQTT_PORT       1883                        //
#define MQTT_USERNAME   ""                          // Change to your Username from Broker
#define MQTT_PASSWORD   ""                          // Change to your password from Broker
#define MQTT_TIMEOUT    10
#define MQTT_QOS        0
#define MQTT_RETAIN    false

uint32_t timer_serial;
uint32_t led_time;
bool has_data;

#define SERIAL_LEN   100

void setup()
{
  delay(3000);

  // Debug console
  Serial.begin(115200);

  // Talk to Arduino UNO
  SerialSoft.begin(9600);

  // LED Built In Indicator
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);                            delay(10);
  ESP.wdtFeed();                                  yield();

  Serial.println("Start");
  timer_serial = millis();
}

void loop()
{
  if(WiFi.status() == WL_CONNECTED){
    if(client.connected()){
      client.loop();

      if((millis() - timer_serial) > 2000){
        timer_serial = millis();

        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

        // String my_s = "UNO0100020003000400001AMAN";
        String my_s = "";

        do{
          if(SerialSoft.available() > 0){
            char d = (char) SerialSoft.read();
            timer_serial = millis();
          }
        }while((millis() - timer_serial) <= 100);

        SerialSoft.println("DATA");
        timer_serial = millis();  
        do{
          if(SerialSoft.available() > 0){
            char d = (char) SerialSoft.read();
            if(d == '\n'){  
              has_data = true; 
              break;
            }
            else if(d == '\r'){ }
            else{ my_s += d;  }

            timer_serial = millis();
          }
        }while((millis() - timer_serial) <= 500);

        if(has_data){
          has_data = false;

          // "UNO0100020003000400001AMAN"
          // Serial.println(my_s);
          client.publish("samikro/data/project/4", my_s.c_str(),false);
        }
        else{
          Serial.println("No Resp");
        }
      }
    }
    else{
      client.disconnect();
      client.setServer(MQTT_BROKER, MQTT_PORT);

      uint8_t i;
      for(i = 0; i < MQTT_TIMEOUT; i++) {
          
        if( client.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASSWORD) ){
          break;
        }

        delay(500);    
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));    // toggle led setiap 500ms                         
      }
    }
  }
  else{
    WiFi.begin(ssid, pass);
    while(WiFi.status() != WL_CONNECTED){
      yield();

      if((millis()-led_time) > 200){          // toggle led setiap 200ms
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      }

      if(WiFi.status() == WL_CONNECTED){
        break;
      }
    }
  }
}
