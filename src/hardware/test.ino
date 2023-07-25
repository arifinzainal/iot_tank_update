// Wifi Configuration
char ssid[] = "Ss";
char pass[] = "123456789";

// Blynk related
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6vlFkRT5j"
#define BLYNK_TEMPLATE_NAME "Smart Water Tank"
#define BLYNK_AUTH_TOKEN "XS_qvHlGxMEwrBXajH3Jzjd8LMkdMvsW"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = BLYNK_AUTH_TOKEN;

// Ultrasonic sensor
#define trigPin D3
#define echoPin D2
#include <NewPing.h>
#include <cmath>
#define MAX_DISTANCE 400
NewPing sonar = NewPing(trigPin, echoPin, MAX_DISTANCE);

// Waterflow sensors
volatile int pulsa_sensor1;
volatile int pulsa_sensor2;
unsigned int literPerjam1;
unsigned int literPerjam2;
unsigned char pinFlowsensor1 = D8; // Pin sensor 1 as input connected to D8
unsigned char pinFlowsensor2 = D7; // Pin sensor 2 as output connected to D7
unsigned long waktuAktual;
unsigned long waktuLoop;
double liter;

void ICACHE_RAM_ATTR cacahPulsa1()
{
  pulsa_sensor1++;
}

void ICACHE_RAM_ATTR cacahPulsa2()
{
  pulsa_sensor2++;
}

void setup()
{
  Serial.begin(9600);
  pinMode(pinFlowsensor1, INPUT); // Declare sensor 1 as input
  pinMode(pinFlowsensor2, INPUT); // Declare sensor 2 as input
  digitalWrite(pinFlowsensor1, HIGH);
  digitalWrite(pinFlowsensor2, HIGH);
  attachInterrupt(digitalPinToInterrupt(pinFlowsensor1), cacahPulsa1, RISING);
  attachInterrupt(digitalPinToInterrupt(pinFlowsensor2), cacahPulsa2, RISING);
  waktuAktual = millis();
  waktuLoop = waktuAktual;
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  int T_Bak = 31;
  int T_Air = (T_Bak - sonar.ping_cm());
  // int d = 24;
  // double r = d / 2;
  // double v = M_PI * pow(r, 2) * T_Air;
  // int v2 = v * 1000;
  Serial.print(T_Air);
  Serial.println(" CM");
  Blynk.virtualWrite(V1, T_Air);
  // Serial.print(v2);
  // Serial.println(" CM³");
  // Blynk.virtualWrite(V4, v2);

  waktuAktual = millis();
  if (waktuAktual >= (waktuLoop + 1000))
  {
    waktuLoop = waktuAktual;
    literPerjam1 = (pulsa_sensor1 * 60 / 7.5); // Measure flow rate for sensor 1
    literPerjam2 = (pulsa_sensor2 * 60 / 7.5); // Measure flow rate for sensor 2
    pulsa_sensor1 = 0;
    pulsa_sensor2 = 0;
    Serial.print("Flow Rate Sensor 1: ");
    Serial.print(literPerjam1, DEC);
    Serial.println(" L/jam");
    Blynk.virtualWrite(V2, literPerjam1);
    Serial.print("Flow Rate Sensor 2: ");
    Serial.print(literPerjam2, DEC);
    Serial.println(" L/jam");
    Blynk.virtualWrite(V3, literPerjam2);
  }
  delay(1000);
}