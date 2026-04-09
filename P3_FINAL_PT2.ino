/*
 * PRACTICA 3: IMPLEMENTACIÓN DE CODIFICADOR DE CANAL
 * ALUMNA: Karla Guadalupe Robert Roa
 * ASIGNACIÓN: 1° CBS (7,3)
 * ---------------------------------------------------------
 * * Matriz Generadora G:
 * [ 1  0  0  1  1  0  1 ]
 * [ 0  1  0  1  0  1  1 ]
 * [ 0  0  1  0  1  1  1 ]
 */

// --- AJUSTE DE LÓGICA DE LEDS ---
const int ENCENDIDO = LOW;  
const int APAGADO   = HIGH; 

// --- PINES ---
const int PIN_INICIO = 12; 

// Entradas
const int PIN_DATO[3]   = {2, 3, 4};        
const int PIN_ERROR[7]  = {5, 6, 7, 8, 9, 10, 11}; 

// Salidas (Barras)
const int BARRA_CODIGO[7]   = {22, 23, 24, 25, 26, 27, 28}; 
const int BARRA_RECIBIDA[7] = {30, 31, 32, 33, 34, 35, 36};

// LEDs Seriales y Síndrome
const int LED_C_SERIAL = 52; 
const int LED_R_SERIAL = 53;
const int LED_SINDROME[4] = {40, 41, 42, 43}; 

// Variables
int d[3]; int c[7]; int e[7]; int r[7]; int s[4];       

// Matriz H (1° CBS 7,3)
const int H[4][7] = {
  {1, 1, 0, 1, 0, 0, 0},
  {1, 0, 1, 0, 1, 0, 0},
  {0, 1, 1, 0, 0, 1, 0},
  {1, 1, 1, 0, 0, 0, 1}
};

void limpiarLEDs() {
  for(int i=0; i<7; i++) {
    digitalWrite(BARRA_CODIGO[i], APAGADO);
    digitalWrite(BARRA_RECIBIDA[i], APAGADO);
  }
  for(int i=0; i<4; i++) digitalWrite(LED_SINDROME[i], APAGADO);
  digitalWrite(LED_C_SERIAL, APAGADO);
  digitalWrite(LED_R_SERIAL, APAGADO);
}

void setup() {
  Serial.begin(9600);
  Serial.println("\n=== SISTEMA LISTO CON CORRECCION VISUAL ===");
  Serial.println("Configura DATO y activa PIN 12 (5V) para iniciar.");

  pinMode(PIN_INICIO, INPUT);
  for(int i=0; i<3; i++) pinMode(PIN_DATO[i], INPUT);
  for(int i=0; i<7; i++) pinMode(PIN_ERROR[i], INPUT);

  for(int i=0; i<7; i++) { pinMode(BARRA_CODIGO[i], OUTPUT); pinMode(BARRA_RECIBIDA[i], OUTPUT); }
  for(int i=0; i<4; i++) pinMode(LED_SINDROME[i], OUTPUT);
  pinMode(LED_C_SERIAL, OUTPUT); pinMode(LED_R_SERIAL, OUTPUT);

  limpiarLEDs();
  
  // Parpadeo inicial
  digitalWrite(LED_C_SERIAL, ENCENDIDO); delay(500); digitalWrite(LED_C_SERIAL, APAGADO);
}

void loop() {
  if (digitalRead(PIN_INICIO) == LOW) return;

  Serial.println("\n--------------------------------");
  Serial.println(">>> INICIANDO PROCESO... <<<");
  limpiarLEDs(); 

  // --- 1. LEER DATO ---
  for(int i=0; i<3; i++) d[i] = digitalRead(PIN_DATO[i]);
  
  Serial.print("1. Dato Entrada (d):    ");
  for(int i=0; i<3; i++) Serial.print(d[i]);
  Serial.println();

  // --- 2. CALCULAR CÓDIGO ---
  c[0] = d[0]; c[1] = d[1]; c[2] = d[2];
  c[3] = d[0] ^ d[1]; c[4] = d[0] ^ d[2]; c[5] = d[1] ^ d[2]; c[6] = d[0] ^ d[1] ^ d[2];

  Serial.print("2. Palabra Codigo (c):  ");
  for(int i=0; i<7; i++) Serial.print(c[i]);
  Serial.println("  <-- Enviando...");

  // ANIMACIÓN ENVÍO
  for(int i=0; i<7; i++) {
    int estado = (c[i] == 1) ? ENCENDIDO : APAGADO;
    digitalWrite(LED_C_SERIAL, estado); delay(300); 
    digitalWrite(BARRA_CODIGO[i], estado);
    digitalWrite(LED_C_SERIAL, APAGADO); delay(150);
  }

  Serial.println("   [PAUSA] Introduce ERRORES ahora (5 seg)...");
  delay(5000); 

  // --- 3. LEER ERRORES Y CALCULAR ---
  for(int i=0; i<7; i++) e[i] = digitalRead(PIN_ERROR[i]);
  
  Serial.print("3. Vector Error (e):    ");
  for(int i=0; i<7; i++) Serial.print(e[i]);
  Serial.println("  <-- Leído del DIP");

  // Calcular Recibido CON ERROR (r = c ^ e)
  for(int i=0; i<7; i++) r[i] = c[i] ^ e[i];

  Serial.print("4. Palabra Recibida (r): ");
  for(int i=0; i<7; i++) Serial.print(r[i]);
  Serial.println("  <-- (Contiene Error)");

  // ANIMACIÓN RECEPCIÓN (Muestra lo que llegó, CON error)
  for(int i=0; i<7; i++) {
    int estado = (r[i] == 1) ? ENCENDIDO : APAGADO;
    digitalWrite(LED_R_SERIAL, estado); delay(300);
    digitalWrite(BARRA_RECIBIDA[i], estado);
    digitalWrite(LED_R_SERIAL, APAGADO); delay(150);
  }

  // --- 4. SÍNDROME Y DIAGNÓSTICO ---
  Serial.print("5. Sindrome:             ");
  for(int j=0; j<4; j++) {
    s[j] = 0;
    for(int i=0; i<7; i++) s[j] += r[i] * H[j][i];
    s[j] = s[j] % 2;
    
    digitalWrite(LED_SINDROME[j], (s[j]==1)?ENCENDIDO:APAGADO);
    Serial.print(s[j]);
  }
  Serial.println();

  // Convertir Síndrome a Decimal
  int valS = (s[0]*8) + (s[1]*4) + (s[2]*2) + (s[3]*1);

  // ============================================================
  // === LÓGICA DE CORRECCIÓN (MODIFICA EL VECTOR r) ===
  // ============================================================
  
  if(valS == 0) {
    Serial.println(">> ESTATUS: Mensaje CORRECTO (Sin Error)");
  } 
  else {
    Serial.print(">> ESTATUS: ERROR DETECTADO (S=");
    Serial.print(valS);
    Serial.println(") -> Corrigiendo...");

    // Aplicamos la corrección volteando los bits afectados en 'r'
    // Usamos '!r[i]' para invertir el bit (si es 0 pasa a 1, si es 1 pasa a 0)
    
    switch(valS) {
      // --- ERRORES SIMPLES (Por si acaso pruebas uno solo) ---
      case 13: r[0] = !r[0]; break; // Error en bit 0
      case 11: r[1] = !r[1]; break; // Error en bit 1
      case 7:  r[2] = !r[2]; break; // Error en bit 2
      case 8:  r[3] = !r[3]; break; // Error en bit 3
      case 4:  r[4] = !r[4]; break; // Error en bit 4
      case 2:  r[5] = !r[5]; break; // Error en bit 5
      case 1:  r[6] = !r[6]; break; // Error en bit 6

      // --- TUS ERRORES DOBLES ASIGNADOS ---
      case 6: // Error Doble (1,2) -> Bits 0 y 1
        Serial.println("   -> Corrigiendo par (1,2)");
        r[0] = !r[0]; 
        r[1] = !r[1];
        break;
        
      case 10: // Error Doble (4,6) -> Bits 3 y 5
        Serial.println("   -> Corrigiendo par (4,6)");
        r[3] = !r[3];
        r[5] = !r[5];
        break;
        
      default: 
        Serial.println("   -> Error múltiple no corregible.");
    }

    // === ACTUALIZACIÓN VISUAL DE LOS LEDS ===
    // Ahora que 'r' ya está corregido, actualizamos la barra de LEDs
    delay(1000); // Pequeña pausa dramática antes de corregir
    Serial.println(">> ACTUALIZANDO BARRA DE LEDS CON PALABRA CORREGIDA.");
    
    for(int i=0; i<7; i++) {
      int estado = (r[i] == 1) ? ENCENDIDO : APAGADO;
      digitalWrite(BARRA_RECIBIDA[i], estado);
    }
  }

  Serial.println(">>> FIN. Reinicia el PIN 12. <<<");

  while(digitalRead(PIN_INICIO) == HIGH) { delay(100); }
  delay(1000);
}