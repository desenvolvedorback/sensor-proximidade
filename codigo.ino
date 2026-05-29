// Definição dos pinos do Sensor Ultrassônico
const int pinoTrig = 9;
const int pinoEcho = 10;

// Definição dos pinos dos LEDs
const int ledVerde   = 2;
const int ledAmarelo = 3;
const int ledVermelho = 4;

// Definição do pino do Buzzer (Som)
const int pinoBuzzer = 5;

// Variáveis para o cálculo da distância
long duracao;
int distancia;

void setup() {
  // Configuração dos pinos do sensor
  pinMode(pinoTrig, OUTPUT);
  pinMode(pinoEcho, INPUT);
  
  // Configuração dos pinos de saída
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(pinoBuzzer, OUTPUT);
  
  // --- INICIALIZAÇÃO DO MONITOR SERIAL ---
  Serial.begin(9600);
  Serial.println("=========================================");
  Serial.println("  SISTEMA INICIALIZADO: SENSOR DE RÉ     ");
  Serial.println("=========================================");
}

void loop() {
  // Limpa o pino Trig
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);
  
  // Envia um pulso de 10 microssegundos para ativar o sensor
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);
  
  // Lê o tempo que o eco demorou para voltar
  duracao = pulseIn(pinoEcho, HIGH);
  
  // Calcula a distância em centímetros
  distancia = duracao * 0.034 / 2;
  
  // --- ENVIANDO LOGS PARA O MONITOR SERIAL ---
  Serial.print("[LOG] Distancia atual: ");
  Serial.print(distancia);
  Serial.print(" cm | Status: ");

  // --- LÓGICA DO SENSOR DE ESTACIONAMENTO + LOGS ---
  
  if (distancia > 30 || distancia <= 0) {
    // Caso 1: Muito longe (Mais de 30cm)
    Serial.println("LIVRE - Tudo apagado");
    desligarTudo();
  } 
  else if (distancia <= 30 && distancia > 15) {
    // Caso 2: Longe (Entre 16cm e 30cm)
    Serial.println("ATENÇÃO - LED Verde Ligado");
    desligarTudo();
    digitalWrite(ledVerde, HIGH);
    bipar(100, 500); // Bipa por 100ms e espera 500ms
  } 
  else if (distancia <= 15 && distancia > 5) {
    // Caso 3: Atenção/Perto (Entre 6cm e 15cm)
    Serial.println("CUIDADO - LED Amarelo Ligado");
    desligarTudo();
    digitalWrite(ledAmarelo, HIGH);
    bipar(100, 200); // Bipa por 100ms e espera 200ms
  } 
  else if (distancia <= 5) {
    // Caso 4: Perigo/Muito perto (5cm ou menos)
    Serial.println("PERIGO ABSOLUTO! - LED Vermelho e Buzzer Continuo");
    desligarTudo();
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(pinoBuzzer, HIGH); // Som travado aceso (perigo)
    delay(100);
  }
}

// Função auxiliar para desligar todos os alertas
void desligarTudo() {
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVermelho, LOW);
  digitalWrite(pinoBuzzer, LOW);
}

// Função auxiliar para fazer o buzzer apitar com intervalos personalizados
void bipar(int tempoAcesdo, int tempoDesligado) {
  digitalWrite(pinoBuzzer, HIGH);
  delay(tempoAcesdo);
  digitalWrite(pinoBuzzer, LOW);
  delay(tempoDesligado);
}
