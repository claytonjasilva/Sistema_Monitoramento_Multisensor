// Michel de Mleo Guimarães* - 202401569852 - TA
// Pedro Macedo de Araujo 202401318728 - TA
// Bernardo Lobo Marques - 202401709433 - TA
// Gustavo Leonel Salvador - 202402875361 - TA
// Bernardo Moreira Guimarães Gonçalves - 202401500283 - TA
// João Victor Bathomarco Corrêa Carneiro - 202302902448 - TA


#include <DHT.h>
#include <Adafruit_Sensor.h>

#define chaveControle 12 

#define A 9 
#define B 10
#define C 11

#define buzzer 8 

#define chaveBiniaria1 2 
#define chaveBiniaria2 3 
#define chaveBiniaria3 4 
#define chaveBiniaria4 5 

#define redPin 23
#define greenPin 25
#define bluePin 27

#define DHTPIN A0     
#define DHTTYPE DHT11 

#define TRIG_PIN 7    
#define ECHO_PIN 6    
#define LDR_PIN A3    

DHT dht(DHTPIN, DHTTYPE); 

int contador = 0;

void setup() {
    Serial.begin(9600);
    
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(chaveControle, INPUT);
    pinMode(chaveBiniaria1, INPUT);
    pinMode(chaveBiniaria2, INPUT);
    pinMode(chaveBiniaria3, INPUT);
    pinMode(chaveBiniaria4, INPUT);
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    
    dht.begin(); 
    
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    
    pinMode(LDR_PIN, INPUT);
}

void loop() {
    int controleModo = digitalRead(chaveControle);
    if (controleModo == LOW) { 
        if (contador == 1) {
            inicioModoProgramacaoSerial();
        } else if (Serial.available() > 0) {
            String comando = Serial.readStringUntil('\n');
            if (comando == "INICIO_PROG") {
                contador = 1;
                Serial.println("Modo de programação ativado.");
                
            }
        }
    } else {
        contador = 0;
        inicioModoProgramacaoBinario();
    } 
}

void inicioModoProgramacaoSerial() {
    if (Serial.available() > 0) {
        String comando = Serial.readStringUntil('\n');
        Serial.print("Comando recebido: ");
        Serial.println(comando); 
        
        if (comando == "LED_ON A") {
            ligarLed(A);
        } 
        else if (comando == "LED_ON B") {
            ligarLed(B);
        } 
        else if (comando == "LED_ON C") {
            ligarLed(C);
        } 
        else if (comando == "LED_OFF A") {
            desligarLed(A);
        } 
        else if (comando == "LED_OFF B") {
            desligarLed(B);
        } 
        else if (comando == "LED_OFF C") {
            desligarLed(C);
        } 
        else if (comando == "BUZZ_ON") {
            ligarBuzzer();
        } 
        else if (comando == "BUZZ_OFF") {
            desligarBuzzer();
        } 
        else if (comando == "RGB_SET_COLOR A RED") {
            corLed(255, 0, 0);
        } 
        else if (comando == "RGB_SET_COLOR A GREEN") {
            corLed(0, 255, 0);
        } 
        else if (comando == "RGB_SET_COLOR A BLUE") {
            corLed(0, 0, 255);
        }
        else if (comando == "TEMP_READ A") {
            float h = dht.readHumidity();
            float t = dht.readTemperature();
            if (isnan(h) || isnan(t)) {
                Serial.println("Falha ao ler do DHT");
            } else {
                Serial.print("Umidade: ");
                Serial.print(h);
                Serial.print(" %\t");
                Serial.print("Temperatura: ");
                Serial.print(t);
                Serial.println(" °C");
            }
        }
        else if (comando == "DIST_CHECK A") {
            long duration, distance;
            digitalWrite(TRIG_PIN, LOW);
            delayMicroseconds(2);
            digitalWrite(TRIG_PIN, HIGH);
            delayMicroseconds(10);
            digitalWrite(TRIG_PIN, LOW);
            duration = pulseIn(ECHO_PIN, HIGH);
            distance = (duration / 2) / 29.1; 
            Serial.print("Distância: ");
            Serial.print(distance);
            Serial.println(" cm");
        }
        else if (comando == "LDR_READ A") {
            int lightLevel = analogRead(LDR_PIN);
            Serial.print("Nível de luz (raw): ");
            Serial.println(lightLevel);
            delay(500);
  
        }
        else if (comando == "FIM_PROG") {
            contador = 0; 
            fimModoProgramacao();
        }
    }
}

void inicioModoProgramacaoBinario() {
    int estadoChave1 = digitalRead(chaveBiniaria1); 
    int estadoChave2 = digitalRead(chaveBiniaria2); 
    int estadoChave3 = digitalRead(chaveBiniaria3); 
    int estadoChave4 = digitalRead(chaveBiniaria4); 

 
    if (estadoChave1 == LOW && estadoChave2 == LOW && estadoChave3 == LOW && estadoChave4 == LOW) { // 0000
        contador = 1;
      Serial.println("Modo de programação ativado.");
      delay(1000);
    } 
    else if (estadoChave1 == LOW && estadoChave2 == LOW && estadoChave3 == LOW && estadoChave4 == HIGH) { // 0001
        ligarLed(A);
    } 
    else if (estadoChave1 == LOW && estadoChave2 == LOW && estadoChave3 == HIGH && estadoChave4 == LOW) { // 0010
        ligarLed(B);
    } 
    else if (estadoChave1 == LOW && estadoChave2 == LOW && estadoChave3 == HIGH && estadoChave4 == HIGH) { // 0011
        ligarLed(C);
    } 
    else if (estadoChave1 == LOW && estadoChave2 == HIGH && estadoChave3 == LOW && estadoChave4 == LOW) { // 0100
        desligarLed(A);
    } 
    else if (estadoChave1 == LOW && estadoChave2 == HIGH && estadoChave3 == LOW && estadoChave4 == HIGH) { // 0101
        desligarLed(B); 
    } 
    else if (estadoChave1 == LOW && estadoChave2 == HIGH && estadoChave3 == HIGH && estadoChave4 == LOW) { // 0110
        desligarLed(C);
    } 
    else if (estadoChave1 == LOW && estadoChave2 == HIGH && estadoChave3 == HIGH && estadoChave4 == HIGH) { // 0111       
        ligarBuzzer();
    } 
    else if (estadoChave1 == HIGH && estadoChave2 == LOW && estadoChave3 == LOW && estadoChave4 == LOW) { // 1000
        //desligarBuzzer();        
    } 
    else if (estadoChave1 == HIGH && estadoChave2 == LOW && estadoChave3 == LOW && estadoChave4 == HIGH) { // 1001
        corLed(255, 0, 0); // RED
    }     
    else if (estadoChave1 == HIGH && estadoChave2 == LOW && estadoChave3 == HIGH && estadoChave4 == LOW) { // 1010
        corLed(0, 255, 0); // GREEN
    } 
    else if (estadoChave1 == HIGH && estadoChave2 == LOW && estadoChave3 == HIGH && estadoChave4 == HIGH) { // 1011
        corLed(0, 0, 255); // BLUE
    }
    else if (estadoChave1 == HIGH && estadoChave2 == HIGH && estadoChave3 == LOW && estadoChave4 == LOW) { // 1100
        long duration, distance;
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);
        duration = pulseIn(ECHO_PIN, HIGH);
        distance = (duration / 2) / 29.1; 
        Serial.print("Distância: ");
        Serial.print(distance);
        Serial.println(" cm");
        delay(1000);
    }
    else if (estadoChave1 == HIGH && estadoChave2 == HIGH && estadoChave3 == LOW && estadoChave4 == HIGH) { // 1101 
      int lightLevel = analogRead(LDR_PIN);
        Serial.print("Nível de luz: ");
        Serial.println(lightLevel);
        delay(1000);
    }
  else if (estadoChave1 == HIGH && estadoChave2 == HIGH && estadoChave3 == HIGH && estadoChave4 == LOW) { // 1110 
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      if (isnan(h) || isnan(t)) {
          Serial.println("Falha ao ler do DHT");
      } else {
          Serial.print("Umidade: ");
          Serial.print(h);
          Serial.print(" %\t");          
          Serial.print("Temperatura: ");
          Serial.print(t);
          Serial.println(" °C");
          delay(1000);
      }
  }
  else if (estadoChave1 == HIGH && estadoChave2 == HIGH && estadoChave3 == HIGH && estadoChave4 == HIGH) { // 1111 
    contador = 0;
    fimModoProgramacao();
  }
}

void desligarTodosOsComponentes() {
    desligarLed(A);
    desligarLed(B);
    desligarLed(C);
    desligarBuzzer();
    corLed(0, 0, 0); 
}

void ligarLed(int led) {
    digitalWrite(led, HIGH);
}

void desligarLed(int led) {
    digitalWrite(led, LOW);
}

void ligarBuzzer() {
    digitalWrite(buzzer, HIGH);
}

void desligarBuzzer() {
    digitalWrite(buzzer, LOW);
}

void corLed(int r, int g, int b) {
    analogWrite(redPin, r);
    analogWrite(greenPin, g);
    analogWrite(bluePin, b);
}

void fimModoProgramacao() {
  Serial.println("Modo de programação finalizado.");
  desligarTodosOsComponentes();
}
