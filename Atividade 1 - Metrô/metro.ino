#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o display LCD (endereço I2C, colunas, linhas)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pinos de entrada
const int validateButtonPin = 2;  // Botão para validar entrada de passageiros
const int stationButtonPin = 3;   // Botão para simular chegada/partida do metrô

// Variáveis de estado
int currentCount = 0;             // Contagem de passageiros do dia atual
int lastDayCount = 0;             // Contagem de passageiros do dia anterior
int dayBeforeLastCount = 0;       // Contagem de passageiros de dois dias atrás
bool isPowered = false;           // Estado da alimentação do sistema
bool isAtStation = false;         // Indica se o metrô está em uma estação

void setup() {
  // Inicializa o display LCD
  lcd.begin();
  lcd.backlight();
  lcd.print("Metro UFS-FFA");

  // Configura os pinos de entrada
  pinMode(validateButtonPin, INPUT);
  pinMode(stationButtonPin, INPUT);

  // Inicializa a comunicação serial
  Serial.begin(9600);
  Serial.println("Sistema Metro UFS-FFA Iniciado");
}

void loop() {
  // Verifica se o botão de chegada/partida foi pressionado
  if (digitalRead(stationButtonPin) == HIGH) {
    delay(200);  // Debounce
    if (isAtStation) {
      // Partida da estação
      departFromStation();
    } else {
      // Chegada na estação
      arriveAtStation();
    }
    isAtStation = !isAtStation;  // Alterna o estado da estação
    delay(500);  // Evita múltiplas leituras
  }

  // Verifica se o botão de validação foi pressionado
  if (digitalRead(validateButtonPin) == HIGH && isPowered) {
    delay(200);  // Debounce
    validateEntry();
    delay(500);  // Evita múltiplas leituras
  }
}

void arriveAtStation() {
  // Simula a chegada do metrô em uma estação
  isPowered = true;  // Liga a alimentação
  lcd.clear();
  lcd.print("Chegada Estacao");
  Serial.println("Chegada na Estacao");
}

void departFromStation() {
  // Simula a partida do metrô de uma estação
  isPowered = false;  // Desliga a alimentação
  lcd.clear();
  lcd.print("Partida Estacao");
  lcd.setCursor(0, 1);
  lcd.print("Passageiros: ");
  lcd.print(currentCount);
  Serial.print("Partida da Estacao - Passageiros: ");
  Serial.println(currentCount);
}

void validateEntry() {
  // Simula a validação de entrada de um passageiro
  currentCount++;
  lcd.clear();
  lcd.print("Passageiros: ");
  lcd.print(currentCount);
  Serial.print("Passageiro validado. Total: ");
  Serial.println(currentCount);
}

void resetSystem() {
  // Reseta o sistema no início de um novo dia
  dayBeforeLastCount = lastDayCount;
  lastDayCount = currentCount;
  currentCount = 0;

  // Exibe a diferença entre os dois últimos dias
  int difference = lastDayCount - dayBeforeLastCount;
  lcd.clear();
  lcd.print("Diferenca: ");
  lcd.print(difference);
  Serial.print("Sistema Resetado. Diferenca: ");
  Serial.println(difference);
}