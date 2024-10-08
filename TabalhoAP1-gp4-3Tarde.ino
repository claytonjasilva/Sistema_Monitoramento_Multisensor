// Rodrigo Silveira  - 202202064831
// Thales Pereira - 202303458606 
// Victor Oliveira  - 202402818406 


#include "DHT.h"
//DHT sensor library

#define LED_A 2
#define LED_B 3
#define LED_C 4
#define BUZZER 12
#define RGB_PIN_RED 9
#define RGB_PIN_GREEN 10
#define RGB_PIN_BLUE 11

#define BINARY_SWITCH_1 5
#define BINARY_SWITCH_2 6
#define BINARY_SWITCH_3 7
#define BINARY_SWITCH_4 8

#define TRIG_PIN 40
#define ECHO_PIN 41


#define DHTPIN 35
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define ENTER_SWITCH 30
#define MODE_SWITCH 13

void setup() {
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_C, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(RGB_PIN_RED, OUTPUT);
  pinMode(RGB_PIN_GREEN, OUTPUT);
  pinMode(RGB_PIN_BLUE, OUTPUT);

  pinMode(BINARY_SWITCH_1, INPUT);
  pinMode(BINARY_SWITCH_2, INPUT);
  pinMode(BINARY_SWITCH_3, INPUT);
  pinMode(BINARY_SWITCH_4, INPUT);

  pinMode(MODE_SWITCH, INPUT);
  pinMode(ENTER_SWITCH, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  dht.begin();

  Serial.begin(9600);
  Serial.println("Inicializando...");
}

struct Command {
  int bitValue;
  String name;
};

String lastMessage = "";
bool lastMode = HIGH;

void clearSerial() {
  for (int i = 0; i < 5; i++) {
    Serial.println();
  }
}
void printIfChanged(String message) {
  if (message != lastMessage) {
    clearSerial();
    Serial.println(message);
    lastMessage = message;
  }
}

void turnOffAll() {
  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);
  digitalWrite(LED_C, LOW);
  noTone(BUZZER);
  digitalWrite(RGB_PIN_RED, LOW);
  digitalWrite(RGB_PIN_GREEN, LOW);
  digitalWrite(RGB_PIN_BLUE, LOW);
  printIfChanged("[ACTION]: Todos foram desligados.");
}

void turnOnLed(int ledPin, String ledName) {
  digitalWrite(ledPin, HIGH);
  printIfChanged("[ACTION]: LED " + ledName + " ligado.");
}


void readTemp() {
  float temperatura = dht.readTemperature();
  printIfChanged("[ACTION]: Temperatura: " + String(temperatura) + " °C");
  delay(500);
}

void readDistance() {
  long d;
  int distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  d = pulseIn(ECHO_PIN, HIGH);

  distance = d * 0.034 / 2;

  printIfChanged("[ACTION]: Distancia medida: " + String(distance) + " cm");
}

void buzzerOn() {
  tone(BUZZER, 1000);
  printIfChanged("[ACTION]: Buzzer ligado.");
}

void buzzerOff() {
  noTone(BUZZER);
  printIfChanged("[ACTION]: Buzzer desligado.");
}

void setRGB(int redState, int greenState, int blueState) {
  digitalWrite(RGB_PIN_RED, redState);
  digitalWrite(RGB_PIN_GREEN, greenState);
  digitalWrite(RGB_PIN_BLUE, blueState);
  printIfChanged("[ACTION]: RGB ajustado.");
}

void turnOnRed() {
  digitalWrite(RGB_PIN_RED, HIGH);
  printIfChanged("[ACTION]: Vermelho do RGB ligado.");
}

void turnOffRed() {
  digitalWrite(RGB_PIN_RED, LOW);
  printIfChanged("[ACTION]: Vermelho do RGB desligado.");
}

void turnOnGreen() {
  digitalWrite(RGB_PIN_GREEN, HIGH);
  printIfChanged("[ACTION]: Verde do RGB ligado.");
}

void turnOffGreen() {
  digitalWrite(RGB_PIN_GREEN, LOW);
  printIfChanged("[ACTION]: Verde do RGB desligado.");
}

void turnOnBlue() {
  digitalWrite(RGB_PIN_BLUE, HIGH);
  printIfChanged("[ACTION]: Azul do RGB ligado.");
}

void turnOffBlue() {
  digitalWrite(RGB_PIN_BLUE, LOW);
  printIfChanged("[ACTION]: Azul do RGB desligado.");
}

Command commands[] = {
  { 0b0000, "TURN_OFF_ALL" },

  { 0b0001, "TURN_ON_LED_A" },
  { 0b0010, "TURN_ON_LED_B" },
  { 0b0011, "TURN_ON_LED_C" },

  { 0b0100, "BUZZER_ON" },
  { 0b0101, "BUZZER_OFF" },

  { 0b0110, "TURN_ON_RED" },
  { 0b0111, "TURN_ON_GREEN" },
  { 0b1000, "TURN_ON_BLUE" },

  { 0b1001, "TURN_OFF_RED" },
  { 0b1010, "TURN_OFF_GREEN" },
  { 0b1011, "TURN_OFF_BLUE" },

  { 0b1100, "DIST_CHECK" },
  { 0b1101, "TEMP_CHECK" }

};

void processSerialCommand() {
  printIfChanged("[MODE]: Entrou no modo serial.");

  while (true) {
    if (digitalRead(MODE_SWITCH) != lastMode) {
      printIfChanged("[MODE]: Saiu modo serial.");
      break;
    }

    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      if (command.length() > 0) {
        printIfChanged("[COMMAND]: " + command);
        int bitSerialCommand = stringToCommand(command);
        if (bitSerialCommand != -1) {
          executeCommand(bitSerialCommand);
        }
      }
    }
    delay(100);
  }
}

void processBinarySwitches() {
  printIfChanged("[MODE]: Entrou no modo binário.");

  while (true) {
    if (digitalRead(MODE_SWITCH) != lastMode) {
      printIfChanged("[MODE]: Saiu modo binário.");
      break;
    }

    if (digitalRead(ENTER_SWITCH) == HIGH) {
      int bitValue = readBinarySwitches();
      executeCommand(bitValue);
      delay(100);
    }
  }
}

void loop() {
  bool currentMode = digitalRead(MODE_SWITCH);
  if (currentMode != lastMode) {
    delay(400);
    lastMode = currentMode;
    if (currentMode == HIGH) {
      processBinarySwitches();
    } else {
      processSerialCommand();
    }
  }
  delay(1000);
}

int readBinarySwitches() {
  int bit0 = digitalRead(BINARY_SWITCH_1);
  int bit1 = digitalRead(BINARY_SWITCH_2);
  int bit2 = digitalRead(BINARY_SWITCH_3);
  int bit3 = digitalRead(BINARY_SWITCH_4);
  return (bit3 << 3) | (bit2 << 2) | (bit1 << 1) | bit0;
}

int stringToCommand(String command) {
  for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
    if (command == commands[i].name) {
      return commands[i].bitValue;
    }
  }
  printIfChanged("[ERROR]: Comando desconhecido.");
  return -1;
}

void executeCommand(int bitValue) {
  switch (bitValue) {
    case 0b0000:
      turnOffAll();
      break;
    case 0b0001:
      turnOnLed(LED_A, "A");
      break;
    case 0b0010:
      turnOnLed(LED_B, "B");
      break;
    case 0b0011:
      turnOnLed(LED_C, "C");
      break;
    case 0b0100:
      buzzerOn();
      break;
    case 0b0101:
      buzzerOff();
      break;
    case 0b0110:
      turnOnRed();
      break;
    case 0b0111:
      turnOnGreen();
      break;
    case 0b1000:
      turnOnBlue();
      break;
    case 0b1001:
      turnOffRed();
      break;
    case 0b1010:
      turnOffGreen();
      break;
    case 0b1011:
      turnOffBlue();
      break;
    case 0b1100:
      readDistance();
      break;
    case 0b1101:
      readTemp();
      break;
    default:
      printIfChanged("[ERROR]: Comando desconhecido.");
      break;
  }
}
