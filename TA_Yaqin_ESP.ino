/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPL8hlT4cH5"
#define BLYNK_DEVICE_NAME "Yaqin"
#define BLYNK_AUTH_TOKEN "VQGAQp6knBhPPubx6M9FTZZqWluGFAmv"

#include <ESP8266WiFi.h>                      // Install esp8266 by ESP8266 Community version 2.7.1
#include <BlynkSimpleEsp8266.h>               // Install Blynk by Volodymyr Shymanskyy version 0.6.1
#include <SoftwareSerial.h>

SoftwareSerial SerialSoft(14, 12);    // RX, TX

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "RND_Wifi";
char pass[] = "RND12345";

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
  // String my_s = "UNO01000200001AMAN";
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
//      delay(10);
//      break;

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

//  while(SerialSoft.available() > 0){
//    char d = (char) SerialSoft.read();
//    if(d == '\n'){  has_data = true; }
//    else if(d == '\r'){ }
//    else{ 
//        my_s += d;
//    }
//    
//    delay(1);
//  }

  if(has_data){
    has_data = false;

    // "UNO01000200001AMAN"
    Serial.println(my_s);
    String head = my_s.substring(0,3);
    if(head == "UNO"){
      String jarak_depan = my_s.substring(3, 7);
      String jarak_belakang = my_s.substring(7, 11);
      String led_r = my_s.substring(11, 12);
      String led_y = my_s.substring(12, 13);
      String led_g = my_s.substring(13,14);
      String stat = my_s.substring(14);
  
    //  Serial.printf("depan %s\r\n", jarak_depan);
    //  Serial.printf("belakang %s\r\n", jarak_belakang);
    //  Serial.printf("R %s, Y %s, G %s\r\n", led_r, led_y, led_g);
    //  Serial.printf("Stat %s\r\n", stat);
  
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
  }
  else{
    Serial.println("No Resp");
  }

  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void setup()
{
  delay(1000);

  // Debug console
  Serial.begin(115200);

  // Talk to Arduino UNO
  SerialSoft.begin(9600);

  // LED Built In Indicator
  pinMode(LED_BUILTIN, OUTPUT);

  // Blynk.begin(auth, ssid, pass);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  Serial.println("Start");
  // Setup a function to be called every second
  timer.setInterval(10000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
