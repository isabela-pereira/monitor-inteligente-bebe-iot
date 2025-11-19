# **👶 Monitor Inteligente de Bem-Estar para Recém-Nascidos (IoT)**
Repositório oficial do projeto **Monitor Inteligente de Bem-Estar para Recém-Nascidos**, desenvolvido por Lidiane Brito Ferreira, Isabela Pereira Felipe, Igor Cristiano Pinto Lopes e equipe como parte da disciplina de Internet das Coisas (IoT).

O sistema utiliza sensores de **temperatura** e **som** conectados a um **ESP32**, com comunicação via **Wi-Fi (TCP/IP)** e **protocolo MQTT** para monitoramento remoto em tempo real.

## **🚼 Funcionalidades e Monitoramento**
O protótipo realiza a leitura dos sensores a cada segundo e publica o **Status Geral** a cada **5 segundos**.
### **Sensores e Limiares:**

|**Grandeza**|**Sensor**|**Pino ESP32**|**Limiar de Alerta**|
| :- | :- | :- | :- |
|**Temperatura**|DS18B20|GPIO 4|$37.8^\circ\text{C}$ (Febre)|
|**Som**|Potenciômetro (Simulação)|GPIO 34|800 (Choro Intenso)|
---

### **Atuador:**
- Ativação automática de alerta sonoro (Buzzer no GPIO 2) em caso de febre ou choro intenso.
---

## **📡 Comunicação MQTT**
Broker utilizado:

broker.hivemq.com

Porta 1883 (TCP padrão para o ESP32)
---

### **Tópicos Publicados:**
O sistema utiliza dois tópicos de saída (publicação) com QoS 0:

|**Tópico**|**Conteúdo**|**Frequência**|
| :- | :- | :- |
|mackenzie/iot/bebe/status|Status geral (Temp, Som, Alerta)|A cada 5 segundos|
|mackenzie/iot/bebe/alerta|Mensagem detalhada do alerta (Febre ou Choro)|Somente quando o limiar é cruzado|
---

## **🧩 Simulação no Wokwi**
A simulação completa pode ser importada usando os arquivos:

📄 [firmware_monitor_bebe.ino](codigo/firmware_monitor_bebe.ino) (Código principal)

📄 diagram.json (Fiação e componentes)
---
<<<<<<< HEAD
### **Hardware Virtual:**
- ESP32 DevKit V1
- Sensor de temperatura DS18B20 (GPIO 4)
- Potenciômetro (GPIO 34)
- Buzzer piezoelétrico (GPIO 2)
- Resistor 4.7k (pull-up no DS18B20)
---
## **👩‍💻 Autores**
Lidiane Brito Ferreira

Isabela Pereira Felipe

Igor Cristiano Pinto Lopes
=======
## Autoria
**Lidiane Brito Ferreira, Isabela Pereira Felipe, Igor Cristiano Pinto Lopes**
>>>>>>> b66e25b543e831894933c2b063db6f5bc853cbeb
