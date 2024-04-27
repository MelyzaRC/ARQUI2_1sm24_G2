#include <Wire.h>
#include <DHT.h>
#include <EEPROM.h>
#include <Servo.h>

#define DHTPIN A2
#define DHTTYPE DHT11
#define TRIGGER_PIN 5
#define ECHO_PIN 6
#define CO2_PIN A0
#define LIGHT_PIN A1
#define FOCO_PIN A3
#define INTERVALO_LECTURA_SENSORES 4000

#define SERVO_PIN 7
#define ENTRADA1_PIN 8
#define ENTRADA2_PIN 9
#define ENABLE_A_PIN 10
#define PULSO_MINIMO 580
#define PULSO_MAXIMO 2500

unsigned long tiempoInicioIluminacion = 0;
bool cicloIluminacionActivo = false;

DHT dht(DHTPIN, DHTTYPE);
Servo servo1;

struct SensorData {
  float temperature;
  float humidity;
  int co2Level;
  int lightLevel;
  int distance;
};

SensorData currentData;

void setup() {
  pinMode(FOCO_PIN, OUTPUT);
  servo1.attach(SERVO_PIN, PULSO_MINIMO, PULSO_MAXIMO);
  servo1.write(180);
  pinMode(ENTRADA1_PIN, OUTPUT);
  pinMode(ENTRADA2_PIN, OUTPUT);
  pinMode(ENABLE_A_PIN, OUTPUT);
  digitalWrite(FOCO_PIN, HIGH);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(CO2_PIN, INPUT);
  pinMode(LIGHT_PIN, INPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  if (Serial.available() > 0) {
    delay(10);
    String message = Serial.readStringUntil('\n');
    accionesIndividuales(message);   
  }
  readSensors();
  verificarIluminacion();
  verificarCalidadAire();
  sendSensorDataToESP32();
  delay(1000);
}

void verificarIluminacion() {
  // Verificar si la iluminación está activada y la distancia es menor a 13
  if (digitalRead(FOCO_PIN) == LOW && currentData.distance > 13) { 
    if (!cicloIluminacionActivo) {
      // Primer ciclo: Iniciar temporizador y enviar notificación después de 30 segundos
      tiempoInicioIluminacion = millis();
      cicloIluminacionActivo = true;
    } else {
      if (millis() - tiempoInicioIluminacion > 30000) { // 30000 milisegundos = 30 segundos
        Serial.println("#HabitacionON:");
        // Segundo ciclo: Apagar la iluminación después de 30 segundos adicionales
        if (millis() - tiempoInicioIluminacion > 60000) { // 60000 milisegundos = 60 segundos
          Serial.println("#HabitacionOFF:");
          digitalWrite(FOCO_PIN, HIGH); // Apagar la luz
          cicloIluminacionActivo = false;
        }
      }
    }
  } else { // Foco apagado o distancia mayor a 13
    cicloIluminacionActivo = false; // Reiniciar el ciclo si la iluminación se apaga o la distancia es mayor a 13
  }
}

void verificarCalidadAire() {
  static unsigned long tiempoInicioCO2 = 0;
  static bool cicloCO2Activo = false;
  static bool ventilacionActiva = false;

  // Verificar si la calidad del aire no es óptima (CO2 mayor a 29)
  if (currentData.co2Level > 29) {
    if (!cicloCO2Activo) {
      // Primer ciclo: Notificar calidad del aire deficiente después de 20-30 segundos
      tiempoInicioCO2 = millis();
      cicloCO2Activo = true;
    } else {
      unsigned long tiempoActual = millis();
      if (tiempoActual - tiempoInicioCO2 > 20000 && tiempoActual - tiempoInicioCO2 < 30000) { // Primer ciclo de 20-30 segundos
        Serial.println("#HabitacionCO2High:");
      } else if (tiempoActual - tiempoInicioCO2 > 30000) { // Segundo ciclo de 30 segundos
        if (!ventilacionActiva) {
          Serial.println("#HabitacionCO2Active:");
          // Activar sistema de ventilación
          motorControl(128, HIGH, LOW); // Accionar el motor para ingresar aire limpio
          ventilacionActiva = true;
        } else {
          if (currentData.co2Level <= 29) { // Verificar si el nivel de CO2 vuelve a ser óptimo
            motorApagar(); // Detener el motor
            Serial.println("#HabitacionCO2Normal:");
            cicloCO2Activo = false;
            ventilacionActiva = false;
          } else if (tiempoActual - tiempoInicioCO2 > 40000) { // 10 segundos después del inicio del ciclo de ventilación
            motorApagar(); // Detener el motor
            Serial.println("#HabitacionCO2Normal:");
            cicloCO2Activo = false;
            ventilacionActiva = false;
          }
        }
      }
    }
  } else {
    cicloCO2Activo = false; // Reiniciar el ciclo si la calidad del aire vuelve a ser óptima
    ventilacionActiva = false; // Reiniciar el estado de ventilación
  }
}


void readSensors() {
  currentData.temperature = dht.readTemperature();
  currentData.humidity = dht.readHumidity();
  currentData.co2Level = analogRead(CO2_PIN);
  currentData.lightLevel = analogRead(LIGHT_PIN);
  currentData.distance = readUltrasonicSensor();
  // Verificar si la distancia es menor que 13
  if (currentData.distance < 13) {
    Serial.print("#Objeto:");
    Serial.println(currentData.distance);
  }
}

int readUltrasonicSensor() {
  long duration, distance;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration / 2 / 29.1;
  
  return distance;
}

void sendSensorDataToESP32() {
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
  if (key == "Temp") {
    Serial.print("#Temperatura:");
    Serial.println(currentData.temperature);
  } else if (key == "GAS") {
    Serial.print("#CO2:");
    Serial.println(currentData.co2Level);
  } else if (key == "Luz") {
    Serial.print("#Lumenes:");
    Serial.println(currentData.lightLevel);
  } else if (key == "Hum") {
    Serial.print("#Humedad:");
    Serial.println(currentData.humidity);
  } else if (key == "Dist") {
    Serial.print("#Distancia:");
    Serial.println(currentData.distance);
  } else if (key == "EncenderFoco") {
    Serial.println("Foco encendido");
    digitalWrite(FOCO_PIN, LOW); // Encender el foco
  } else if (key == "ApagarFoco") {
    Serial.println("Foco apagado");
    digitalWrite(FOCO_PIN, HIGH); // Apagar el foco
  } else if (key == "AbrirPuerta") {
    Serial.println("#AbrirP");
    abrirPuerta();
  } else if (key == "CerrarPuerta") {
    Serial.println("#CerrarP");
    cerrarPuerta();
  } else if (key == "ventiladorMax") {
    Serial.println("#vMaximo");
    motorControl(128, HIGH, LOW);
  } else if (key == "ventiladorMed") {
    Serial.println("#vMedia");
    motorControl(64, HIGH, LOW);
  } else if (key == "ventiladorOff") {
    Serial.println("#ApagarVentilacion");
    motorApagar();
  } else if (key == "EEP") { 
    saveDataToEEPROM(currentData);
    Serial.print("#EEPROM");
    Serial.println(" Guardada con exito");
  } else if (key == "READ") { 
    displaySavedData();
  } else {
    // Si la clave no coincide con ninguna acción específica, no hacer nada
  }
}


void abrirPuerta() {
  for (int angulo = 180; angulo >= 0; angulo--) {
    servo1.write(angulo);
    delay(15);
  }
}

void cerrarPuerta() {
  for (int angulo = 0; angulo <= 180; angulo++) {
    servo1.write(angulo);
    delay(15);
  }
}

void saveDataToEEPROM(SensorData data) {
  EEPROM.put(0, data);
}

void displaySavedData() {
  SensorData tempData;
  EEPROM.get(0, tempData);

  Serial.print("#BEGIN_EEPROM");
  Serial.print(tempData.co2Level);
  Serial.print(",");
  Serial.print(tempData.temperature);
  Serial.print(",");
  Serial.print(tempData.humidity);
  Serial.print(",");
  Serial.print(tempData.lightLevel);
  Serial.print(",");
  Serial.println(tempData.distance);
}

void motorControl(int velocidad, int pin1State, int pin2State) {
  digitalWrite(ENTRADA1_PIN, pin1State);
  digitalWrite(ENTRADA2_PIN, pin2State);
  analogWrite(ENABLE_A_PIN, velocidad);
}

void motorApagar() {
  motorControl(0, LOW, LOW);
}
