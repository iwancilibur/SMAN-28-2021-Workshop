#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

//KONFIGURASI SSID & PASSWORD DAN SETTING SERVER MQTT BROKER
const char* ssid          = "www.interactiverobotics.club";
const char* password      = "cilibur2019";
const char* mqtt_server   = "iwancilibur.ip-dynamic.com"; // MQTT Broker pubic (iwancilibur.ip-dynamic.com)
//const char* mqtt_server   = "broker.hivemq.com"; // MQTT Broker pubic (iwancilibur.ip-dynamic.com)
      char* clientid      = "iwanciliburDevice01";

//INISIALISASI PIN
int relay1=D5;
int relay2=D6;

#define DHTTYPE DHT11
#define DHTPIN  D2
DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  dht.begin();
  pinMode(relay1, OUTPUT);     // Initialize the LED pin as an output
  pinMode(relay2, OUTPUT);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


void callback(char *Topic, byte *message, unsigned int length)
{
  Serial.print("Pesan yang di Terima: ");
  Serial.print(Topic);
  Serial.print(". isi pesan: ");
  
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  
  //Jika topic "iwan/relay1" menerima pesan "on" maka Relay HIGH (Menyala), jika "off" maka Relay LOW (Mati)
  if (String(Topic) == "iwan/relay1") {
    Serial.print("Status :  ");
    if(messageTemp =="on"){ 
      digitalWrite(relay1, HIGH);
      Serial.println("LAMPU 1 NYALA");
    }else if(messageTemp =="off"){
      digitalWrite(relay1, LOW); 
      Serial.println("LAMPU 1 MATI");
  }
  }

  //Jika topic "iwan/relay2" menerima pesan "on" maka Relay HIGH (Menyala), jika "off" maka Relay LOW (Mati)
  if (String(Topic) == "iwan/relay2") {
    Serial.print("Status :  ");
    if(messageTemp =="on"){ 
      digitalWrite(relay2, HIGH);
      Serial.println("LAMPU 2 NYALA");
    }else if(messageTemp =="off"){
      digitalWrite(relay2, LOW); 
      Serial.println("LAMPU 2 MATI");
  }
  }
  
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
    client.loop();
    delay(1000);

    //BACA DATA
    float dataSUHU = dht.readTemperature();
    float dataKELEMBABAN = dht.readHumidity();

    //POST TO SERIAL
    Serial.print("SUHU :");Serial.print(dataSUHU);
    Serial.print(" | KELEMBABAN :");Serial.print(dataKELEMBABAN);
    Serial.println(); //UNTUK ENTER
    
    //POST TO MQTT
    client.publish("iwan/suhu", String(dataSUHU).c_str()); //Nama Topic harus Unix (tidak boleh sama)
    client.publish("iwan/kelembaban", String(dataKELEMBABAN).c_str());//Nama Topic harus Unix (tidak boleh sama)

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

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Menghubungkan Ulang ke MQTT...");
    // AtsuhuPublisht to connect
    if (client.connect(clientid)) {
      Serial.println("connected");
      client.subscribe("iwan/relay1");
      client.subscribe("iwan/relay2");
      // Once connected, publish an announcement...
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
