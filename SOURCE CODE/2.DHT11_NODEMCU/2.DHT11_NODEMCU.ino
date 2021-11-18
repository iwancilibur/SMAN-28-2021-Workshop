#include <DHT.h>

//INISIALISASI PIN
#define DHTTYPE DHT11
#define DHTPIN  D2
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
    //BACA DATA
    float dataSUHU        = dht.readTemperature();
    float dataKELEMBABAN  = dht.readHumidity();

    //POST TO SERIAL
    Serial.print("SUHU :");Serial.print(dataSUHU);
    Serial.print(" | KELEMBABAN :");Serial.print(dataKELEMBABAN);
    Serial.println(); //UNTUK ENTER
}
