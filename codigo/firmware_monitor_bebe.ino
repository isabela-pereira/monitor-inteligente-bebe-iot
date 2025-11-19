// ==========================================================
// 1. INCLUSÃO DE BIBLIOTECAS (JÁ INCLUÍDAS NO AMBIENTE WOKWI)
// ==========================================================
#include <WiFi.h>              // Biblioteca para conectividade Wi-Fi do ESP32
#include <PubSubClient.h>      // Biblioteca para o protocolo MQTT
#include <OneWire.h>           // Biblioteca para o Sensor DS18B20 (protocolo OneWire)
#include <DallasTemperature.h> // Biblioteca para o Sensor DS18B20

// ==========================================================
// 2. CONFIGURAÇÕES DE REDE E MQTT
// ==========================================================
// Substitua pelas suas credenciais Wi-Fi (o Wokwi simula uma rede virtual)
const char* ssid = "Wokwi-GUEST"; 
const char* password =""; 

// Configurações do Broker MQTT (HiveMQ - broker público)
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

// Tópicos MQTT para Publicação
const char* TOPIC_STATUS = "mackenzie/iot/bebe/status"; // Status geral
const char* TOPIC_ALERTA = "mackenzie/iot/bebe/alerta"; // Alerta de febre ou choro

// Criação de objetos Wi-Fi e MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// ==========================================================
// 3. CONFIGURAÇÕES DOS PINOS E SENSORES
// ==========================================================
// Pinos do ESP32
const int ONE_WIRE_BUS = 4; // Pino DATA do DS18B20 [cite: 47]
const int SOUND_PIN = 34;   // Pino Analógico do Potenciômetro (simulação de som) 
const int BUZZER_PIN = 2;   // Pino de Acionamento do Buzzer 

// Objetos para DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress babyThermometer; // Armazena o endereço do sensor

// Limiares de Alerta [cite: 72, 73]
const float FEVER_THRESHOLD = 37.8; // Temperatura em Celsius para febre [cite: 47, 72]
const int SOUND_THRESHOLD = 800;    // Valor do potenciômetro para choro intenso (ajuste conforme o teste)

// Variáveis de tempo
long lastMsg = 0;
long publishInterval = 5000; // Publicar a cada 5 segundos

// ==========================================================
// 4. FUNÇÕES AUXILIARES
// ==========================================================

// Função de conexão Wi-Fi
void setup_wifi() {
  delay(10);
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

// Função de reconexão MQTT
void reconnect_mqtt() {
  // Loop enquanto não estiver conectado
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    // Tenta se conectar com um ID aleatório
    String clientId = "ESP32Monitor_";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado!");
      // Opcional: Assinar um tópico para receber comandos (não usado neste projeto)
      // client.subscribe("mackenzie/iot/bebe/comandos");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" Nova tentativa em 5 segundos");
      delay(5000);
    }
  }
}

// ==========================================================
// 5. FUNÇÃO SETUP (CONFIGURAÇÃO INICIAL)
// ==========================================================
void setup() {
  Serial.begin(115200);
  // NOVA LINHA DE TESTE: SE ISTO APARECER, O CÓDIGO ESTÁ RODANDO.
  Serial.println("--- INICIALIZAÇÃO DO CÓDIGO INICIADA ---");
  pinMode(BUZZER_PIN, OUTPUT); // Configura o pino do buzzer como saída

  // Inicia Wi-Fi
  setup_wifi();

  // Configurações MQTT
  client.setServer(mqtt_server, mqtt_port);
  
  // Configurações do DS18B20
  sensors.begin();
  if (!sensors.getAddress(babyThermometer, 0)) {
    Serial.println("ERRO: Nenhum sensor DS18B20 encontrado!");
  } else {
    Serial.print("Sensor DS18B20 encontrado no endereço: ");
    for (int i = 0; i < 8; i++) {
      Serial.print(babyThermometer[i], HEX);
    }
    Serial.println();
    sensors.setResolution(babyThermometer, 10); // 10 bits de resolução
  }
}

// ==========================================================
// 6. FUNÇÃO LOOP (LÓGICA PRINCIPAL)
// ==========================================================
void loop() {
  // Garante a conexão MQTT
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop(); // Processa mensagens de entrada e mantém o keep-alive

  long now = millis();
  if (now - lastMsg > 1000) { // Verifica os sensores a cada 1 segundo
    lastMsg = now;

    // --- LEITURA DE TEMPERATURA (DS18B20) ---
    sensors.requestTemperatures(); // Envia o comando para ler a temperatura
    float temperatureC = sensors.getTempC(babyThermometer);
    
    // --- LEITURA DE SOM (POTENCIÔMETRO VIRTUAL) ---
    int soundLevel = analogRead(SOUND_PIN);
    
    // --- PROCESSAMENTO E ACIONAMENTO DE ALERTA ---
    bool alerta = false;
    String alerta_msg = "";
    
    // 1. Alerta de Febre [cite: 72]
    if (temperatureC > FEVER_THRESHOLD) {
      Serial.printf("ALERTA DE FEBRE! Temperatura: %.2f C\n", temperatureC);
      alerta = true;
      alerta_msg = "Febre detectada! Temp: " + String(temperatureC, 2) + "C";
    }

    // 2. Alerta de Choro Intenso [cite: 73]
    if (soundLevel > SOUND_THRESHOLD) {
      Serial.printf("ALERTA DE CHORO! Nível de Som: %d\n", soundLevel);
      alerta = true;
      if (alerta_msg.length() > 0) alerta_msg += " | "; // Concatena se já houver alerta
      alerta_msg += "Choro intenso detectado! Nível: " + String(soundLevel);
    }
    
    // 3. Acionamento do Atuador Local (Buzzer)
    if (alerta) {
      digitalWrite(BUZZER_PIN, HIGH); // Liga o Buzzer [cite: 74]
      // Publica alerta no tópico específico
      if (client.connected()) {
        client.publish(TOPIC_ALERTA, alerta_msg.c_str()); 
      }
    } else {
      digitalWrite(BUZZER_PIN, LOW); // Desliga o Buzzer
    }

    // --- PUBLICAÇÃO DE STATUS GERAL (A cada 5 leituras) ---
    static int count = 0;
    count++;
    if (count >= 5) { // Publica o status a cada 5 segundos (5x1000ms de intervalo de leitura)
        // Cria a string de status geral
        String statusPayload = "Temp: " + String(temperatureC, 2) + "C | Som: " + String(soundLevel) + " | Alerta: " + (alerta ? "SIM" : "NAO");
        
        // Publica no tópico de Status
        if (client.connected()) {
          client.publish(TOPIC_STATUS, statusPayload.c_str());
          Serial.print("Status MQTT publicado: ");
          Serial.println(statusPayload);
        }
        count = 0;
    }
  }
}