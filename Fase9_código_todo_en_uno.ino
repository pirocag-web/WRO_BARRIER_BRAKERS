#include <Servo.h> // encargada de la abstracción de la Modulación por ancho de pulsos.  
// recursos para mostrar color en carro
#include <FastLED.h>
#define NUM_LEDS     1
#define DATA_PIN     4
#define BUTTON_PIN   2
#define BRIGHTNESS   128
#define DEBOUNCE_MS  60
#define CLOCK_PIN 13
CRGB leds[NUM_LEDS];

// Estados: 0 = pink, 1 = Naranja, 2 = tuquuesa
int estado = 0;
bool iniciado = false;           // ¿ya se oprimió al menos una vez?
// Variables para lectura y antirrebote
int valorPulsador = HIGH;        // lectura cruda actual
int valorAnterior = HIGH;        // última lectura cruda
unsigned long tCambio = 0;       // tiempo de último cambio crudo
bool presionadoEstable = false;  // lectura estable actual (true = presionado)
bool presionadoPrev = false;     // lectura estable anterior

int PWMA =6;      // Velocidad motores derecha
int AIN1 =8;     // Dirección motores derecha
int STBY =3;      // Stand-by del puente
int VELOCIDAD =200;   // Velocidad de ensayo (0-255)

Servo miServoA; // Objeto encargado de administrar todas las órdenes a través de él
Servo miServoB; //Mueve al motor ultrasónico

int TRIG_PIN = 13;     // Pin que envía el pulso
int ECHO_PIN = 12;     // Pin que recibe el eco
long TIMEOUT_US = 30000UL; // 30 ms → ~5 m

/* Ángulos de barrido del sensor (0 170°) */
const int MIN_PAN = 0;
const int MAX_PAN = 170;
const int STEP_PAN = 20;

/* ---------- Constantes de control ---------- */
int SPEED_FWD  = 180;   // 0 255
const int SPEED_REV  = 160;   // retroceso lento
const unsigned int SAFE_DIST = 25;   // cm
const unsigned long SCAN_DELAY = 15;  // milisegundos entre mediciones

/* Ángulos de dirección (ajusta a tu mecánica) */
const int CENTER_DIR = 90;
const int LEFT_DIR   = 130;
const int RIGHT_DIR  = 30;

// Pines sensores ultrasónicos laterales cercanos (para evitar paredes cerca)
int trigLateralIzq = 24;
int echoLateralIzq = 25;
int trigLateralDer = 22;
int echoLateralDer = 23;

// Variables para distancias laterales
float lateralIzq = -1;
float lateralDer = -1;


//Varibles para camara ESP32
char lastCode = 0;


void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // botón a GND (pulsado = LOW)
  // Estado inicial: no se ha oprimido → azul
  leds[0] = CRGB::DeepSkyBlue;
  FastLED.show();
  //Serial.println("esperando accion");
     miServoA.attach(11); // Pin 10 o Pin 11.
   miServoB.attach(10); // Mueve al motor ultrasónico

pinMode(PWMA, OUTPUT);
   pinMode(AIN1, OUTPUT);
   pinMode(STBY, OUTPUT);
   digitalWrite(STBY, HIGH);  // Activa el TB6612

   miServoA.write(90);
   miServoB.write(90);

   //Serial.println("===== Test de motores iniciado =====");

   pinMode(TRIG_PIN, OUTPUT);
   pinMode(ECHO_PIN, INPUT);
   digitalWrite(TRIG_PIN, LOW);          // Asegura nivel bajo inicial
   //Serial.println("HC-SR04 listo…");

   // Pines sensores laterales cercanos
   pinMode(trigLateralIzq, OUTPUT);
   pinMode(echoLateralIzq, INPUT);
   pinMode(trigLateralDer, OUTPUT);
   pinMode(echoLateralDer, INPUT);

}

void loop() {
  // -------- Lectura con antirrebote (debounce) --------
  int lectura = digitalRead(BUTTON_PIN);

  if (lectura != valorAnterior) {          // cambio "bruto"
    tCambio = millis();
    valorAnterior = lectura;
  }

  if ((millis() - tCambio) > DEBOUNCE_MS) {
    // Fijar lectura estable (con INPUT_PULLUP, LOW = presionado)
    if (lectura == LOW) {
      presionadoEstable = true;
    } else {
      presionadoEstable = false;
    }

    // -------- Logs estilo solicitado con if (valorPulsador == HIGH) / else --------
    // (Imprime solo cuando cambia el estado estable para no saturar el puerto serie)
    if (presionadoEstable != presionadoPrev) {
      valorPulsador = lectura;            // reflejar lectura cruda
      if (valorPulsador == HIGH) {
        //Serial.println(" esperando accion");
      } else {
       // Serial.println(" pulsado");
      }
    }

    // -------- Detección de flanco (avanza solo una vez por pulsación) --------
    if (presionadoEstable == true && presionadoPrev == false) {
      if (iniciado == false) {
        iniciado = true;      // primera pulsación: entra al estado 0 (rojo)
        estado = 0;
      } else {
        estado = (estado + 1) % 3;   // 0→1→2→0...
      }
    }

    presionadoPrev = presionadoEstable;
  }

  // -------- Salida final de color usando SOLO if/else --------
  if (iniciado == false) {
    // Aún no se ha oprimido: 
    leds[0] = CRGB::Pink;
  } else {
    if (estado == 0) {
      leds[0] = CRGB::LimeGreen;        // Estado 1: Paredes
      paredes();
      // colocar aqui cualquier accion que dese realizar en la opicón 1.
      //Lo ideal es unicamente crear una función para no saturar de codigo esta parte.

    } else {
      if (estado == 1) {
        leds[0] = CRGB::Orange;   // Estado 2: DetectarColores
        detectarColores();
          // colocar aqui cualquier accion que dese realizar en la opicón 2.
          //Lo ideal es unicamente crear una función para no saturar de codigo esta parte.
      } else {
        leds[0] = CRGB::Turquoise;    // Estado 3: detner y alinear
        detener();
        centerAll();
          // colocar aqui cualquier accion que dese realizar en la opicón 3.
          //Lo ideal es unicamente crear una función para no saturar de codigo esta parte.
      }
    }
  }

  FastLED.show();
}


// Función  de control Adelante motores del lado derecho
void atrasMD(int VELOCIDAD)  
{
 analogWrite(PWMA, VELOCIDAD);
 digitalWrite(AIN1, HIGH);
}
// Función de control  Atras motores del lado derecho
void adelanteMD(int SPEED_FWD)  
{
 analogWrite(PWMA, VELOCIDAD);
 digitalWrite(AIN1, LOW);
}

void detener() {
  analogWrite(PWMA, 0);
  digitalWrite(STBY, LOW);      // Desactiva brevemente el puente
  delay(10);
  digitalWrite(STBY, HIGH);
  //Serial.println(F("Motores detenidos"));
}

float medirDistanciaCm(int trigPin, int echoPin) {
  /* 1. Pulso de disparo: 10 µs en alto */
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  /* 2. Duración del eco con control de tiempo máximo */
  long dur = pulseIn(echoPin, HIGH, TIMEOUT_US);
  if (dur == 0) return -1.0;            // Sin eco → error/rango excedido
  /* 3. Conversión a cm (velocidad del sonido ~343 m/s a 20 °C) */
  float distancia = dur / 58.0;         // 58 µs ≈ 1 cm
  /* 4. Verificación de límites prácticos */
  if (distancia < 2 || distancia > 400) return -1.0;
  return distancia;
}

/* ---------- Lectura de distancia ---------- */
unsigned int readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL); // 30 ms ⇒ ~5 m
  if (duration == 0) return 500; // fuera de rango

  return duration / 58; // μs → cm
}

int scanBestAngle() {
  unsigned int bestDist = 0;
  int bestAngle = 90;

  for (int angle = MIN_PAN; angle <= MAX_PAN; angle += STEP_PAN) {
    miServoA.write(angle);
    delay(150); // tiempo para que el servo llegue y el eco se estabilice
    unsigned int d = readDistance();
    if (d > bestDist) {
      bestDist = d;
      bestAngle = angle;
    }
    delay(SCAN_DELAY);
  }
  return bestAngle; // devuelve el ángulo con más espacio
}

/* ---------- Funciones auxiliares ---------- */
void centerAll() {
  miServoA.write(90);
  miServoB.write(CENTER_DIR);
}
/* ---------- Funcion para Leer datos de la ESP32 ---------- */
char leerESP32(){

 // Reenvía crudo todo lo que entre por RX1 al Monitor Serie
  while (Serial.available() > 0) {
     char c = Serial.read();
return c;
   }
}


void paredes() {
  /* 1. Avance normal */
  miServoB.write(90);
  adelanteMD(SPEED_FWD);
  miServoA.write(90); // mirada frontal

  unsigned int d = readDistance();

  // Leer distancias laterales cercanas
  lateralIzq = medirDistanciaCm(trigLateralIzq, echoLateralIzq);
  lateralDer = medirDistanciaCm(trigLateralDer, echoLateralDer);

 // Serial.print("Frontal: ");
  //Serial.print(d);
  //Serial.print(" cm | Lateral Izq: ");
  //Serial.print(lateralIzq);
  //Serial.print(" cm | Lateral Der: ");
  //Serial.println(lateralDer);

  // Evitar paredes laterales cercanas (menos de 12 cm)
  if (lateralIzq > 0 && lateralIzq < 15) {
   // Serial.println(F("Pared cerca a la izquierda, giro suave a la derecha"));
    miServoB.write(110); // gira un poco a la derecha
    delay(200);
  }
  else if (lateralDer > 0 && lateralDer < 15) {
    //Serial.println(F("Pared cerca a la derecha, giro suave a la izquierda"));
    miServoB.write(60);  // gira un poco a la izquierda
    delay(200);
  }
  else {
    miServoB.write(CENTER_DIR);
  }

  if (d < SAFE_DIST) {
    /* 2. Obstáculo detectado */
    detener();
    //Serial.print(F("Obstáculo a "));
    //Serial.print(d);
    //Serial.println(F(" cm → Escanear..."));

    /* 3. Barrido panorámico */
    int bestAngle = scanBestAngle();
   // Serial.print(F("Mejor ángulo: "));
    //Serial.println(bestAngle);

    /* 4. Toma de decisión */
    if (bestAngle < 70) {                 // zona izquierda
    //  Serial.println(F("Giro a la derecha"));
      miServoB.write(RIGHT_DIR);
      delay(700);
      adelanteMD(SPEED_FWD);
      delay(700);
    }
    else if (bestAngle > 100) {                 // zona derecha
     // Serial.println(F("Giro a la izquierda"));
      miServoB.write(LEFT_DIR);
      delay(700);
      adelanteMD(SPEED_FWD);
      delay(700);
    }
    else {
      /* No hay salida clara: retroceder */
     // Serial.println(F("Sin salida → Retroceso"));
      atrasMD(SPEED_REV);
      delay(600); //originalmente estaba a 500
    }

    /* 5. Reajustar */
    detener();
    centerAll();
  }

  delay(50); // bucle suave
}


void esquivarVerde(){
  /*  Obstáculo detectado */
detener(); 
  miServoB.write(LEFT_DIR);
      delay(700);
      adelanteMD(SPEED_FWD);
      delay(400);

}

void esquivarRojo(){
   /*  Obstáculo detectado */
detener(); 
      miServoB.write(RIGHT_DIR);
      delay(700);
      adelanteMD(SPEED_FWD);
      delay(400);

}

void detectarColores()
{
  //codigo para manejar con colores
char letra =leerESP32();

    // Normaliza a mayúscula
    if (letra >= 'a' && letra<= 'z') letra = char(letra - 'a' + 'A');

    // Solo actúa si cambia el código (evita spam)
    if (letra != lastCode) {
      lastCode = letra;

      switch (letra) {
        case 'V':
         // codigo de lo que quieren hacer al ver verde
         Serial.println(letra); // esquivar a la izquierda
         leds[0] = CRGB::Green;
        FastLED.show();
        esquivarVerde();

          break;
        case 'R':
 // codigo de lo que quieren hacer al ver rojo
         Serial.println(letra); // esquivar a la derecha. 
         leds[0] = CRGB::Red;
        FastLED.show();
        esquivarRojo(); 
          break;
        case 'A':
 // codigo de lo que quieren hacer al ver azul
         Serial.println(letra); //accion de contar 
         leds[0] = CRGB::Blue;
        FastLED.show();
        paredes();
          break;
        case 'M':
 // codigo de lo que quieren hacer al ver magenta
         Serial.println(letra); //accion de parquear
         leds[0] = CRGB::Magenta;
        FastLED.show();
        paredes();
          break;
          case 'O':
 // codigo de lo que quieren hacer al no ver ningun color 
         Serial.println(letra); // accion de moverse adelante
           leds[0] = CRGB::Black;
        FastLED.show();
        paredes();
          break;
          }
  }
  
}
