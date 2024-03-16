#include <PubSubClient.h>
#include <WiFi.h>


const char* ssid = "Tech_D0021920";
const char* password = "HUVKFFUR";
const char* mqtt_server = "3.138.175.17"; // Dirección IP o nombre de dominio de tu broker MQTT
const int mqtt_port = 1883; // Puerto MQTT
const char* mqtt_topic = "allSensors";
const char* mqtt_topic_temp = "temperatura";
const char* mqtt_topic_co2 = "co2";
const char* mqtt_topic_humidity = "humedad";
const char* mqtt_topic_distance = "distancia";
const char* mqtt_topic_luz = "luz";
const char* mqtt_topic_motor = "motor";
const char* mqtt_topic_memoria = "memoria";

WiFiClient espClient;
PubSubClient client(espClient);

void setup();
void loop();
void setup_wifi();
void reconnect();
void callback(char* topic, byte* message, unsigned int length);

String message = "";
int numeros[5];

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  // Suscripción a los tópicos
  client.subscribe(mqtt_topic);
  client.subscribe(mqtt_topic_temp);
  client.subscribe(mqtt_topic_co2);
  client.subscribe(mqtt_topic_humidity);
  client.subscribe(mqtt_topic_distance);
  client.subscribe(mqtt_topic_luz);
  client.subscribe(mqtt_topic_motor);
  client.subscribe(mqtt_topic_memoria);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  // Esperar a recibir información serial del Arduino Mega
  if (Serial.available() > 0) {
    // Leer el mensaje del Arduino Mega
    String message = Serial.readStringUntil('\n');
    processSpecialMessages(message);
    
  }
}

void processSpecialMessages(String message) {
  if (message.startsWith("#BEGIN_EEPROM")) {
    String data = message.substring(13); 
    client.publish(mqtt_topic_memoria, data.c_str()); 
  } else if (message.startsWith("#EEPROM")) {
    Serial.println("Mensaje especial recibido: EEPROM");
  } else if (message.startsWith("#Distancia:")) {
    String datoDis = message.substring(11); 
    client.publish(mqtt_topic_distance, datoDis.c_str()); 
  } else if (message.startsWith("#Humedad:")) {
    String datoHum = message.substring(9); 
    client.publish(mqtt_topic_humidity, datoHum.c_str()); 
  } else if (message.startsWith("#Lumenes:")) {
    String datolum = message.substring(9); 
    client.publish(mqtt_topic_luz, datolum.c_str());
  } else if (message.startsWith("#CO2:")) {
    String dataco2 = message.substring(5); 
    client.publish(mqtt_topic_co2, dataco2.c_str()); 
  } else if (message.startsWith("#Temperatura:")) {
    String datatemp = message.substring(13); 
    client.publish(mqtt_topic_temp, datatemp.c_str()); 
  } else if (message.startsWith("#Todos")) {
    String data = message.substring(6); 
    client.publish(mqtt_topic, data.c_str()); 
  } else if (message.startsWith("#Motor:")) {
    //String data = message.substring(7); 
    //client.publish(mqtt_topic_motor, data.c_str()); 
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado a la red WiFi");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    if (client.connect("ESP32Client", "usermqttserver", "mqttg2")) {
      
      client.subscribe(mqtt_topic);
      client.subscribe(mqtt_topic_temp);
      client.subscribe(mqtt_topic_co2);
      client.subscribe(mqtt_topic_humidity);
      client.subscribe(mqtt_topic_distance);
      client.subscribe(mqtt_topic_luz);
      client.subscribe(mqtt_topic_motor);
      client.subscribe(mqtt_topic_memoria);
      Serial.println("Conectado");
    } else {
      Serial.print("Error de conexión, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  if (strcmp(topic, mqtt_topic_temp) == 0) {
    // Acción para la lectura de temperatura
    Serial.print("Temp");
  } else if (strcmp(topic, mqtt_topic_co2) == 0) {
    // Acción para la lectura de CO2
    Serial.print("CO2");
  } else if (strcmp(topic, mqtt_topic_humidity) == 0) {
    Serial.print("Hum");
  } else if (strcmp(topic, mqtt_topic_distance) == 0) {
    Serial.print("Dist");
  }else if (strcmp(topic, mqtt_topic) == 0) {
     //Serial.print("All");
  }else if (strcmp(topic, mqtt_topic_luz) == 0) {
     Serial.print("Luz");
  }else if (strcmp(topic, mqtt_topic_motor) == 0) {
     String cad = String((char*)message);
     String subString = substringWithLength(cad, 0, length);
     Serial.println(subString);
  }else if (strcmp(topic, mqtt_topic_memoria) == 0) {
     String cad = String((char*)message);
     String subString = substringWithLength(cad, 0, length);
     Serial.println(subString);
  }
  
}

String substringWithLength(String str, int startIndex, int length) {
    String result = "";

    // Verificar si el índice inicial está dentro de los límites de la cadena
    if (startIndex < 0 || startIndex >= str.length()) {
        return result; // Devolver cadena vacía si el índice está fuera de los límites
    }

    // Iterar sobre la cadena y agregar caracteres a la subcadena
    for (int i = startIndex; i < startIndex + length && i < str.length(); i++) {
        result += str.charAt(i);
    }

    return result;
}
