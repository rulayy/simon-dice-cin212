#include <Arduino.h>

// CIN-212: Simon Dice con tres colores. No requiere bibliotecas externas.
const byte LEDS[3] = {8, 9, 10};
const byte BOTONES[3] = {2, 3, 4};
const byte BUZZER = 11;
const unsigned int NOTAS[3] = {262, 330, 392};
const byte MAX_PASOS = 100;
const unsigned long ANTIRREBOTE = 30;
byte secuencia[MAX_PASOS]; // Cada elemento contiene 0, 1 o 2.
byte longitud = 0;
byte paso = 0;
byte lecturaAnterior[3] = {HIGH, HIGH, HIGH};
byte estable[3] = {HIGH, HIGH, HIGH};
unsigned long cambio[3] = {0, 0, 0};
unsigned long inicioRespuesta = 0;
bool esperandoSoltar = true;

enum Estado { NUEVO_NIVEL, RESPUESTA };
Estado estado = NUEVO_NIVEL;

void apagarLuces() {
  for (byte i = 0; i < 3; i++) digitalWrite(LEDS[i], LOW);
}

void sonido(unsigned int frecuencia, unsigned int duracion) {
  tone(BUZZER, frecuencia, duracion);
}

// Muestreo sin delay: exige 30 ms estables tanto al pulsar como al soltar.
void actualizarBotones() {
  unsigned long ahora = millis();
  for (byte i = 0; i < 3; i++) {
    byte lectura = digitalRead(BOTONES[i]);
    if (lectura != lecturaAnterior[i]) {
      cambio[i] = ahora;
      lecturaAnterior[i] = lectura;
    }
    if (ahora - cambio[i] >= ANTIRREBOTE) estable[i] = lectura;
  }
}

bool todosSueltos() {
  for (byte i = 0; i < 3; i++) {
    if (estable[i] == LOW || lecturaAnterior[i] == LOW) return false;
    if (millis() - cambio[i] < ANTIRREBOTE) return false;
  }
  return true;
}

// Retorna -1 sin evento, -2 para pulsacion multiple o 0..2 para un color.
// Tras cada evento exige soltar TODOS los botones antes de aceptar otro.
int leerEntrada() {
  if (esperandoSoltar) {
    if (todosSueltos()) esperandoSoltar = false;
    return -1;
  }
  byte cantidad = 0;
  int elegido = -1;
  for (byte i = 0; i < 3; i++) {
    if (estable[i] == LOW) { cantidad++; elegido = i; }
  }
  if (cantidad == 0) return -1;
  esperandoSoltar = true;
  if (cantidad > 1) return -2;
  return elegido;
}

void reproducirSecuencia() {
  Serial.print(F("Nivel: ")); Serial.println(longitud);
  delay(700);
  for (byte i = 0; i < longitud; i++) {
    byte color = secuencia[i];
    digitalWrite(LEDS[color], HIGH);
    sonido(NOTAS[color], 500);
    delay(500);
    digitalWrite(LEDS[color], LOW);
    delay(200);
  }
  // Las pulsaciones durante la muestra no son respuestas validas.
  for (byte i = 0; i < 3; i++) {
    lecturaAnterior[i] = digitalRead(BOTONES[i]);
    estable[i] = lecturaAnterior[i];
    cambio[i] = millis();
  }
  esperandoSoltar = true;
  inicioRespuesta = millis();
  Serial.println(F("Tu turno: repite las luces."));
}

void exito() {
  apagarLuces();
  sonido(523, 150); delay(180);
  sonido(659, 150); delay(180);
  sonido(784, 250); delay(300);
  Serial.println(F("Nivel completado."));
}

void reiniciarPartida() {
  longitud = 0;
  paso = 0;
  for (byte i = 0; i < MAX_PASOS; i++) secuencia[i] = 0;
  esperandoSoltar = true;
  estado = NUEVO_NIVEL;
}

void error() {
  Serial.println(F("Error. Nueva partida desde nivel 1."));
  sonido(130, 900);
  for (byte n = 0; n < 3; n++) {
    for (byte i = 0; i < 3; i++) digitalWrite(LEDS[i], HIGH);
    delay(150);
    apagarLuces(); delay(150);
  }
  noTone(BUZZER);
  reiniciarPartida();
}

void iniciarNivel() {
  // Limite explicito para no escribir fuera del arreglo.
  if (longitud == MAX_PASOS) {
    Serial.println(F("Completaste 100 niveles. Nueva partida."));
    reiniciarPartida();
  }
  secuencia[longitud] = (byte)random(0, 3);
  longitud++;
  paso = 0;
  reproducirSecuencia();
  estado = RESPUESTA;
}

void procesarRespuesta() {
  actualizarBotones();
  // LED encendido mientras el boton permanece presionado y estabilizado.
  for (byte i = 0; i < 3; i++)
    digitalWrite(LEDS[i], estable[i] == LOW ? HIGH : LOW);
  int entrada = leerEntrada();
  if (entrada == -1) return;
  Serial.print(F("Tiempo desde ultimo paso (ms): "));
  Serial.println(millis() - inicioRespuesta);
  inicioRespuesta = millis();
  if (entrada < 0 || entrada != secuencia[paso]) { error(); return; }
  sonido(NOTAS[entrada], 120);
  paso++;
  if (paso == longitud) {
    exito();
    estado = NUEVO_NIVEL;
  }
}

void setup() {
  Serial.begin(9600);
  for (byte i = 0; i < 3; i++) {
    pinMode(LEDS[i], OUTPUT);
    pinMode(BOTONES[i], INPUT_PULLUP);
  }
  pinMode(BUZZER, OUTPUT);
  apagarLuces();
  randomSeed(analogRead(A0)); // Dejar A0 sin conectar en el circuito fisico.
  Serial.println(F("SIMON DICE - rojo, verde, azul"));
}

void loop() {
  if (estado == NUEVO_NIVEL) iniciarNivel();
  else procesarRespuesta();
}
