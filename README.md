# Monitor Inteligente de Bem-Estar para Recém-Nascidos (IoT)

Repositório oficial do projeto **Monitor Inteligente de Bem-Estar para Recém-Nascidos**, desenvolvido por Isabela Pereira Felipe e equipe como parte da disciplina de Internet das Coisas (IoT).

O sistema utiliza sensores de **temperatura** e **som** conectados a um **ESP32**, com comunicação via **Wi-Fi (TCP/IP)** e **protocolo MQTT** para monitoramento remoto em tempo real.

---

## 🚼 Sobre o Projeto
O protótipo monitora:
- Temperatura corporal do bebê (DS18B20)
- Nível de som (microfone / KY-038 — simulado via potenciômetro)
- Ativação automática de alerta sonoro (buzzer)
- Envio dos dados para a internet via MQTT (HiveMQ)
- Possibilidade de monitoramento remoto em dashboards MQTT

Aplicação direta na ODS 3: Saúde e Bem-Estar.

---

## 📡 Comunicação MQTT

Broker utilizado:
broker.hivemq.com
Porta 1883


Tópicos:
- `bebemonitor/temperatura`
- `bebemonitor/som`

Toda documentação está em `docs/comunicacao_mqtt.md`.

---

## 🧩 Simulação no Wokwi

A simulação completa pode ser importada usando o arquivo:

📄 `/simulacao-wokwi/diagram.json`

Ou acessando:

📌 *(insira o link após importar no Wokwi)*

---

## 🛠 Hardware Usado

- ESP32 DevKit V1  
- Sensor de temperatura DS18B20  
- Potenciômetro simulando microfone KY-038  
- Buzzer piezoelétrico  
- Resistor 4.7k (pull-up)  
- Jumpers + protoboard

Documentação completa: `docs/hardware.md`

---
## Autora
**Lidiane Brito Ferreira, Isabela Pereira Felipe, Igor Cristiano Pinto Lopes**
