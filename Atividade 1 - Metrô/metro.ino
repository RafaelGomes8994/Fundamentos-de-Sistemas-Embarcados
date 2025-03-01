#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd_1(7); // Configuração do LCD

// Variáveis de estado
int currentCount = 0;
int lastDayCount = 0;
int dayBeforeLastCount = 0;
bool isPowered = false;
bool isAtStation = false;

// Pinos dos botões
const int stationButtonPin = A0;   // Chegada/partida
const int validateButtonPin = A1;  // Validação
const int resetButtonPin = A2;     // Novo botão de reset

void setup() {
  lcd_1.begin(16, 2);
  lcd_1.print("Metro UFS-FFA");

  // Configura os pinos dos botões como entrada
  pinMode(stationButtonPin, INPUT);
  pinMode(validateButtonPin, INPUT);
  pinMode(resetButtonPin, INPUT); // Novo botão

  Serial.begin(9600);
  Serial.println("Sistema Metro UFS-FFA Iniciado");
}

void loop() {
  // Verifica botão de chegada/partida
  if (digitalRead(stationButtonPin) == HIGH) {
    delay(200);
    if (isAtStation) {
      departFromStation();
    } else {
      arriveAtStation();
    }
    isAtStation = !isAtStation;
    delay(500);
  }

  // Verifica botão de validação
  if (digitalRead(validateButtonPin) == HIGH && isPowered) {
    delay(200);
    validateEntry();
    delay(500);
  }

  // --- NOVO: Verifica botão de reset ---
  if (digitalRead(resetButtonPin) == HIGH) {
    delay(200); // Debounce
    resetSystem();
    delay(500); // Evita múltiplos resets
  }
}

void arriveAtStation() {
  // Simula a chegada do metrô em uma estação
  isPowered = true;  // Liga a alimentação
  lcd_1.clear();
  lcd_1.print("Chegada Estacao");
  Serial.println("Chegada na Estacao");
}

void departFromStation() {
  // Simula a partida do metrô de uma estação
  isPowered = false;  // Desliga a alimentação
  lcd_1.clear();
  lcd_1.print("Partida Estacao");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Passageiros: ");
  lcd_1.print(currentCount);
  Serial.print("Partida da Estacao - Passageiros: ");
  Serial.println(currentCount);
}

void validateEntry() {
  // Simula a validação de entrada de um passageiro
  currentCount++;
  lcd_1.clear();
  lcd_1.print("Passageiros: ");
  lcd_1.print(currentCount);
  Serial.print("Passageiro validado. Total: ");
  Serial.println(currentCount);
}

void resetSystem() {
  // Reseta o sistema no início de um novo dia
  dayBeforeLastCount = lastDayCount;
  lastDayCount = currentCount;
  currentCount = 0;

  // Calcula a diferença entre os dois últimos dias
  int difference = lastDayCount - dayBeforeLastCount;
  lcd_1.clear();
  lcd_1.print("Diferenca: ");
  lcd_1.print(difference);
  Serial.print("Sistema Resetado. Diferenca: ");
  Serial.println(difference);
}