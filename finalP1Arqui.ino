#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <DHT.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD 2X16

#define DHTPIN 10      // Pin sensor DHT22
#define DHTTYPE DHT11  // Tipo Sensor DHT 22
DHT dht(DHTPIN, DHTTYPE); // Inicializando sensor DHT

#define DHTPIN2 13
#define DHTTYPE2 DHT11
DHT dht2(DHTPIN2,DHTTYPE2);
#define trigger 22 // Pines de sensor ultrasonico
#define echo 23    // Pines de sensor ultrasonico
char buffer1[16];
bool cambio=false;
int eepromAddress = 0;
int pinButtonCO2 = A0;  // C02
int pinButtonFoto = A1; // Fotoresistencia
//int pinHumedad = 13;    // Humedad
int interruptPin = 3;

// Struct para guardar información de los sensores
struct SensorData {
  int temperature;
  int humidity;
  int co2Level;
  int lightLevel;
  int distance;
};

SensorData currentData;

const byte ROWS = 3; // Filas
const byte COLS = 3; // Columnas
char keys[ROWS][COLS] = { // Simbolos
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'}};
byte rowPins[ROWS] = {9, 8, 7};
byte colPins[COLS] = {6, 5, 4};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SENSOR CLIMATICO ");
  lcd.setCursor(0, 1);
  lcd.print("  GRUPO 2  ");
  dht.begin();
  dht2.begin();
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
  Serial.begin(9600);
}

void loop() {
  readSensors();
  if(cambio){
    processKeypad('9');
    cambio = !cambio;
  }
  char key = keypad.getKey();
  if (key != NO_KEY) {
    processKeypad(key);
  }
}



void blink()
{ // funcion ISR0

  static unsigned long lastSensorReadTime = 0;
  const unsigned long sensorReadInterval = 4000; // Intervalo de lectura de sensores en milisegundos (por ejemplo, cada 5 segundos)

  unsigned long currentMillis = millis();

  // Leer los sensores cada cierto intervalo de tiempo
  if (currentMillis - lastSensorReadTime >= sensorReadInterval) {
    int temp = dht.readTemperature();
    Serial.println(temp);
    cambio = !cambio;
    lastSensorReadTime = currentMillis; // Actualizar el tiempo de la última lectura de sensores
  }   
}

void readSensors() {
  currentData.temperature = dht.readTemperature();
  currentData.humidity = dht2.readHumidity();
  //currentData.humidity = analogRead(pinHumedad);
  currentData.co2Level = analogRead(pinButtonCO2);
  currentData.lightLevel = analogRead(pinButtonFoto);
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

void processKeypad(char key) {
  lcd.clear();
  switch (key) {
  case '1': // caso para mostrar sensor de temperatura
    lcd.setCursor(0, 0);
    lcd.print("Temperatura:");
    lcd.setCursor(0, 1);
    lcd.print(currentData.temperature);
    lcd.print(" Celsius");
    break;
  case '2': // caso para mostrar sensor de CO2
    lcd.setCursor(0, 0);
    lcd.print("Nivel CO2:");
    lcd.setCursor(0, 1);
    lcd.print(currentData.co2Level);
    lcd.print("ppm");
    break;
  case '3': // Caso para mostrar sensor de ilumiación fotocelda
    lcd.setCursor(0, 0);
    lcd.print("Lumenes:");
    lcd.setCursor(8, 0);
    lcd.print(nivelLuz(currentData.lightLevel));
    lcd.setCursor(0, 1);
    lcd.print(currentData.lightLevel);
    lcd.print(" ");
    lcd.print(nivelLuz(currentData.lightLevel));
    lcd.print(" Lum");
    break;
  case '4': // caso para mostrar sensor de humedad
    lcd.setCursor(0, 0);
    lcd.print("Humedad:");
    lcd.setCursor(0, 1);
    lcd.print(currentData.humidity);
    lcd.print("%");
    break;
  case '5': // caso para mostrar sensor de distancia
    lcd.setCursor(0, 0);
    lcd.print("Distancia:");
    currentData.distance = readUltrasonicSensor();
    lcd.setCursor(0, 1);
    lcd.print(currentData.distance);
    lcd.setCursor(8, 1);
    lcd.print(currentData.distance < 20 ? "Presente" : "Ausente ");
    break;
  case '6': // caso para guardar la información a memoria
    saveDataToEEPROM(currentData);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Save data");
    break;
  case '7': // caso para mostrar información guardada en memoria
    displaySavedData();
    break;
  default:
    printAllSensor();
    break;
  }
}

void printAllSensor() {
  lcd.setCursor(0, 0);
  lcd.print("A:");
  lcd.setCursor(7, 0);
  lcd.print("T:");
  lcd.setCursor(12, 0);
  lcd.print("H:");
  lcd.setCursor(0, 1);
  lcd.print("I:");
  lcd.setCursor(7, 1);
  lcd.print("D:");
  lcd.setCursor(2, 0);
  lcd.print(currentData.co2Level);
  lcd.setCursor(9, 0);
  lcd.print(currentData.temperature);
  lcd.setCursor(14, 0);
  lcd.print(currentData.humidity);
  lcd.setCursor(2, 1);
  lcd.print(currentData.lightLevel);
  lcd.setCursor(8, 1);
  lcd.print(readUltrasonicSensor());
}

String nivelLuz(int valor) {
  if (valor <= 100)
    return " Alta";
  if (valor > 900)
    return " Baja";
  return " Media";
}

void saveDataToEEPROM(SensorData data) {
  EEPROM.put(eepromAddress, data);
}

void displaySavedData() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Read data:");
  SensorData tempData;
  EEPROM.get(0, tempData);
  lcd.setCursor(0, 0);
  lcd.print("A:");
  lcd.setCursor(7, 0);
  lcd.print("T:");
  lcd.setCursor(12, 0);
  lcd.print("H:");
  lcd.setCursor(0, 1);
  lcd.print("I:");
  lcd.setCursor(7, 1);
  lcd.print("D:");
  lcd.setCursor(2, 0);
  lcd.print(tempData.co2Level);
  lcd.setCursor(9, 0);
  lcd.print(tempData.temperature);
  lcd.setCursor(14, 0);
  lcd.print(tempData.humidity);
  lcd.setCursor(2, 1);
  lcd.print(tempData.lightLevel);
  lcd.setCursor(8, 1);
  lcd.print(tempData.distance);
}