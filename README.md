# 🚀 CHECKPOINT 03: Protocolo MQTT com ESP32, Mosquitto e Node-RED

## 📝 Informações do Projeto

| Item | Detalhe |
| :--- | :--- |
| **Disciplina** | Disruptive Architectures: IOT, IOB & Generative AI |
| **Professor** | André Tritiack |
| **Grupo** | Arthur Thomas RM: 561061 |
| **Data da Entrega**| 03/11/2025 |
| **Status** | CONCLUÍDO COM SUCESSO |

---

## 🔗 Entregáveis

| Arquivo/Link | Status | Descrição |
| :--- | :--- | :--- |
| **Link do Vídeo** | **[https://youtu.be/oKUahJeDL2E]** | Demonstração da comunicação (Wokwi -> Mosquitto -> Node-RED) em tempo real. |
| `integrantes.txt` | OK | Arquivo contendo os nomes completos dos membros. |
| `q4/src/main.cpp` | OK | Código final do ESP32 com credenciais e lógica de simulação. |

---

## 🎯 Objetivo da Atividade

Implementar um sistema de comunicação IoT completo utilizando o protocolo **MQTT**. O sistema consiste em um dispositivo simulado (ESP32) enviando dados para um Broker em nuvem (Mosquitto na Azure), que é visualizado e processado em um dashboard (Node-RED).

## ☁️ PARTE 1: Criação da Infraestrutura na Azure

Toda a infraestrutura de comunicação foi configurada em uma **Máquina Virtual** na Azure.

### Detalhes da VM

| Recurso | Detalhe |
| :--- | :--- |
| **IP Público** | `102.37.159.53` |
| **Sistema Operacional** | Ubuntu Server 20.04 LTS |
| **Node-RED Acesso** | `http://102.37.159.53:1880` |

### Portas de Comunicação (Firewall Inbound)

As seguintes portas foram liberadas no Grupo de Segurança de Rede (NSG):

* **22:** SSH (Acesso)
* **1883:** MQTT (Broker Mosquitto)
* **1880:** Node-RED (Dashboard)

### Credenciais MQTT

O Mosquitto Broker foi configurado para **não permitir conexões anônimas**, exigindo autenticação:

* **Usuário MQTT:** `arthur`
* **Senha MQTT:** `arthur`

---

## 💻 PARTE 2: Configuração e Simulação do ESP32 (Ambiente Q4)

O código foi desenvolvido no ambiente PlatformIO/VS Code e simulado no Wokwi, utilizando a estrutura **`[env:q4]`** no `platform.ini`.

### 1. Dependências do PlatformIO (`platform.ini`)

Apenas as dependências essenciais foram mantidas para o ambiente `q4`:
* `bblanchon/ArduinoJson`: Para criar o payload JSON.
* `knolleary/PubSubClient`: Para gerenciar a comunicação MQTT.

### 2. Lógica de Simulação (`q4/src/main.cpp`)

O ESP32 simula a leitura de quatro variáveis, gerando valores aleatórios dentro dos limites exigidos, e publica o resultado no formato **JSON** a cada 5 segundos no tópico `fiap/grupoA/sensor_data`.

| Variável | Range de Simulação |
| :--- | :--- |
| **Temperatura** | 20°C a 35°C |
| **Umidade** | 40% a 80% |
| **Pressão** | 980 hPa a 1050 hPa |
| **Altitude** | 0 m a 500 m |

### Exemplo de Payload JSON Enviado

```json
{"id":"FIAP-GrupoA", "temperatura":26.8, "umidade":67, "pressao":982, "altitude":318, "timestamp":8176}
