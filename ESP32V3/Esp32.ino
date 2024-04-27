#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>

const char* ssid = "Tech_D0021920";
const char* password = "HUVKFFUR";
const char* mqtt_server = "3.138.175.17";
const int   mqtt_port = 1883; // Puerto MQTT
const char* mqtt_topic = "allSensors";
const char* mqtt_topic_temp = "temperatura";
const char* mqtt_topic_co2 = "gasco2";
const char* mqtt_topic_humidity = "humedad";
const char* mqtt_topic_distance = "distancia";
const char* mqtt_topic_lumus = "luminosidad";
const char* mqtt_topic_guardar = "guardar";
const char* mqtt_topic_leer = "leer";
const char* mqtt_topic_focoOn = "focoON";
const char* mqtt_topic_focoOff = "focoOFF";
const char* mqtt_topic_puertaAbierta = "Abierta";
const char* mqtt_topic_puertaCerrada = "Cerrada";
const char* mqtt_topic_ventiladorMax = "ventiladorVelocidadMaxima";
const char* mqtt_topic_ventiladorMed = "ventiladorVelocidadMedia";
const char* mqtt_topic_ventiladorOff = "ventilacionOFF";
const char* mqtt_topic_Objeto = "hayObjeto";
WiFiClient espClient;
PubSubClient client(espClient);
HTTPClient http;

void setup();
void loop();
void setup_wifi();
void reconnect();
void callback(char* topic, byte* message, unsigned int length);

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  // Suscripción a los tópicos
  client.subscribe(mqtt_topic);
  client.subscribe(mqtt_topic_temp);
  client.subscribe(mqtt_topic_co2);
  client.subscribe(mqtt_topic_humidity);
  client.subscribe(mqtt_topic_distance);
  client.subscribe(mqtt_topic_ventiladorOff);
  client.subscribe(mqtt_topic_lumus);
  client.subscribe(mqtt_topic_guardar);
  client.subscribe(mqtt_topic_leer);
  client.subscribe(mqtt_topic_focoOn);
  client.subscribe(mqtt_topic_focoOff);
  client.subscribe(mqtt_topic_puertaAbierta);
  client.subscribe(mqtt_topic_puertaCerrada);
  client.subscribe(mqtt_topic_ventiladorMax);
  client.subscribe(mqtt_topic_ventiladorMed);
  client.subscribe(mqtt_topic_Objeto);
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
     String dataRead = message.substring(13); // Ignora los primeros 6 caracteres (#Todos)
    client.publish(mqtt_topic_leer, dataRead.c_str()); // Publicar los datos en el topic general
  
  } else if (message.startsWith("#EEPROM")) {
    String dataSave = "Guardada con exito";
    client.publish(mqtt_topic_guardar,dataSave.c_str());

  } else if (message.startsWith("#Distancia:")) {
    
    String datoDis = message.substring(11); 
    client.publish(mqtt_topic_distance, datoDis.c_str()); 
    api(5,datoDis);

  }else if (message.startsWith("#Objeto:")) {
    
     String datahum = message.substring(8); // Ignora los primeros 13 caracteres (#Temperatura:)
     String envio = "Hay un objeto";
    client.publish(mqtt_topic_Objeto, envio.c_str()); 
  

  }else if (message.startsWith("#HabitacionON:")) {
    
     String datahum = message.substring(8); // Ignora los primeros 13 caracteres (#Temperatura:)
     String envio = "La habitación está iluminada pero no hay presencia humana";
    client.publish(mqtt_topic_Objeto, envio.c_str()); 
  

  }else if (message.startsWith("#HabitacionOFF:")) {
    
     String datahum = message.substring(8); // Ignora los primeros 13 caracteres (#Temperatura:)
     String envio = "El sistema de iluminación ha sido apagado";
    client.publish(mqtt_topic_Objeto, envio.c_str()); 
  

  }else if (message.startsWith("#HabitacionCO2High:")) {
    
     String envio = "La habitación cuenta con una calidad de aire deficiente";
    client.publish(mqtt_topic_Objeto, envio.c_str()); 
  

  }else if (message.startsWith("#HabitacionCO2Active:")) {
    
     String envio = "El sistema procederá a realizar la limpieza del aire de la habitación";
    client.publish(mqtt_topic_Objeto, envio.c_str()); 
  

  }else if (message.startsWith("#HabitacionCO2Normal:")) {
    
     String envio = "La habitación posee una calidad del aire óptima";
    client.publish(mqtt_topic_Objeto, envio.c_str()); 
  

  } else if (message.startsWith("#Humedad:")) {
    String datahum = message.substring(9); // Ignora los primeros 13 caracteres (#Temperatura:)
    client.publish(mqtt_topic_humidity, datahum.c_str()); // Publicar los datos en el topic general
    api(2,datahum);
  } else if (message.startsWith("#Lumenes:")) {
     String dataluz = message.substring(9); // Ignora los primeros 13 caracteres (#Temperatura:)
    client.publish(mqtt_topic_lumus, dataluz.c_str()); // Publicar los datos en el topic general
    api(4,dataluz);
  } else if (message.startsWith("#CO2:")) {
    String dataco2 = message.substring(5); // Ignora los primeros 13 caracteres (#Temperatura:)
    client.publish(mqtt_topic_co2, dataco2.c_str()); // Publicar los datos en el topic general
    api(3,dataco2);
  } else if (message.startsWith("#Temperatura:")) {
    String datatemp = message.substring(13); // Ignora los primeros 13 caracteres (#Temperatura:)
    client.publish(mqtt_topic_temp, datatemp.c_str()); // Publicar los datos en el topic general
    api(1,datatemp);
  } else if (message.startsWith("#Todos")) {
    String data = message.substring(6); // Ignora los primeros 6 caracteres (#Todos)
    client.publish(mqtt_topic, data.c_str()); // Publicar los datos en el topic general
    apiAllSensores(data);
  }
}

/***************************/
void apiAllSensores(String valores){
    int i = 0;
    char *ptr;
    int datos[5];
    char cadenaBuffer[20];
    valores.toCharArray(cadenaBuffer, sizeof(cadenaBuffer));

    ptr = strtok(cadenaBuffer, ",");
    while(ptr != NULL) {
      datos[i] = atoi(ptr); // Convertir el token a entero y almacenarlo en el array datos
      ptr = strtok(NULL, ",");
      i++;
    }
    //0temp, 1hum, 2co2, 3luz, 4dis

    api(1,String(datos[0]));
    api(2,String(datos[1]));
    api(3,String(datos[2]));
    api(4,String(datos[3]));
    api(5,String(datos[4]));
}

void api(int sensor, String val){
  String serverUrl = "";
  String json = "";

if(sensor == 1) { 
  serverUrl = "http://3.147.69.159:3010/addTemperatura";
  json = "{\"temperatura\":" + String(val) + "}";
}
if(sensor == 2) { 
  serverUrl = "http://3.147.69.159:3010/addHumedad";
  json = "{\"humedad\":" + String(val) + "}";
}
if(sensor == 3) { 
  serverUrl = "http://3.147.69.159:3010/addAire";
  json = "{\"aire\":" + String(val) + "}";
}
if(sensor == 4) { 
  serverUrl = "http://3.147.69.159:3010/addIluminacion";
  json = "{\"iluminacion\":" + String(val) + "}";
}
if(sensor == 5) { 
  serverUrl = "http://3.147.69.159:3010/addDistancia";
  json = "{\"distancia\":" + String(val) + "}";
}

  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(json);
/*
  if (httpResponseCode > 0) {
    String respuesta = http.getString();
    Serial.println("-->"+serverUrl+"<--");
    Serial.println(json);
    Serial.println(httpResponseCode);
    Serial.println(respuesta);
  } else {
    Serial.print("Error en la solicitud HTTP: ");
    Serial.println(httpResponseCode);
  }*/

  http.end();
}



/**************************/


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
      client.subscribe(mqtt_topic_ventiladorOff);
      client.subscribe(mqtt_topic_lumus);
      client.subscribe(mqtt_topic_guardar);
      client.subscribe(mqtt_topic_leer);
      client.subscribe(mqtt_topic_focoOn);
      client.subscribe(mqtt_topic_focoOff);
      client.subscribe(mqtt_topic_puertaAbierta);
      client.subscribe(mqtt_topic_puertaCerrada);
      client.subscribe(mqtt_topic_ventiladorMax);
      client.subscribe(mqtt_topic_ventiladorMed);
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
    Serial.print("Temp");
  } else if (strcmp(topic, mqtt_topic_co2) == 0) {
    Serial.print("GAS");
  } else if (strcmp(topic, mqtt_topic_humidity) == 0) {
    Serial.print("Hum");
  } else if (strcmp(topic, mqtt_topic_distance) == 0) {
    Serial.print("Dist");
  }else if (strcmp(topic, mqtt_topic) == 0) {
  }else if (strcmp(topic, mqtt_topic_focoOn) == 0) {
    Serial.print("EncenderFoco");
  }else if (strcmp(topic, mqtt_topic_focoOff) == 0) {
    Serial.print("ApagarFoco");
  }else if (strcmp(topic, mqtt_topic_lumus) == 0) {
    Serial.print("Luz");
  }else if (strcmp(topic, mqtt_topic_puertaAbierta) == 0) {
    Serial.print("AbrirPuerta");
  }else if (strcmp(topic, mqtt_topic_puertaCerrada) == 0) {
    Serial.print("CerrarPuerta");
  }else if (strcmp(topic, mqtt_topic_ventiladorMax) == 0) {
    Serial.print("ventiladorMax");
  }else if (strcmp(topic, mqtt_topic_ventiladorMed) == 0) {
    Serial.print("ventiladorMed");
  }else if (strcmp(topic, mqtt_topic_ventiladorOff) == 0) {
    Serial.print("ventiladorOff");
  } else if (strcmp(topic, mqtt_topic_guardar) == 0) {
    Serial.print("EEP");
  }
  else if (strcmp(topic, mqtt_topic_leer) == 0) {
    Serial.print("READ");
  }
}

