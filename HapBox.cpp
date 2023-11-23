#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <time.h>
#include <ESP32Servo.h>

String firebaseId = "1";          // ID do banco de dados
#define MEDICATION1 "Dipirona"    //Medicamento alocado no compartimento 1
#define MEDICATION2 "Paracetamol" //Medicamento alocado no compartimento 2
#define MEDICATION3 "Ibuprofeno"  //Medicamento alocado no compartimento 3  
#define MEDICATION4 "Aspirina"    //Medicamento alocado no compartimento 4
#define MEDICATION5 "Naproxeno"   //Medicamento alocado no compartimento 5
#define MEDICATION6 "Varfarina"   //Medicamento alocado no compartimento 6

const char* ssid = "Wokwi-GUEST";
const char* password = "";

#define NUM_SERVOS 6 

int anguloFechado = 0;
int anguloAberto = 180;
int pinosServo[NUM_SERVOS] = {14, 27, 26, 25, 33, 32};

Servo servos[NUM_SERVOS];

#define BTN_PIN 5
#define TFT_DC 2
#define TFT_CS 15
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

const String urlBase = "https://hapbox-f9157-default-rtdb.firebaseio.com/";
String url = urlBase + firebaseId + ".json";

DynamicJsonDocument doc(2048);


// Define os parâmetros NTP
long gmtOffset_sec = -3 * 3600; 
int daylightOffset_sec = 3600; 
const char* ntpServer = "pool.ntp.org";

bool horarioCorrespondente[6] = {false, false, false, false, false, false};

void abrirComporta(int indiceServo) {
  servos[indiceServo].write(anguloAberto);
  delay(1000);
  servos[indiceServo].write(anguloFechado);
  delay(1000);
}

DynamicJsonDocument getData() {
  HTTPClient http;
  http.useHTTP10(true);
  http.begin(url);
  http.GET();
  String result = http.getString();

  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, result);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return doc;
  }

  String CPF = doc["CPF"].as<String>();
  String Email = doc["Email"].as<String>();
  String Name = doc["Name"].as<String>();
  String Phone = doc["Phone"].as<String>();

  String Horarios = "";
  for (const auto& horario : doc["Horarios"].as<JsonArray>()) {
    Horarios += horario.as<String>() + " ";
  }

  String Medicamentos = "";
  for (const auto& medicamento : doc["Medicamentos"].as<JsonArray>()) {
    Medicamentos += medicamento.as<String>() + " ";
  }

  String Doses = "";
  for (const auto& dose : doc["Doses"].as<JsonArray>()) {
    Doses += String(dose.as<int>()) + " ";
  }

  String QuantidadeComprimidos = "";
  for (const auto& quantidade : doc["QuantidadeComprimidos"].as<JsonArray>()) {
    QuantidadeComprimidos += String(quantidade.as<int>()) + " ";
  }

  http.end();
  return doc;
}

void displayData() {
  tft.setTextColor(ILI9341_WHITE);
  tft.println("\nLoading data...");

  DynamicJsonDocument data = getData();
  String jsonData;
  serializeJson(data, jsonData);

  tft.setTextColor(ILI9341_GREEN);
  tft.println(jsonData.c_str());
}

void exibirHoraLocal() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  char currentTime[50]; 
  strftime(currentTime, sizeof(currentTime), "%H:%M", &timeinfo);

  JsonArray horarios = doc["Horarios"].as<JsonArray>();
  JsonArray medicamentos = doc["Medicamentos"].as<JsonArray>();

  for (int i = 0; i < horarios.size(); i++) {
    if (String(currentTime) == horarios[i].as<String>()) {
      if (horarioCorrespondente[i]) { 
        continue;
      }
      Serial.println("Horario do remedio");
      horarioCorrespondente[i] = true; 
      String medicamento = medicamentos[i].as<String>();
      if (medicamento == "Dipirona") {
        abrirComporta(0);
      } else if (medicamento == "Paracetamol") {
        abrirComporta(1);
      } else if (medicamento == "Ibuprofeno") {
        abrirComporta(2);
      } else if (medicamento == "Aspirina") {
        abrirComporta(3);
      } else if (medicamento == "Naproxeno") {
        abrirComporta(4);
      } else if (medicamento == "Varfarina") {
        abrirComporta(5);
      }
    } else if (String(currentTime) > horarios[i].as<String>() && horarioCorrespondente[i]) {
      horarioCorrespondente[i] = false; 
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.print("Conectando-se ao Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println(" Conectado!");
  Serial.println(" Inicializando servos!");


  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].attach(pinosServo[i]);
    servos[i].write(anguloFechado);
  }

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  pinMode(BTN_PIN, INPUT_PULLUP);

  tft.begin();
  tft.setRotation(1);

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);

  doc = getData(); 

  displayData();
}

void loop() {
  exibirHoraLocal(); 

  delay(500);
}
