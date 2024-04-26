
//////////// Initiering ///////////

//inkluderer nødvendige bibloteker
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <ArduinoJson.h>
// Definerer id og password til netværksforbindelse som NodeMCU anvender
char *ssid = "Alb"; //Indsæt navnet på jeres netværk her
const char *password = "aaaaaaaa"; //Indsæt password her
// Definerer information til mqtt serveren
const char *mqtt_server = "maqiatto.com"; //navn på mqtt-server. Find navnet på cloudmqtt-hjemmesiden
const int mqtt_port = 1883; // Definerer porten
const char *mqtt_user = "s223989@dtu.dk"; // Definerer mqtt-brugeren
const char *mqtt_pass = "1234"; // Definerer koden til mqtt-brugeren
const int buttonPin = 7;  
String payload; // Definerer variablen 'payload' i det globale scope (payload er navnet på besked-variablen)
int lock = 2;

/////// FUNKTIONSOPSÆTNING ////////
// Opretter en placeholder for callback-funktionen til brug senere. Den rigtige funktion ses længere nede.
void callback(char* byteArraytopic, byte* byteArrayPayload, unsigned int length);

// Opretter en klient der kan forbinde til en specifik internet IP adresse.
WiFiClient espClient; // Initialiserer wifi bibloteket ESP8266Wifi, som er inkluderet under "nødvendige bibloteker"

// Opretter forbindelse til mqtt klienten:
PubSubClient client(mqtt_server, mqtt_port, callback, espClient); // Initialiserer bibloteket for at kunne modtage og sende beskeder til mqtt. Den henter fra definerede mqtt server og port. Den henter fra topic og beskeden payload

///////// CALLBACKFUNKTION ////////
// Definerer callback funktionen der modtager beskeder fra mqtt
// OBS: den her funktion kører hver gang MCU'en modtager en besked via mqtt
void callback(char* byteArraytopic, byte* byteArrayPayload, unsigned int length) {

  // Konverterer indkomne besked (topic) til en string:
  String topic;
  topic = String(byteArraytopic);
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  // Konverterer den indkomne besked (payload) fra en array til en string:
  // Topic == Temperaturmaaler, Topic == Kraftsensor
  if(topic == "s223989@dtu.dk/lock"){
    payload = ""; // Nulstil payload variablen så forloopet ikke appender til en allerede eksisterende payload
    for (int i = 0; i < length; i++) {
      payload += (char)byteArrayPayload[i];
    }
    lock = payload.toInt();
    Serial.println(lock);
  }
}

/////// OPSÆTNING AF WIFI-FORBINDELSE  ///////////
// Opretter forbindelse til WiFi
void setup_wifi() {
  // Forbinder til et WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    delay(5000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
// Opretter forbindelse til mqtt server. Dette gentages ved manglende forbindelse til WiFi, server osv.
void reconnect() { 
  // Fortsætter til forbindelsen er oprettet
  while (!client.connected()) {
    Serial.print("Forsøger at oprette MQTT forbindelse...");

    if (client.connect("Mekatronik24", mqtt_user, mqtt_pass)) { // Forbinder til klient med mqtt bruger og password
      Serial.println("connected");
      // Derudover subsribes til topic "Test" hvor NodeMCU modtager payload beskeder fra
      client.subscribe("s223989@dtu.dk/lock");
      //client.subscribe("Test1");
      // Eller til samtlige topics ved at bruge '#' (Se Power Point fra d. 18. marts)
      // client.subscribe("#");

      // Hvis forbindelsen fejler køres loopet igen efter 5 sekunder indtil forbindelse er oprettet
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Venter 5 sekunder før den prøver igen
      delay(5000);
    }
  }
}

///////// SETUP ///////////
void setup() {
  Serial.begin(115200); // Åbner serial porten og sætter data raten til 115200 baud
  delay(1000);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  setup_wifi(); // Kører WiFi loopet og forbinder herved.
  client.setServer(mqtt_server, mqtt_port); // Forbinder til mqtt serveren (defineret længere oppe)
  client.setCallback(callback); // Ingangsætter den definerede callback funktion hver gang der er en ny besked på den subscribede "cmd"- topic
}

///////// FUNCTIONS //////////
void turnOnLED(bool LEDSTATE){
  digitalWrite( 3, LEDSTATE);
  digitalWrite(4, !LEDSTATE);
  delay(1000);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
}

/////// LOOP /////////
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 
  delay(1000);
  if(lock != 2){
    turnOnLED(lock);

    lock = 2;
  }
  // Hvis der opstår problemer med forbindelsen til mqtt broker oprettes forbindelse igen ved at køre client loop
}

