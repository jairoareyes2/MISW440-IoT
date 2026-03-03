#include <Wire.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <PubSubClient.h>
#include <DHT.h>

 
// Definiciones

// Pin del sensor de temperatura y humedad
#define DHTPIN 14
// Tipo de sensor de temperatura y humedad
#define DHTTYPE DHT11
// Intervalo en segundo de las mediciones
#define MEASURE_INTERVAL 2
// Duración aproximada en la pantalla de las alertas que se reciban
#define ALERT_DURATION 60

#define CRITICAL_DURATION 60

#define CRITICAL_PIN 12

// Display
// SH1106 128x64 I2C
U8G2_SH1106_128X64_NONAME_F_HW_I2C display(
  U8G2_R0,
  U8X8_PIN_NONE
);


// Conexión a Wifi
// Nombre de la red Wifi
const char ssid[] = "";
// Contraseña de la red Wifi
const char pass[] = "";


// Declaraciones

// Sensor DHT
DHT dht(DHTPIN, DHTTYPE);
// Cliente WiFi
WiFiClient net;
// Cliente MQTT
PubSubClient client(net);

//Conexión a Mosquitto
#define USER "" // TODO Reemplace UsuarioMQTT por un usuario (no administrador) que haya creado en la configuración del bróker de MQTT.
const char MQTT_HOST[] = ""; // TODO Reemplace ip.maquina.mqtt por la IP del bróker MQTT que usted desplegó. Ej: 192.168.0.1
const int MQTT_PORT = 8082;
const char MQTT_USER[] = USER;
//Contraseña de MQTT
const char MQTT_PASS[] = ""; // TODO Reemplace ContrasenaMQTT por la contraseña correpondiente al usuario especificado.

//Tópico al que se recibirán los datos
// El tópico de publicación debe tener estructura: <país>/<estado>/<ciudad>/<usuario>/out
const char MQTT_TOPIC_PUB[] = "colombia/cundinamarca/bogota/" USER "/out"; //TODO Reemplace el valor por el tópico de publicación que le corresponde.
// El tópico de suscripción debe tener estructura: <país>/<estado>/<ciudad>/<usuario>/in
const char MQTT_TOPIC_SUB[] = "colombia/cundinamarca/bogota/" USER "/in"; //TODO Reemplace el valor por el tópico de suscripción que le corresponde.
const char MQTT_TOPIC_SUB_CRITICAL[] = "colombia/cundinamarca/bogota/" USER "/in_critical"; //TODO Reemplace el valor por el tópico de suscripción que le corresponde.

// Declaración de variables globales

// Timestamp de la fecha actual.
time_t now;
// Tiempo de la última medición
long long int measureTime = millis();
// Tiempo en que inició la última alerta
long long int alertTime = millis();

long long int criticalTime = millis();
// Mensaje para mostrar en la pantalla
String alert = "";
// Valor de la medición de temperatura
float temp;
// Valor de la medición de la humedad
float humi;

bool isCritical = false;

/**
 * Conecta el dispositivo con el bróker MQTT usando
 * las credenciales establecidas.
 * Si ocurre un error lo imprime en la consola.
 */
void mqtt_connect()
{
  //Intenta realizar la conexión indefinidamente hasta que lo logre
  while (!client.connected()) {
    
    Serial.print("MQTT connecting ... ");
    
    if (client.connect(MQTT_USER, MQTT_USER, MQTT_PASS)) {
      
      Serial.println("connected.");
      client.subscribe(MQTT_TOPIC_SUB);
      client.subscribe(MQTT_TOPIC_SUB_CRITICAL);
      
    } else {
      
      Serial.println("Problema con la conexión, revise los valores de las constantes MQTT");
      int state = client.state();
      Serial.print("Código de error = ");
      alert = "MQTT error: " + String(state);
      Serial.println(state);
      
      if ( client.state() == MQTT_CONNECT_UNAUTHORIZED ) {
        ESP.deepSleep(0);
      }
      
      // Espera 5 segundos antes de volver a intentar
      delay(5000);
    }
  }
}

/**
 * Publica la temperatura y humedad dadas al tópico configurado usando el cliente MQTT.
 */
void sendSensorData(float temperatura, float humedad) {
  String data = "{";
  data += "\"temperatura\": "+ String(temperatura, 1) +", ";
  // data += "\"luminosidad\": 100, ";
  data += "\"humedad\": "+ String(humedad, 1);
  data += "}";
  char payload[data.length()+1];
  data.toCharArray(payload,data.length()+1);
  
  client.publish(MQTT_TOPIC_PUB, payload);
}

/**
 * Lee la temperatura del sensor DHT, la imprime en consola y la devuelve.
 */
float readTemperatura() {
  
  // Se lee la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" *C ");
  
  return t;
}

/**
 * Lee la humedad del sensor DHT, la imprime en consola y la devuelve.
 */
float readHumedad() {
  // Se lee la humedad relativa
  float h = dht.readHumidity();
  
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.println(" %\t");

  return h;
}

/**
 * Verifica si las variables ingresadas son números válidos.
 * Si no son números válidos, se imprime un mensaje en consola.
 */
bool checkMeasures(float t, float h) {
  // Se comprueba si ha habido algún error en la lectura
    if (isnan(t) || isnan(h)) {
      Serial.println("Error obteniendo los datos del sensor DHT11");
      return false;
    }
    return true;
}

void startDisplay() {
  Wire.begin(D2, D1);  // SDA, SCL (NodeMCU)
  display.begin();

  display.clearBuffer();
  display.setFont(u8g2_font_ncenB08_tr);

}

void displayNoSignal() {
  display.clearBuffer();

  display.setFont(u8g2_font_ncenB14_tr);
  display.drawStr(10,30,"No hay");
  display.drawStr(10,55,"senal");

  display.sendBuffer();
}

void displayHeader() {

  char header[40];

  long long int milli = now + millis() / 1000;
  struct tm* tinfo = localtime(&milli);

  char hour[10];
  strftime(hour, sizeof(hour), "%H:%M:%S", tinfo);

  sprintf(header, "IOT Sensors %s", hour);

  display.setFont(u8g2_font_6x10_tr);
  display.drawStr(0,10, header);
}

void displayMeasures() {

  char buffer[40];

  sprintf(buffer, "T: %.1f  H: %.1f", temp, humi);

  display.setFont(u8g2_font_6x12_tr);
  display.drawStr(0,30, buffer);
}

void displayMessage(const char* message) {

  display.setFont(u8g2_font_6x10_tr);
  display.drawStr(0,45,"Msg:");

  display.setFont(u8g2_font_ncenB14_tr);

  if (strcmp(message, "OK") == 0) {
    display.drawStr(40,63,"OK");  // centrado aproximado
  } else {
    display.drawStr(0,63,message);
  }
}

void displayConnecting(const char* ssid) {

  display.clearBuffer();

  display.setFont(u8g2_font_6x10_tr);
  display.drawStr(0,15,"Connecting to:");

  display.drawStr(0,35, ssid);

  display.sendBuffer();
}

/**
 * Verifica si ha llegdo alguna alerta al dispositivo.
 * Si no ha llegado devuelve OK, de lo contrario retorna la alerta.
 * También asigna el tiempo en el que se dispara la alerta.
 */
String checkAlert() {
  String message = "OK";
  
  if (alert.length() != 0) {
    message = alert;
    if ((millis() - alertTime) >= ALERT_DURATION * 1000 ) {
      alert = "";
      alertTime = millis();
     }
  }
  return message;
}

/**
 * Función que se ejecuta cuando llega un mensaje a la suscripción MQTT.
 * Construye el mensaje que llegó y si contiene ALERT lo asgina a la variable 
 * alert que es la que se lee para mostrar los mensajes.
 */
void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  String data = "";
  for (unsigned int i = 0; i < length; i++) {
    data += String((char)payload[i]);
  }
  Serial.print(data);
  if (data.indexOf("ALERT") >= 0) {
    alert = data;
  }

  if (data.indexOf("CRITICAL") >= 0) 
  {
    isCritical=true;
  }
}

/**
 * Verifica si el dispositivo está conectado al WiFi.
 * Si no está conectado intenta reconectar a la red.
 * Si está conectado, intenta conectarse a MQTT si aún 
 * no se tiene conexión.
 */
void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Checking wifi");
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      displayNoSignal();
      delay(10);
    }
    Serial.println("connected");
  }
  else
  {
    if (!client.connected())
    {
      mqtt_connect();
    }
    else
    {
      client.loop();
    }
  }
}

/**
 * Imprime en consola la cantidad de redes WiFi disponibles y
 * sus nombres.
 */
void listWiFiNetworks() {
  int numberOfNetworks = WiFi.scanNetworks();
  Serial.println("\nNumber of networks: ");
  Serial.print(numberOfNetworks);
  for(int i =0; i<numberOfNetworks; i++){
      Serial.println(WiFi.SSID(i));
 
  }
}

/**
 * Inicia el servicio de WiFi e intenta conectarse a la red WiFi específicada en las constantes.
 */
void startWiFi() {
  
  WiFi.hostname(USER);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  
  Serial.println("(\n\nAttempting to connect to SSID: ");
  Serial.print(ssid);
  //Intenta conectarse con los valores de las constantes ssid y pass a la red Wifi
  //Si la conexión falla el node se dormirá hasta que lo resetee
  while (WiFi.status() != WL_CONNECTED)
  {
    if ( WiFi.status() == WL_NO_SSID_AVAIL ) {
      Serial.println("\nNo se encuentra la red WiFi ");
      Serial.print(ssid);
      WiFi.begin(ssid, pass);
    } else if ( WiFi.status() == WL_WRONG_PASSWORD ) {
      Serial.println("\nLa contraseña de la red WiFi no es válida.");
    } else if ( WiFi.status() == WL_CONNECT_FAILED ) {
      Serial.println("\nNo se ha logrado conectar con la red, resetee el node y vuelva a intentar");
      WiFi.begin(ssid, pass);
    }
    Serial.print(".");
    delay(1000);
  }
  Serial.println("connected!");
}

/**
 * Consulta y guarda el tiempo actual con servidores SNTP.
 */
void setTime() {
  //Sincroniza la hora del dispositivo con el servidor SNTP (Simple Network Time Protocol)
  Serial.print("Setting time using SNTP");
  configTime(-5 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < 1510592825) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  //Una vez obtiene la hora, imprime en el monitor el tiempo actual
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

/**
 * Configura el servidor MQTT y asigna la función callback para mensajes recibidos por suscripción.
 * Intenta conectarse al servidor.
 */
void configureMQTT() {
  //Llama a funciones de la librería PubSubClient para configurar la conexión con Mosquitto
  client.setServer(MQTT_HOST, MQTT_PORT);
  
  // Se configura la función que se ejecutará cuando lleguen mensajes a la suscripción
  client.setCallback(receivedCallback);
  
  //Llama a la función de este programa que realiza la conexión con Mosquitto
  mqtt_connect();
}

/**
 * Verifica si ya es momento de hacer las mediciones de las variables.
 * si ya es tiempo, mide y envía las mediciones.
 */
void measure() {
  if ((millis() - measureTime) >= MEASURE_INTERVAL * 1000 ) {
    Serial.println("\nMidiendo variables...");
    measureTime = millis();
    
    temp = readTemperatura();
    humi = readHumedad();

    // Se chequea si los valores son correctos
    if (checkMeasures(temp, humi)) {
      // Se envían los datos
      sendSensorData(temp, humi); 
    }
  }
}

void checkCritical(){
  if (isCritical) {
    if ((millis() - criticalTime) <= CRITICAL_DURATION * 1000 ) {
      digitalWrite(CRITICAL_PIN, HIGH);
      criticalTime = millis();
      
    }
    else {
      
      digitalWrite(CRITICAL_PIN, LOW);
       isCritical = false;
     }
  }
}

/////////////////////////////////////
//         FUNCIONES ARDUINO       //
/////////////////////////////////////

void setup() {
  Serial.begin(115200);

  pinMode(CRITICAL_PIN, OUTPUT);

  listWiFiNetworks();

  startDisplay();

  displayConnecting(ssid);

  startWiFi();

  dht.begin();

  setTime();

  configureMQTT(); 
}

void loop() {

  checkWiFi();

  String message = checkAlert();

  measure();
  
  display.clearBuffer();

  displayHeader();
  displayMeasures();
  displayMessage(message.c_str());

  checkCritical();

  display.sendBuffer();

}
