#include <Wire.h>
#include <DHT.h>
#include <EEPROM.h>

#define DHTPIN 3      // Pin sensor DHT22
#define DHTTYPE DHT11  // Tipo Sensor DHT 22
DHT dht(DHTPIN, DHTTYPE); // Inicializando sensor DHT

#define DHTPIN2 4
#define DHTTYPE2 DHT11
DHT dht2(DHTPIN2,DHTTYPE2);
#define trigger 5 // Pines de sensor ultrasonico
#define echo 6    // Pines de sensor ultrasonico
int eepromAddress = 0;
int pinButtonCO2 = A6;  // C02
int pinButtonFoto = A7; // Fotoresistencia
// Struct para guardar información de los sensores
struct SensorData {
  int temperature;
  int humidity;
  int co2Level;
  int lightLevel;
  int distance;
};

SensorData currentData;

//valores para normalizar sensor dht11
int temAnt = 0;
int humAnt = 0;

int interruptPin = 3;

int IN1 = 8;
int IN2 = 7;

void setup() {
  dht.begin();
  dht2.begin();
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop() {
  // Verificar si hay datos disponibles en el puerto serial
  if (Serial.available() > 0) {
    // Leer el primer carácter recibido
    //;
    delay(10);
    //String receivedChar = Serial.read();
    String message = Serial.readStringUntil('\n');
    // Llamar al método processKeypad con el carácter recibido como argumento
    accionesIndividuales(message);
    // Imprimir el carácter recibido para verificar si se está recibiendo correctamente
    //Serial.print("Carácter recibido: "+message);
   
  }
  readSensors();
  sendSensorDataToESP32();
  
  /*delay(500); // Pausa entre cada envío de datos
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(3000);*/
  //digitalWrite(IN1, LOW);
  //digitalWrite(IN2, LOW);
}

void blink() {
  static unsigned long lastSensorReadTime = 0;
  const unsigned long sensorReadInterval = 4000; // Intervalo de lectura de sensores en milisegundos (por ejemplo, cada 5 segundos)

  unsigned long currentMillis = millis();

  // Leer los sensores cada cierto intervalo de tiempo
  if (currentMillis - lastSensorReadTime >= sensorReadInterval) {
    currentData.temperature = dht.readTemperature();
    currentData.humidity = dht.readHumidity();
    currentData.co2Level = analogRead(A0);
    currentData.lightLevel = analogRead(A1);
    currentData.distance = readUltrasonicSensor();
    lastSensorReadTime = currentMillis; // Actualizar el tiempo de la última lectura de sensores
  }
}

void readSensors() {

 temAnt = currentData.temperature;
 humAnt = currentData.humidity;
 
  currentData.temperature = dht.readTemperature();
  currentData.humidity = dht.readHumidity();
  currentData.co2Level = analogRead(A0);
  currentData.lightLevel = analogRead(A1);
  currentData.distance = readUltrasonicSensor();

  if( currentData.temperature == 0 ){
    currentData.temperature = temAnt;
  }else{
    temAnt = currentData.temperature;
  }

  if(currentData.humidity == 0 ){
    currentData.humidity = humAnt;
  }else {
    humAnt = currentData.humidity;
  }
  
}

int readUltrasonicSensor() {
  long duration, distance;
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration / 2 / 29.1;
  return distance;
}

void sendSensorDataToESP32() {
  // Enviar los datos de los sensores al ESP32
  Serial.print("#Todos");
  Serial.print(currentData.temperature);
  Serial.print(",");
  Serial.print(currentData.humidity);
  Serial.print(",");
  Serial.print(currentData.co2Level);
  Serial.print(",");
  Serial.print(currentData.lightLevel);
  Serial.print(",");
  Serial.println(currentData.distance);
}

void accionesIndividuales(String key) {
  Serial.print("key: "); Serial.print("%"); Serial.print(key.c_str()); Serial.print("%"); Serial.print("Temp"); Serial.print("%"); Serial.println(key.c_str() == "EEP");
  if (key.endsWith("Temp")) { // Enviar lectura del sensor de temperatura
    Serial.print("#Temperatura:");
    Serial.println(currentData.temperature);
  } else if (key.endsWith("CO2")) { // Enviar lectura del sensor de CO2
    Serial.print("#CO2:");
    Serial.println(currentData.co2Level);
  } else if (key.endsWith("Luz")) { // Enviar lectura del sensor de iluminación fotocelda
    Serial.print("#Lumenes:");
    Serial.println(currentData.lightLevel);
  } else if (key.endsWith("Hum")) { // Enviar lectura del sensor de humedad
    Serial.print("#Humedad:");
    Serial.println(currentData.humidity);
  } else if (key.endsWith("Dist")) { // Enviar lectura del sensor de distancia
    int distance = readUltrasonicSensor();
    Serial.print("#Distancia:");
    Serial.println(distance);
    //Serial.print("Estado:");
    //Serial.println(distance < 20 ? "Presente" : "Ausente");
  } else if (key.startsWith("EEP")) { // Guardar la información en la memoria EEPROM
    saveDataToEEPROM(currentData);
    //Serial.println("#EEPROM");
  } else if (key.startsWith("READ")) { // Mostrar información guardada en la memoria EEPROM
    displaySavedData();
  } else if(key.startsWith("#Motor:1")){ // Enviar lecturas de todos los sensores
    motorDC(1);
  } else if(key.startsWith("#Motor:0")){
    motorDC(0);
  }
}

void motorDC(int on){
  if(on == 1){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }else{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
}

void saveDataToEEPROM(SensorData data) {
  EEPROM.put(eepromAddress, data);
}
void displaySavedData() {
  // Leer los datos guardados en la memoria EEPROM
  SensorData tempData;
  EEPROM.get(0, tempData);

  // Enviar los datos al ESP32
  Serial.print("#BEGIN_EEPROM");
  Serial.print(tempData.temperature);
  Serial.print(",");
  Serial.print(tempData.humidity);
  Serial.print(",");
  Serial.print(tempData.co2Level);
  Serial.print(",");  
  Serial.print(tempData.lightLevel);
  Serial.print(",");
  Serial.println(tempData.distance);
}

String nivelLuz(int valor) {
  if (valor <= 100)
    return " Alta";
  if (valor > 900)
    return " Baja";
  return " Media";
}
