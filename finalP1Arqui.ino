#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
#include <Keypad.h>
#include <DHT.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27,16,2); // LCD 2X16

#define DHTPIN 9     // Pin sensor DHT22
#define DHTTYPE DHT11   // Tipo Sensor DHT 22
DHT dht(DHTPIN, DHTTYPE); //// Inicializando sensor DHT
#define trigger 13  //
#define echo 14
// Dirección de memoria EEPROM para almacenar los datos de los sensores
int eepromAddress = 0;
int pinButtonCO2 = 10; // C02
int pinButtonFoto = 11; //Fotoresistencia
int pinHumedad = 12; // Humedad

/////


const byte ROWS = 3;// Filas
const byte COLS = 2;//Columnas
char keys[ROWS][COLS] = {//Simbolos
  {'1','2'},
  {'3','4'},
  {'5','6'}
};
byte rowPins[ROWS] = {6, 7, 8}; 
byte colPins[COLS] = {5, 4}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


// Struct para guardar información de los sensores
struct SensorData {
  int temperature;
  int humidity;
  int co2Level;
  int lightLevel;
  int distance;
};

SensorData currentData;


void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SENSOR CLIMATICO ");
  lcd.setCursor(0,1);
  lcd.print("  GRUPO 2  ");
  
  dht.begin();
  pinMode(pinButtonCO2,     INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  readSensors();
  char key = keypad.getKey();
  if (key != NO_KEY) {
    processKeypad(key);
  }

}

void readSensors(){
  currentData.temperature = dht.readTemperature();
  currentData.humidity = analogRead(pinHumedad);
  currentData.co2Level = analogRead(pinButtonCO2);
  currentData.lightLevel = analogRead(pinButtonFoto);
  currentData.distance = readUltrasonicSensor();
}

int readUltrasonicSensor() {
  long duracion, distancia;
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  duracion = pulseIn(echo, HIGH);
  distancia = duracion / 2 / 29.1;
  return distancia;
}

void processKeypad(char key) {
  switch (key) {
    case '1':  // caso para mostrar sensor de temperatura
      lcd.setCursor(0, 0); lcd.print("Temperatura:"); lcd.setCursor(0,1);
      lcd.print(currentData.temperature); lcd.print("° Celsius");
      break;
    case '2': // caso para mostrar sensor de CO2
      lcd.clear(); lcd.setCursor(0, 0); lcd.print("Nivel CO2:");
      lcd.setCursor(0,1); lcd.print(currentData.co2Level); lcd.print("ppm");
      break;
    case '3': // Caso para mostrar sensor de ilumiación fotocelda
      lcd.clear(); lcd.setCursor(0, 0); lcd.print("Iluminacion:");
      lcd.setCursor(0,1); lcd.print(currentData.lightLevel);
      lcd.print(nivelLuz(currentData.lightLevel)); lcd.print(" Lum");
      break;
    case '4': // caso para mostrar sensor de humedad
      lcd.clear(); lcd.setCursor(0,0); lcd.print("Humedad:");
      lcd.setCursor(0,1); lcd.print(currentData.humidity); lcd.print("%");
      break;
    case '5': // caso para mostrar sensor de distancia
      lcd.clear(); lcd.setCursor(0, 0); lcd.print("Distancia:");
      lcd.setCursor(0, 1);  lcd.print(currentData.distance);
      lcd.setCursor(8, 1); lcd.print(currentData.distance < 15 ? "Presente" : "Ausente ");
      break;
    case 6: // caso para guardar la información a memoria
      saveDataToEEPROM(currentData);
      lcd.clear(); lcd.setCursor(0, 0); lcd.print("Información guardada con exito:");
      lcd.scrollDisplayLeft();
      break;
    case 7: // caso para mostrar información guardada en memoria
      displaySavedData();
      break;
    default:
      printAllSensor();
      break;
  }
}

void printAllSensor(){
      lcd.setCursor(0, 0); lcd.print("G:");
      lcd.setCursor(7, 0); lcd.print("T:");
      lcd.setCursor(12, 0); lcd.print("H:");
      lcd.setCursor(0, 1); lcd.print("I:");
      lcd.setCursor(7, 1); lcd.print("D:");
      lcd.setCursor(2, 0); lcd.print(currentData.co2Level);
      lcd.setCursor(9, 0);  lcd.print(currentData.temperature);
      lcd.setCursor(14, 0); lcd.print(currentData.humidity);
      lcd.setCursor(2, 1); lcd.print(currentData.lightLevel);
      lcd.setCursor(8, 1); lcd.print(currentData.distance);
      lcd.scrollDisplayLeft();
}


String nivelLuz(int valor)
{
  if (valor <= 100) return " Alta";
  if (valor > 900) return " Baja";
  return " Media";
}

void saveDataToEEPROM(SensorData data) {
  EEPROM.put(eepromAddress, data);
}


void displaySavedData() {
  // Lee los valores almacenados en la EEPROM
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Datos almacenados:");
    SensorData tempData;
    EEPROM.get(0, tempData);
    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.print(tempData.temperature);
    lcd.print("C H:");
    lcd.print(tempData.humidity);
    lcd.print("% CO2:");
    lcd.print(tempData.co2Level);
    lcd.print("ppm");
    lcd.print(" L:");
    lcd.print(tempData.lightLevel);
    lcd.print(" D:");
    lcd.print(tempData.distance);
    lcd.scrollDisplayLeft();
    delay(2000);
    lcd.clear();
}
