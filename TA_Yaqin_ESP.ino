/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPL8hlT4cH5"
#define BLYNK_DEVICE_NAME "Yaqin"
#define BLYNK_AUTH_TOKEN "VQGAQp6knBhPPubx6M9FTZZqWluGFAmv"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

SoftwareSerial SerialSoft(4, 5);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "OFFICE";
char pass[] = "seipandaan";

BlynkTimer timer;

WidgetLED ledR(V3);
WidgetLED ledY(V4);
WidgetLED ledG(V5);

uint32_t timer_serial;
bool has_data;

#define SERIAL_LEN   100
char text[SERIAL_LEN];

void myTimerEvent()
{
  // String my_s = "01000200001AMAN";
  String my_s = "";

  Serial.println("DATA");
  timer_serial = millis();  
  do{
    if(Serial.available() > 0){
      delay(10);
      break;
    }
  }while((millis() - timer_serial) <= 500);

  while(Serial.available() > 0){
    byte d = (char) Serial.read();
    if(d == '\n'){  hasData = true; }
    else if(d == '\r'){ }
    else{ 
        text[n] = d;
        n++;  
    }
    
    delay(1);
  }

  if(has_data){
    String jarak_depan = my_s.substring(0, 4);
    String jarak_belakang = my_s.substring(4, 8);
    String led_r = my_s.substring(8, 9);
    String led_y = my_s.substring(9, 10);
    String led_g = my_s.substring(10,11);
    String stat = my_s.substring(11);

    // Serial.printf("depan %s\r\n", jarak_depan);
    // Serial.printf("belakang %s\r\n", jarak_belakang);
    // Serial.printf("R %s, Y %s, G %s\r\n", led_r, led_y, led_g);
    // Serial.printf("Stat %s\r\n", stat);

    // You can send any value at any time.
    // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V1, jarak_depan.toInt());
    Blynk.virtualWrite(V2, jarak_belakang.toInt());
    
    if(led_r.toInt()) { ledR.on(); }
    else { ledR.off(); }

    if(led_y.toInt()) { ledY.on(); }
    else { ledY.off(); }

    if(led_g.toInt()) { ledG.on(); }
    else { ledG.off(); }
    
    Blynk.virtualWrite(V6, stat);
  }

  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  // Talk to Arduino UNO
  SerialSoft.begin(9600);

  // LED Built In Indicator
  pinMode(LED_BUILTIN, OUTPUT);

  Blynk.begin(auth, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
