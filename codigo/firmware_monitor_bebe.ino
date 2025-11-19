#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PubSubClient.h>

// -----------------------------
// CONFIGURAÇÕES DE REDE E MQTT
// -----------------------------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

// Tópicos MQTT
const char* topicTemp = "bebemonitor/temperatura";
const char* topicSom  = "bebemonitor/som";

// -----------------------------
// PINOS
// -----------------------------
#define ONE_WIRE_BUS 4      // DS18B20
#define POT_PIN 34          // Potenciômetro simulando microfone
#define BUZZER_PIN 5        // Buzzer

// -----------------------------
// OBJETOS
// -----------------------------
WiFiClient espClient;
PubSubClient client(espClient);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// -----------------------------
// LIMITES
// -----------------------------
float limiteFebre = 37.8;     // Temperatura (°C)
int limiteSom = 2200;         // Limiar do choro/agitação

// -----------------------------
// FUNÇÕES
// -----------------------------
void conectarWiFi() {
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");
  Serial.println(WiFi.localIP());
}

void conectarMQTT() {
  while (!client.connected()) {
    Serial.println("Conectando ao broker MQTT...");
    
    if (client.connect("MonitorBebeESP32")) {
      Serial.println("Conectado ao MQTT!");
    } else {
      Serial.print("Falha (rc=");
      Serial.print(client.state());
      Serial.println("). Tentando novamente em 3 segundos...");
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  sensors.begin();
  conectarWiFi();

  client.setServer(mqtt_server, mqtt_port);
  conectarMQTT();
}

// -----------------------------
// LOOP PRINCIPAL
// -----------------------------
void loop() {
  if (!client.connected()) {
    conectarMQTT();
  }
  client.loop();

  // Leitura temperatura
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  // Leitura som (potenciômetro)
  int som = analogRead(POT_PIN);

  // Exibição no monitor serial
  Serial.print("Temperatura: ");
  Serial.print(tempC);
  Serial.print(" °C | Som: ");
  Serial.println(som);

  // Publicar temperatura
  client.publish(topicTemp, String(tempC).c_str());

  // Publicar som
  client.publish(topicSom, String(som).c_str());

  // Verificar febre
  if (tempC >= limiteFebre) {
    Serial.println("⚠ Alerta: FEBRE detectada!");
    tone(BUZZER_PIN, 1000, 300);
    delay(300);
  }

  // Verificar choro/agitação
  if (som >= limiteSom) {
    Serial.println("⚠ Alerta: SOM ALTO detectado!");
    tone(BUZZER_PIN, 1500, 300);
    delay(300);
  }

  delay(1000);  // intervalo de 1s
}
