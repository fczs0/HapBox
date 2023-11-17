// Learn about the ESP32 WiFi simulation in
// https://docs.wokwi.com/guides/esp32-wifi

#include <WiFi.h> // Inclui a biblioteca do ESP32 WiFi
#include <Wire.h> // Inclui a biblioteca para comunicação via I2C
#include <LiquidCrystal_I2C.h> // Inclui a biblioteca para o LCD via I2C

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2); // Inicializa o LCD com endereço 0x27, 16 colunas e 2 linhas

#define NTP_SERVER     "pool.ntp.org" // Define o servidor NTP a ser utilizado
#define UTC_OFFSET     -3 * 60 * 60 // Define o deslocamento de tempo UTC
#define UTC_OFFSET_DST 1 // Define o deslocamento de tempo para o horário de verão

void spinner() {
  static int8_t counter = 0; // Variável estática para manter o estado do contador
  const char* glyphs = "\xa1\xa5\xdb"; // Conjunto de caracteres para criar um spinner animado
  LCD.setCursor(15, 1); // Define a posição no LCD para exibir o spinner
  LCD.print(glyphs[counter++]); // Imprime o próximo caractere do spinner
  if (counter == strlen(glyphs)) { // Reinicia o contador quando atinge o final dos caracteres do spinner
    counter = 0;
  }
}

void printLocalTime() {
  struct tm timeinfo; // Estrutura para armazenar informações de data e hora
  if (!getLocalTime(&timeinfo)) { // Obtém a hora local do ESP32
    LCD.setCursor(0, 1);
    LCD.println("Conection Error"); // Exibe mensagem de erro se não conseguir obter a hora
    return;
  }

  LCD.setCursor(8, 0);
  LCD.println(&timeinfo, "%H:%M:%S"); // Exibe a hora no formato HH:MM:SS

  LCD.setCursor(0, 1);
  LCD.println(&timeinfo, "%d/%m/%Y  %Z"); // Exibe a data no formato DD/MM/AAAA
}

void setup() {
  Serial.begin(115200); // Inicializa a comunicação serial

  LCD.init(); // Inicializa o LCD
  LCD.backlight(); // Liga o backlight do LCD
  LCD.setCursor(0, 0);
  LCD.print("Connecting to ");
  LCD.setCursor(0, 1);
  LCD.print("WiFi ");

  WiFi.begin("Wokwi-GUEST", "", 6); // Conecta-se à rede WiFi especificada
  while (WiFi.status() != WL_CONNECTED) { // Aguarda a conexão com a rede WiFi
    delay(250);
    spinner(); // Chama a função para exibir o spinner enquanto aguarda a conexão
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // Exibe o endereço IP atribuído ao ESP32 após a conexão WiFi ser estabelecida

  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.println("Online");
  LCD.setCursor(0, 1);
  LCD.println("Updating time...");

  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER); // Configura o ESP32 para obter a hora do servidor NTP
}

void loop() {
  printLocalTime(); // Chama a função para exibir a hora atual
  delay(250); // Aguarda um curto período de tempo antes de atualizar novamente
}
