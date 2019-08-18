#define Vcc 13
#define Vee 12
#define Claw 27
#define Wrist 33 
#define Elbow 15
#define Shoulder 32
#define Base 14
#define Ddelay 11

#include <WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "YOURSSID";
const char* password =  "YOURPASS";
const char* mqttServer = "m12.cloudmqtt.com";
const int mqttPort = 12345;
const char* mqttUser = "YOURCLOUDMQTTUSER";
const char* mqttPassword = "YOURCLOUDMQTTPASS";
bool sw = false;
int counter = 0;
int mov=0;
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
  pinMode(Vcc,OUTPUT);  
  pinMode(Vee,OUTPUT); 
  pinMode(Claw,OUTPUT);
  pinMode(Wrist,OUTPUT);
  pinMode(Elbow,OUTPUT);
  pinMode(Shoulder,OUTPUT);
  pinMode(Base,OUTPUT);
  // Turning Off Everything
  digitalWrite(Vcc,LOW); 
  digitalWrite(Vee,HIGH); 
  digitalWrite(Claw,HIGH);
  digitalWrite(Wrist,HIGH);
  digitalWrite(Elbow,HIGH);
  digitalWrite(Shoulder,HIGH);
  digitalWrite(Base,HIGH);
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ANYNAME", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("/ESPTopic", "Hello from ESP32"); 
  client.subscribe("/ArmTopic"); 
  delay(5000);
  digitalWrite(Vcc,HIGH);
  delay(5000);
  Serial.print("Move Arm Start");
  FmoveP();
  Fmove();
  Fmove();
  Fmove();
  Serial.print("Move Arm Stop");

}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  String datas="";
  for (int i = 0; i < length; i++) 
  {
    datas+=char(payload[i]);
  }
  Serial.println(datas);
  if(datas=="Sit Flex" && mov==0 && counter<10)
  {
    counter++;
  }
  else if(datas=="Sit Flex" && mov==0 && counter==10)
  {
    mov++;
    counter=0;
    FmoveR();
    SmoveP();
    Smove();
    Smove();
    Smove();
  }
  else if(datas=="Sit Up" && mov==1 && counter<10)
  {
    counter++;
  }
  else if(datas=="Sit Up" && mov==1 && counter==10)
  {
    mov++;
    counter=0;
    SmoveR();
    TmoveP();
    Tmove();
    Tmove();
    Tmove();
  }
   else if(datas=="Sit Inside" && mov==2 && counter<10)
  {
    counter++;
  }
  else if(datas=="Sit Inside" && mov==2 && counter==10)
  {
    mov++;
    counter=0;
    TmoveR();
  }
  
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void up(int pin,int Delay)
{
  digitalWrite(Claw,HIGH);
  digitalWrite(Wrist,HIGH);
  digitalWrite(Elbow,HIGH);
  digitalWrite(Shoulder,HIGH);
  digitalWrite(Base,HIGH);
  digitalWrite(pin,LOW);
  digitalWrite(Vcc,LOW);  // +Vcc ON
  delay(Delay);
  digitalWrite(Vcc,HIGH);
  digitalWrite(pin,HIGH);
}

void down(int pin, int Delay)
{
  digitalWrite(Claw,LOW);
  digitalWrite(Wrist,LOW);
  digitalWrite(Elbow,LOW);
  digitalWrite(Shoulder,LOW);
  digitalWrite(Base,LOW);
  digitalWrite(pin,HIGH);
  digitalWrite(Vee,LOW); // -Vee ON
  delay(Delay);
  digitalWrite(Vee,HIGH);
  digitalWrite(Claw,HIGH);
  digitalWrite(Wrist,HIGH);
  digitalWrite(Elbow,HIGH);
  digitalWrite(Shoulder,HIGH);
  digitalWrite(Base,HIGH);
  
}


void FmoveP(void)
{
  down(Shoulder,5000);
  down(Elbow,5500);
}
void Fmove(void)
{
  up(Wrist,3500);
  down(Wrist,3500);
}
void FmoveR(void)
{
  up(Shoulder,5500);
  up(Elbow,6000);
}

void SmoveP(void)
{
  up(Base,6000);
  down(Shoulder,5000);
  down(Elbow,5300);
}
void Smove(void)
{
  up(Elbow,6500);
  down(Elbow,5300);
}
void SmoveR(void)
{
  up(Elbow,6000);
  up(Shoulder,5500);
  down(Base,6000);
  
}

void TmoveP(void)
{
  up(Base,6300);
  down(Shoulder,5000);
  up(Elbow,3000);
  up(Wrist,3500); 
}
void Tmove(void)
{
  up(Elbow,4000);
  down(Elbow,4000);
  up(Elbow,4000);
  down(Elbow,4000);
}
void TmoveR(void)
{
  down(Elbow,3500);
  down(Wrist,3500);
  up(Shoulder,5700);
  down(Base,6300);
}


void reconnect() {
 while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client2", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  client.publish("/ESPTopic", "Hello from ESP32"); 
  client.subscribe("/ArmTopic"); 
}
