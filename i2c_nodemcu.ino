#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>


char auth[] = "QJ3sOarWRIVwVOdvN1FYs0P01EGsg4Wj";//Enter your Auth token
char ssid[] = "i13";//Enter your WIFI name
char pass[] = "12345678";//Enter your WIFI password


DHT dht(2, DHT11); //(sensor pin,sensor type)
BlynkTimer timer;
bool pirbutton = 0;

#define Buzzer 10
#define MQ2 A0
#define flame D0
#define PIR D3
#define trig D5
#define echo D6
#define relay1 13
#define relay2 15

BLYNK_WRITE(V6) {
  bool value1 = param.asInt();
  Serial.print("LED 1 Status= ");
  // Check these values and turn the relay1 ON and OFF
  if (value1 == 1) {
    digitalWrite(relay1, HIGH);
    Serial.println("ON");
  } else {
    digitalWrite(relay1, LOW);
    Serial.println("OFF");
  }
}

//Get the button values
BLYNK_WRITE(V7) {
  bool value2 = param.asInt();

  Serial.print("LED 2 Status= ");
  // Check these values and turn the relay2 ON and OFF
  if (value2 == 1) {
    Serial.println("ON");
    digitalWrite(relay2, HIGH);
  } else {
    Serial.println("OFF");
    digitalWrite(relay2, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(Buzzer, OUTPUT);
  pinMode(flame, INPUT);
  pinMode(PIR, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

 
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(100L, gassensor);
  timer.setInterval(100L, DHT11sensor);
  timer.setInterval(100L, flamesensor);
  timer.setInterval(100L, pirsensor);
  timer.setInterval(100L, ultrasonic);
}

void gassensor() {
  int value = analogRead(MQ2);
  Serial.println("---------------------");
  Serial.print("Gas= ");
  Serial.println(value);
  value = map(value, 0, 1024, 0, 100);
  Blynk.virtualWrite(V0, value);
}

void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity= ");
  Serial.println(h);
  Serial.print("Temperature= ");
  Serial.println(t);
  Blynk.virtualWrite(V2, t);
  Blynk.virtualWrite(V4, h);
  
}

void flamesensor() {
  bool value = digitalRead(flame );
  if(value==1){
    Serial.println("No Fire Detected !!");
  }
  else{
    Serial.println("Warning! Fire was detected");
  }
  Blynk.virtualWrite(V1,value);

}
void pirsensor() {
  bool value = digitalRead(PIR);
    if (value == 0) {
      Blynk.virtualWrite(V2,value);
    } 
    else if (value == 1) {
      Serial.println("Warning! Please check your security system");
      Blynk.virtualWrite(V3,value);
    }
  
}

void ultrasonic() {
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long t = pulseIn(echo, HIGH);
  long cm = t / 29 / 2;
  Blynk.virtualWrite(V5, cm);
  Serial.print("UltraSonic= ");
  Serial.println(cm);
  Serial.println("---------------------");
}


void loop() {
  Blynk.run();
  timer.run();
  delay(5000);
}