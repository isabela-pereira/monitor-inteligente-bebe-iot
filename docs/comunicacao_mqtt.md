# Comunicação MQTT
Usa broker HiveMQ:

Servidor: broker.hivemq.com  
Porta: 1883  

Tópicos:
- mackenzie/iot/bebe/status
- mackenzie/iot/bebe/alerta

Formato:
- Temperatura (float)
- Nível de som (0–4095)

Bibliotecas utilizadas:
- PubSubClient
- OneWire
- DallasTemperature
