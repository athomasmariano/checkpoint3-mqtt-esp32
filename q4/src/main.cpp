#include <WiFi.h>
#include <PubSubClient.h> // Para comunicação MQTT
#include <ArduinoJson.h>  // Para formatar os dados em JSON

// Identificadores (Mude o ID do seu grupo e módulo)
const char* ID        = "FIAP-GrupoA";
const char* moduleID  = "ESP32-Q4-01"; 

// Wi-Fi (Wokwi usa esta rede automaticamente)
const char* SSID      = "Wokwi-GUEST";
const char* PASSWORD  = ""; 

// MQTT Broker (Seu IP Público da Azure e Porta 1883)
// EX: "102.37.159.53"
const char* BROKER_MQTT  = "102.37.159.53"; 
const int   BROKER_PORT  = 1883;

// Credenciais de autenticação do Mosquitto
// EX: "fiapuser" e "sua_senha_secreta"
const char* mqttUser     = "arthur";
const char* mqttPassword = "arthur";

// Tópico MQTT para publicar dados
// Ex: fiap/grupoA/dados
#define TOPICO_PUBLISH  "fiap/grupoA/sensor_data" 

// =========================================================================
// 2. CONFIGURAÇÃO MQTT E WIFI
// =========================================================================

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
const int PUBLISH_INTERVAL_MS = 5000; // Publica a cada 5 segundos

// Função para conectar ao Wi-Fi
void setup_wifi() {
  delay(10);
  Serial.print("\nConectando a ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" SUCESSO!");
  Serial.print("IP da Placa: ");
  Serial.println(WiFi.localIP());
}

// Função para reconectar ao Broker MQTT
void reconnect() {
  // Loop enquanto não se conectar
  while (!client.connected()) {
    Serial.print("Tentando conexao MQTT...");
    // Tenta conectar, usando o ID do módulo como Client ID
    if (client.connect(moduleID, mqttUser, mqttPassword)) {
      Serial.println(" conectado!");
    } else {
      Serial.print(" falhou, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

// =========================================================================
// 3. LÓGICA DE SIMULAÇÃO E PUBLICAÇÃO
// =========================================================================

void publishData() {
  // Gerar dados simulados dentro dos ranges especificados:
  
  // Temperatura: 20°C a 35°C (Gerando float com 1 casa decimal)
  float temp = random(200, 350) / 10.0; 

  // Umidade: 40% a 80% (Gerando inteiro)
  int umid = random(40, 80);

  // Pressão: 980 hPa a 1050 hPa (Gerando inteiro)
  int pressao = random(980, 1050);

  // Altitude: 0 m a 500 m (Gerando inteiro)
  int altitude = random(0, 500);

  // Usando ArduinoJson para criar o payload
  StaticJsonDocument<200> doc;
  doc["id"] = ID;
  doc["temperatura"] = temp;
  doc["umidade"] = umid;
  doc["pressao"] = pressao;
  doc["altitude"] = altitude;
  doc["timestamp"] = millis(); // Adicionando timestamp para rastreio

  char jsonPayload[200];
  serializeJson(doc, jsonPayload);

  // Publicar mensagem
  client.publish(TOPICO_PUBLISH, jsonPayload);

  // Log no serial
  Serial.print("Publicado no Topico [");
  Serial.print(TOPICO_PUBLISH);
  Serial.print("]: ");
  Serial.println(jsonPayload);
}

// =========================================================================
// 4. SETUP E LOOP PRINCIPAL
// =========================================================================

void setup() {
  Serial.begin(115200);
  Serial.println("--- Checkpoint 03 - MQTT com ESP32 ---");
  
  // Configurações do MQTT
  client.setServer(BROKER_MQTT, BROKER_PORT);
  
  setup_wifi();
}

void loop() {
  // Garante que o cliente MQTT esteja conectado
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // Mantém o cliente MQTT em funcionamento

  long now = millis();
  // Publica a cada 5 segundos
  if (now - lastMsg > PUBLISH_INTERVAL_MS) {
    lastMsg = now;
    publishData();
  }
}
