/* * ---------------------------------------------------------
 * PRACTICA 3: IMPLEMENTACIÓN DE CODIFICADOR DE CANAL
 * ALUMNA: Karla Guadalupe Robert Roa
 * ASIGNACIÓN: 1° CBS (7,3)
 * ---------------------------------------------------------
 * * Matriz Generadora G:
 * [ 1  0  0  1  1  0  1 ]
 * [ 0  1  0  1  0  1  1 ]
 * [ 0  0  1  0  1  1  1 ]

 * * Ecuaciones de Paridad (derivadas de G):
 * - c4 = d1 ^ d2
 * - c5 = d1 ^ d3
 * - c6 = d2 ^ d3
 * - c7 = d1 ^ d2 ^ d3
 * * ---------------------------------------------------------
 *//* * CÓDIGO ETAPA 1: CODIFICADOR (CODER) CBS (7,3)
 * CORREGIDO: TODO CON LÓGICA INVERSA (Barra + Indicadores)
 * - Los LEDs 12 y 13 ahora se apagan con HIGH.
 * - La barra también se apaga con HIGH.
 */
// --- CONFIGURACIÓN DE PINES ---
const int pinD1 = 2; 
const int pinD2 = 3; 
const int pinD3 = 4; 
const int button = A1; 

// INDICADORES SERIALES (Ahora configurados como Active Low)
const int led_cero = 12;   
const int led_uno  = 13;   

// BARRA DE LEDS 
const int pinC7 = 11; 
const int pinC6 = 10; 
const int pinC5 = 9;  
const int pinC4 = 8;  
const int pinC3 = 7;  
const int pinC2 = 6;  
const int pinC1 = 5;  

bool lastButton = LOW; 

void setup() {
  pinMode(pinD1, INPUT);
  pinMode(pinD2, INPUT);
  pinMode(pinD3, INPUT);
  pinMode(button, INPUT_PULLUP); 

  pinMode(led_cero, OUTPUT);
  pinMode(led_uno, OUTPUT);
  pinMode(pinC1, OUTPUT);
  pinMode(pinC2, OUTPUT);
  pinMode(pinC3, OUTPUT);
  pinMode(pinC4, OUTPUT);
  pinMode(pinC5, OUTPUT);
  pinMode(pinC6, OUTPUT);
  pinMode(pinC7, OUTPUT);

  // --- APAGADO INICIAL (TODO EN HIGH) ---
  // Como tus LEDs son Active Low, HIGH significa APAGADO.
  digitalWrite(led_cero, HIGH);
  digitalWrite(led_uno, HIGH);
  
  digitalWrite(pinC1, HIGH); 
  digitalWrite(pinC2, HIGH); 
  digitalWrite(pinC3, HIGH);
  digitalWrite(pinC4, HIGH); 
  digitalWrite(pinC5, HIGH); 
  digitalWrite(pinC6, HIGH); 
  digitalWrite(pinC7, HIGH);

  Serial.begin(9600);
  Serial.println("\n--- PRACTICA 3 KARLA ROBERT  ---");
  Serial.println("\nCOLOCA LA PALABRA DATO Y PRESIONA EL BOTON.");
}

void loop() {
  bool currentButton = digitalRead(button);

  if (lastButton == HIGH && currentButton == LOW) {
      
      // 1. APAGAR BARRA ANTES DE EMPEZAR (HIGH = Apagado)
      digitalWrite(pinC1, HIGH); digitalWrite(pinC2, HIGH); digitalWrite(pinC3, HIGH);
      digitalWrite(pinC4, HIGH); digitalWrite(pinC5, HIGH); digitalWrite(pinC6, HIGH); digitalWrite(pinC7, HIGH);

      // Leer
      int d1 = digitalRead(pinD1);
      int d2 = digitalRead(pinD2);
      int d3 = digitalRead(pinD3);

      // --- AQUÍ MOSTRAMOS EL DATO INGRESADO ---
      Serial.println("\n###############################");
      Serial.print("PALABRA DATO INGRESADA: ");
      Serial.print(d1); Serial.print(d2); Serial.println(d3);
      Serial.println("###############################");
      
      Serial.println("\n HACIENDO TRANSMISIÓN DE BITS");
      // Calcular
      int c1 = d1;
      int c2 = d2;
      int c3 = d3;
      int c4 = d1 ^ d2; 
      int c5 = d1 ^ d3; 
      int c6 = d2 ^ d3; 
      int c7 = (d1 ^ d2) ^ d3; 

      int palabraCodigo[7] = {c1, c2, c3, c4, c5, c6, c7};

      // 2. TRANSMISIÓN SERIAL (Corregida para tus LEDs)
      for(int i = 0; i < 7; i++) {
        int bitActual = palabraCodigo[i];
        
        Serial.print("Bit "); Serial.print(i+1); Serial.print(": "); Serial.println(bitActual);

        if(bitActual == 1) {
           // Si es 1, prendemos led_uno (con LOW) y apagamos led_cero (con HIGH)
           digitalWrite(led_uno, LOW);  
           digitalWrite(led_cero, HIGH); 
        } else {
           // Si es 0, prendemos led_cero (con LOW) y apagamos led_uno (con HIGH)
           digitalWrite(led_uno, HIGH);   
           digitalWrite(led_cero, LOW); 
        }

        delay(1000); 
        
        // APAGAR AMBOS (HIGH = Apagado)
        digitalWrite(led_uno, HIGH);
        digitalWrite(led_cero, HIGH);
        delay(200); 
      }

      Serial.println("FIN");

      // 3. MOSTRAR BARRA
      // Usamos !c1 porque 1 debe volverse 0 (LOW) para prender
      digitalWrite(pinC1, !c1); 
      digitalWrite(pinC2, !c2);
      digitalWrite(pinC3, !c3);
      digitalWrite(pinC4, !c4);
      digitalWrite(pinC5, !c5);
      digitalWrite(pinC6, !c6);
      digitalWrite(pinC7, !c7); 
  }

  lastButton = currentButton;
  delay(50);
}