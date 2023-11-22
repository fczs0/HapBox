# HapBox
**Equipe HapBox 21/11/2023**

*Felipe Cortez dos Santos -RM99750*

*Victor Hugo Aranda Forte -RM99667*

## Objetivo
![logo](https://github.com/Blue260910/global-site/assets/84590776/d4fd3d07-239b-44cd-b157-2b90e74764ce)

Este projeto consiste em um código desenvolvido para o **HapBox**. O objetivo é automatizar a administração de medicamentos, estabelecendo uma conexão direta com o médico e proporcionando personalização para o usuário.

## Projeto
O projeto **HapBox** adota uma abordagem inovadora na gestão do tratamento, visando promover a adesão e a supervisão médica. A implementação abrange o controle de informações provenientes do dispositivo HapBox e sua integração a uma plataforma online, permitindo o registro e a análise de dados sobre o uso dos medicamentos em diversas localizações do país. A avaliação dos dados se baseia na consideração da frequência, da dosagem e do horário dos medicamentos, o que contribui para a melhoria da qualidade de vida dos pacientes.
**Modelo 3D HapBox:**

- *O site:* https://blue260910.github.io/global-site/#/
- *Simulação:* https://wokwi.com/projects/381937989733909505

## Componentes necessários para a funcionalidade do código
![image](https://github.com/fczs0/HapBox/assets/145861403/8463d574-ac53-4782-ae69-538bb78c29a9)
Para executar este código, você vai precisar dos seguintes componentes:

- Acesso ao banco de dados da HapBox para definir horários em que os medicamentos serão dispensados, meicamentos, as dosagens e as informações do usuário.
- 1 Dispositivo HapBox (Estrutura).
- 1 Placa de desenvolvimento ESP32 para funções IoT do dispositivo.
- 6 Aparlehos servo motor para liberar os meddicamentos.
- 1 Display LCD para mostrar o status, horário e quiantidade de medicamento.
- 1 sensor de peso, que é usado para medir a quantidade de medicamentos disponíveis no dispositivo.

## Funcionalidade
- Conecta o ESP32 à rede WiFi e ao servidor NTP para obter a hora local.
- Obtém os dados do usuário e do tratamento do banco de dados Firebase, usando a biblioteca ArduinoJson e a classe HTTPClient.
- Inicializa os servos motores que controlam as comportas dos compartimentos de medicamentos, usando a biblioteca ESP32Servo.
- Inicializa o display TFT que exibe as informações do usuário e do tratamento, usando a biblioteca Adafruit_ILI9341.
- Compara a hora local com os horários dos medicamentos definidos no banco de dados, e aciona os servos motores correspondentes para abrir as comportas e dispensar os medicamentos, usando uma função auxiliar.

**Exemplo:**

- *Vídeo* link
