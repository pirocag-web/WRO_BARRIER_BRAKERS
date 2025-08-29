Somos del Colegio El Sagrado Corazón, campus Naranjo, participamos en WRO en la categoría Futuros Ingenieros. Nuestro equipo, Paula Isabel Roca Gálvez y Mariana Isabel Herrera Serna, presenta un carro con Arduino que se mueve, esquiva obstáculos con sensor, sigue colores y se estaciona o se detiene automáticamente.

El software que controla nuestro vehículo autónomo está organizado en módulos principales, cada uno dedicado a una función esencial. Esta estructura facilita la comprensión y el mantenimiento del sistema, permitiendo que cada parte se encargue de una tarea específica y se comunique con las demás para lograr el comportamiento deseado.

1. Módulo de sensores de obstáculos: 
Este módulo gestiona la lectura de datos desde los sensores de ultrasonido e infrarrojo instalados en el frente y los laterales del vehículo. Permite detectar objetos y paredes, calculando distancias y enviando señales al sistema de navegación para evitar colisiones.

2. Módulo de control de motores:
Este módulo traduce las decisiones del sistema de lógica en movimientos concretos. Controla los motores que permiten avanzar, retroceder, girar y detenerse. La comunicación con los motores se realiza mediante señales PWM que regulan velocidad y dirección.
Un servomotor es un motor pequeño que puede girar hasta una posición específica y quedarse allí, controlado por señales desde Arduino.  
En nuestro vehículo, usamos servomotores para dos cosas principales:

3. Girar las ruedas delanteras:  
   El servomotor mueve las ruedas hacia la izquierda o derecha cuando el carro necesita cambiar de dirección. Así, el giro es preciso y automático.

4. Mover el sensor de distancia: 
   Otro servomotor ajusta el ángulo del sensor de distancia (ultrasónico), permitiendo que el sensor apunte hacia diferentes lados y detecte obstáculos no solo al frente, sino también a los costados. Esto ayuda a que el carro tome mejores decisiones al moverse y evitar obstáculos.

5. Módulo de lógica de navegación: 
Interpreta los datos de los sensores y decide el comportamiento del vehículo en tiempo real. Integra las funciones de evasión de obstáculos, seguimiento de líneas y estacionamiento. Si detecta un obstáculo, ordena un giro; si ve la línea, la sigue; si encuentra la zona de estacionamiento, ejecuta la maniobra automática.

6. Módulo de estacionamiento automático: 
Contiene la rutina que, al identificar la zona de estacionamiento por color o distancia, realiza una secuencia de movimientos para estacionarse correctamente, simulando una maniobra real de estacionamiento.



Cada módulo de software interactúa de manera directa con los componentes electromagnéticos del vehículo, logrando que la programación se traduzca en acciones físicas reales.

Sensores ultrasónicos e infrarrojos: 
  Los módulos de sensores envían y reciben señales electromagnéticas (sonido y luz) para medir distancias y detectar obstáculos. El código interpreta estos datos y determina cómo debe moverse el vehículo.

  
  Motores y servos:  
  Los módulos de control de motores envían señales eléctricas de control (PWM) a los motores, que responden girando las ruedas en la dirección y velocidad indicadas por el algoritmo del sistema.

Placa Arduino Mega: 
  Es el corazón del vehículo, donde se ejecutan todos los módulos. Sus pines de entrada y salida permiten conectar sensores y motores, recibiendo y enviando señales electromagnéticas que transforman la información digital en movimientos físicos.

  Integración:  
  El sistema se comunica internamente mediante variables y funciones. Los sensores informan al módulo de lógica, que decide cómo actuar y da órdenes a los motores y servos para ejecutar los movimientos. Así, el vehículo responde de forma automática y adaptativa a su entorno.
  

Proceso de construcción, compilación y carga en Arduino Mega

1. Montaje físico del vehículo**  
Se empieza por ensamblar el chasis y montar la placa Arduino Mega en una posición central. Los sensores de obstáculos se colocan en la parte frontal y lateral, mientras que el sensor de color se instala en la parte inferior, cerca de las ruedas delanteras. Los motores y servos se fijan a las ruedas y ejes, asegurando una conexión firme y estable. Todos los componentes se conectan a los pines correspondientes de la placa según el diagrama de conexiones.

2. Preparación del ambiente de desarrollo 
Descarga e instala el entorno de desarrollo Arduino IDE en tu computadora. Asegúrate de tener las librerías necesarias para los sensores y motores, como Servo.h y la librería específica para el sensor de color. Descarga el código fuente desde el repositorio WRO_BARRIER_BRAKERS y abre el archivo principal (.ino).

3. Configuración del código  
Revisa la definición de pines y variables en el código, asegurándote que corresponda a tu montaje físico. Ajusta parámetros como distancias de detección, sensibilidad de color y velocidad de motores según las características de tu carro.

4. Compilación del código
En el Arduino IDE, utiliza el botón de “Verificar” para compilar el código y detectar posibles errores de sintaxis o configuración. Si el IDE muestra algún error, revisa las conexiones y las librerías instaladas.

5. Carga del código en la placa 
Conecta la placa Arduino Mega a la PC mediante cable USB. Selecciona el modelo de placa y el puerto en el menú de herramientas. Haz clic en “Subir” para cargar el código. Cuando el proceso finalice, la placa ejecutará automáticamente el programa y el vehículo estará listo para operar.

6. Pruebas y ajustes**  
Coloca el vehículo en el área de prueba y observa su comportamiento. Verifica que los sensores detecten obstáculos y que el sensor de color siga las líneas correctamente. Ajusta parámetros en el código si es necesario y repite el proceso de compilación y carga hasta obtener el resultado deseado.


El proyecto WRO_BARRIER_BRAKERS es un ejemplo didáctico de cómo la robótica integra módulos de software y componentes electromagnéticos para crear vehículos inteligentes. El proceso de construcción y programación en Arduino Mega permite experimentar con tecnología en un entorno educativo. Este enfoque facilita la comprensión y la mejora continua, invitando a estudiantes y aficionados a explorar el fascinante mundo de la robótica y la ingeniería.
