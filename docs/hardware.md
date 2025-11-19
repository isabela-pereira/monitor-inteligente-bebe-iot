# **🛠 Hardware Utilizado**
Esta seção detalha os componentes físicos utilizados no projeto e as conexões elétricas estabelecidas.
## **1. Componentes:**
- **Microcontrolador:** ESP32 DevKit V1 (Processamento e Conectividade Wi-Fi).
- **Sensor de Temperatura:** DS18B20 (Leitura digital de temperatura com protocolo OneWire).
- **Sensor de Som:** Potenciômetro (Utilizado para simular o nível de som/microfone KY-038).
- **Atuador:** Buzzer piezoelétrico (Emite o alerta sonoro local).
- **Resistor:** Resistor de $4.7\text{ k}\Omega$ (Necessário para a comunicação Pull-Up do DS18B20).
- **Apoio:** Protoboard e jumpers.
## **2. Ligações (Fiação)**
As conexões a seguir garantem que o firmware no ESP32 possa ler os dados dos sensores e acionar o atuador.

|**Componente**|**Função**|**Pino do ESP32**|**Observações**|
| :- | :- | :- | :- |
|**DS18B20 DATA**|Sinal de Dados|**GPIO4**|Utiliza o protocolo OneWire.|
|**DS18B20 VCC**|Alimentação|**3.3V**|Alimentação da placa.|
|**DS18B20 GND**|Terra|**GND**|Ponto de referência negativo.|
|**Resistor 4.7k**|Pull-up|Entre DATA e VCC|**CRÍTICO** para o funcionamento do DS18B20.|
|**Potenciômetro SIG**|Nível Analógico|**GPIO34**|Pino de entrada analógica (ADC).|
|**Buzzer**|Atuador de Alerta|**GPIO2**|Pino de saída digital para controle do som.|
