
#include <PubSubClient.h>
#include <WiFi.h>

#define ssid "Freebox-75729C"
#define password "multaris69-peritus6-exoleto-confossuri"

const char* mqtt_server = "192.168.1.45";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

//const int lightGPIO = 2;
const int echoPin = 35;
const int trigPin = 32;
const long interval = 5000;        // Interval at which to publish sensor readings : 5 sec
const int ledPin = 5;

float timetravel;
float distance;

void setup() {
  Serial.begin(9600);
  Serial.println();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
 
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("ofF");
      digitalWrite(ledPin, LOW);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void loop() {
   if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
 

//  float tempVal = analogRead(tempGPIO);
//  Serial.println("Temperature = " + String(tempVal/1024*3.3 + basetemperature));
//  char mqttTemp[5];
//  String tempstring = String((tempVal/1024*3.3 + basetemperature));
//  tempstring.toCharArray(mqttTemp,5);
//  client.publish("esp32/temperature", mqttTemp);
//  Serial.println(mqttTemp);

//  float soundVal = analogRead(soundGPIO);
//  Serial.println("Sound = " + String(soundVal/1024*3.3));
//  char mqttSound[5];
//  String soundstring = String((soundVal/1024*3.3));
//  soundstring.toCharArray(mqttSound,5);
//  client.publish("esp32/sound", mqttSound);
//  delay(interval);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  timetravel = pulseIn(echoPin, HIGH);
  distance = timetravel*0.034/2;
  Serial.println("Distance : " + String(distance) + " cm");
  char mqttDistance[5];
  String distanceString = String(distance);
  distanceString.toCharArray(mqttDistance,5);
  client.publish("esp32/distance", mqttDistance);

  delay(interval);
 }
}
