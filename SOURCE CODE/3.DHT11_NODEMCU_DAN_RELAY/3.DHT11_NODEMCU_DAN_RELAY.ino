#include <DHT.h>

//INISIALISASI PIN
int relay1=D5;
int relay2=D6;

#define DHTTYPE DHT11
#define DHTPIN  D2
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(relay1, OUTPUT);     // Initialize the LED pin as an output
  pinMode(relay2, OUTPUT);
}

void loop() {
    //BACA DATA
    float dataSUHU        = dht.readTemperature();
    float dataKELEMBABAN  = dht.readHumidity();

    //POST TO SERIAL
    Serial.print("SUHU :");Serial.print(dataSUHU);
    Serial.print(" | KELEMBABAN :");Serial.print(dataKELEMBABAN);
    Serial.println(); //UNTUK ENTER

    //LOGIKA PROGRAM
    if(dataSUHU >=30){
      digitalRead(relay1,HIGH);
      Serial.println("RELAY 1 MENYALA");
    }else{
      digitalRead(relay1,LOW);
      Serial.println("RELAY 1 MATI");
    }
}
