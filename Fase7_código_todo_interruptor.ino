int PWMA =6;      // Velocidad motores derecha
int AIN1 =8;     // Dirección motores derecha
int STBY =3;      // Stand-by del puente
int VELOCIDAD =150;   // Velocidad de ensayo (0-255)
#include <Servo.h> // encargada de la abstracción de la Modulación por ancho de pulsos.  
Servo miServoA; // Objeto encargado de administrar todas las órdenes a través de él
Servo miServoB; //Mueve al motor ultrasónico
//Servo panServo;// pin 10
//Servo dirServo; //pin 11
int TRIG_PIN = 13;     // Pin que envía el pulso
int ECHO_PIN = 12;     // Pin que recibe el eco
long TIMEOUT_US = 30000UL; // 30 ms → ~5 m
/* Ángulos de barrido del sensor (0 170°) */
const int MIN_PAN = 0;
const int MAX_PAN = 170;
const int STEP_PAN = 20;
/* ---------- Constantes de control ---------- */
int SPEED_FWD  = 190;   // 0 255
const int SPEED_REV  = 170;   // retroceso lento
const unsigned int SAFE_DIST = 30;   // cm
const unsigned long SCAN_DELAY = 50;  // milisegundos entre mediciones
/* Ángulos de dirección (ajusta a tu mecánica) */
const int CENTER_DIR = 90;
const int LEFT_DIR   = 150;
const int RIGHT_DIR  = 30;
// Define color sensor pins
const int  s0= 38;
const int  s1= 40;
const int  s2= 42;
const int  s3= 44;
const int  out= 46;

// Variables for Color Pulse Width Measurements
int rojo = 0;  
int verde = 0;  
int azul = 0;


#include <FastLED.h>
#define NUM_LEDS 1
#define DATA_PIN 4
#define CLOCK_PIN 13
#define BUTTON_PIN   2
#define BRIGHTNESS   128
#define DEBOUNCE_MS  60
CRGB leds[NUM_LEDS];
int pulsadorPin = 2;
int valorPulsador =0;

// Estados: 0 = Rojo, 1 = Naranja, 2 = Verde
int estado = 0;
bool iniciado = false;           // ¿ya se oprimió al menos una vez?

// Variables para lectura y antirrebote
int valorAnterior = HIGH;        // última lectura cruda
unsigned long tCambio = 0;       // tiempo de último cambio crudo
bool presionadoEstable = false;  // lectura estable actual (true = presionado)
bool presionadoPrev = false;     // lectura estable anterior

void setup() {
    Serial.begin(9600); 
  pinMode(s0,OUTPUT);  
  pinMode(s1,OUTPUT);  
  pinMode(s2,OUTPUT);  
  pinMode(s3,OUTPUT);  
  pinMode(out,INPUT);   
  digitalWrite(s0,HIGH);  
  digitalWrite(s1,HIGH);  
 
   miServoA.attach(11); // Pin 10 o Pin 11.
   miServoB.attach(10); // Mueve al motor ultrasónico
   Serial.begin(9600);
   pinMode(PWMA, OUTPUT);
   pinMode(AIN1, OUTPUT);
   pinMode(STBY, OUTPUT);
   digitalWrite(STBY, HIGH);  // Activa el TB6612
   miServoA.write(90);
   miServoB.write(90);
   Serial.println("===== Test de motores iniciado =====");
   pinMode(TRIG_PIN, OUTPUT);
   pinMode(ECHO_PIN, INPUT);
   digitalWrite(TRIG_PIN, LOW);          // Asegura nivel bajo inicial
   Serial.println("HC-SR04 listo…");

 
   FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS);
   //Activamos los pines de entrada y salida
 pinMode(pulsadorPin, INPUT);

  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(BUTTON_PIN, INPUT_PULLUP);  // botón a GND (pulsado = LOW)

  // Estado inicial: no se ha oprimido → azul
  leds[0] = CRGB::DeepSkyBlue;
  FastLED.show();
  Serial.println("esperando accion");
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
        Serial.println(" esperando accion");
      } else {
        Serial.println(" pulsado");
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

  // -------- Salida de color usando SOLO if/else --------
  if (iniciado == false) {
    // Aún no se ha oprimido: azul
    leds[0] = CRGB::DeepSkyBlue;
  } else {
    if (estado == 0) {
      leds[0] = CRGB::Red;        // Estado 1: rojo
      todo();
    } else {
      if (estado == 1) {
        leds[0] = CRGB::Orange;   // Estado 2: naranja
      } else {
        leds[0] = CRGB::Green;    // Estado 3: verde
      }
    }
  }

  FastLED.show();
  
}



   void color()  
{    
  digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);   
  rojo = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s3, HIGH);   
  azul = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s2, HIGH);    
  verde = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
}

 
// Función  de control Adelante motores del lado izquierdo
void adelanteMD(int VELOCIDAD)  
{
 analogWrite(PWMA, VELOCIDAD);
 digitalWrite(AIN1, HIGH);
}
// Función de control  Atras motores del lado izquierdo
void atrasMD(int SPEED_FWD)  
{
 analogWrite(PWMA, VELOCIDAD);
 digitalWrite(AIN1, LOW);
}

void detener() {
  analogWrite(PWMA, 0);
  digitalWrite(STBY, LOW);      // Desactiva brevemente el puente
  delay(10);
  digitalWrite(STBY, HIGH);
  Serial.println(F("Motores detenidos"));
}


// Function to read Red Pulse Widths
int getrojo() {
  // Set sensor to read Red only
  digitalWrite(s2,LOW);
  digitalWrite(s3,LOW);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(out, LOW);
  // Return the value
  return PW;
}

// Function to read Green Pulse Widths
int getverde() {
  // Set sensor to read Green only
  digitalWrite(s2,HIGH);
  digitalWrite(s3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(out, LOW);
  // Return the value
  return PW;
}

// Function to read Blue Pulse Widths
int getazul() {
  // Set sensor to read Blue only
  digitalWrite(s2,LOW);
  digitalWrite(s3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(out, LOW);
  // Return the value
  return PW;
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


void todo(){
    /* 1. Avance normal */
  miServoB.write(90);
  atrasMD(SPEED_FWD);
  miServoA.write(90); // mirada frontal
  unsigned int d = readDistance();
    if (d < SAFE_DIST) {
    /* 2. Obstáculo detectado */
    detener();
    Serial.print(F("Obstáculo a "));
    Serial.print(d);
    Serial.println(F(" cm → Escanear..."));

        /* 3. Barrido panorámico */
    int bestAngle = scanBestAngle();
    Serial.print(F("Mejor ángulo: "));
    Serial.println(bestAngle);

        /* 4. Toma de decisión */
    if (bestAngle < 70) {                 // zona izquierda
      Serial.println(F("Giro a la derecha"));
      miServoB.write(RIGHT_DIR);
      delay(700);
      atrasMD(SPEED_FWD);
      delay(500);
    }
       if (bestAngle > 110) {                 // zona derecha
      Serial.println(F("Giro a la izquierda"));
      miServoB.write(LEFT_DIR);
      delay(700);
      atrasMD(SPEED_FWD);
      delay(500);
    }
        else {
      /* No hay salida clara: retroceder */
      Serial.println(F("Sin salida → Retroceso"));
      adelanteMD(SPEED_REV);
      delay(200); //originalmente estaba a 500
    }
   
    /* 5. Reajustar */
    detener();
    centerAll();
  }

  delay(50); // bucle suave
  color(); 
  Serial.print("   ");  
  Serial.print(rojo, DEC);  
  Serial.print("   ");  
  Serial.print(verde, DEC);  
  Serial.print("   ");  
  Serial.print(azul, DEC);  

  if (rojo < azul && verde > azul && rojo < 35) //35  --88
  {  
   Serial.println("   Rojo");    
  }   
  else if (azul < rojo && azul < verde && verde < rojo)  
  {  
   Serial.println("   Azul");        
  }  

  else if (rojo > verde && azul > verde )  
  {  
   Serial.println("   Verde");       
  }  
  else{
  Serial.println("  ");  
  }
  delay(300);  

// Read Red Pulse Width
  rojo = getrojo();
  // Delay to stabilize sensor
  delay(10);

  // Read Green Pulse Width
  verde = getverde();
  // Delay to stabilize sensor
  delay(10);

  // Read Blue Pulse Width
  azul = getazul();
  // Delay to stabilize sensor
  delay(10);

  if (rojo < verde && rojo < azul) {
    Serial.println("Color detected: Red");
    miServoB.write(RIGHT_DIR);
  delay (200);
  } else if (verde < rojo && verde < azul) {
   miServoB.write(LEFT_DIR);
  delay (200);
    Serial.println("Color detected: Green");
  } else {
    Serial.println("Color detected is not Red or Green");
  }

}
