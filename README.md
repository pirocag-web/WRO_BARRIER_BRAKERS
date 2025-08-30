We are from Colegio El Sagrado Corazón, Naranjo campus, participating in WRO in the Future Engineers category. Our team, Paula Isabel Roca Gálvez and Mariana Isabel Herrera Serna, presents an Arduino-powered car that moves, avoids obstacles with a sensor, follows colors, and parks or stops automatically. 

The software that controls our autonomous vehicle is organized into main modules, each dedicated to a key function. This structure makes the system easier to understand and maintain, allowing each part to handle a specific task and communicate with the others to achieve the desired behavior.


1. Obstacle Sensor Module:
   This module manages data reading from the ultrasonic and infrared sensors installed on the front and sides of the vehicle. It detects objects and walls, calculating distances and sending signals to the navigation system to avoid collisions.


3. Motor Control Module:
   This module translates the decisions of the logic system into concrete movements. It controls the motors that allow it to move forward, backward, turn, and stop. Communication with the motors is done through PWM signals that regulate speed and direction. A servo motor is a small motor that can rotate to a specific position and stay there, controlled by signals from an Arduino. In our vehicle, we use servo motors for two main purposes.

 4. Turning the front wheels:
 The servo motor moves the wheels left or right when the car needs to change direction. This makes turning precise and automatic.
    

 5. Moving the distance sensor:
 Another servo motor adjusts the angle of the distance sensor (ultrasonic), allowing the sensor to point in different directions and detect obstacles not only in front but also to the sides. This helps the car make better decisions when moving and avoiding obstacles.

    6. Navigation logic module:
     Interprets data from the sensors and decides the vehicle's behavior in real time. It integrates obstacle avoidance, line following, and parking functions. If it detects an obstacle, it commands a turn; if it sees the line, it follows it; if it finds the parking zone, it executes the automatic maneuver.
    
     7. Automatic Parking Module:
     Contains the routine that, upon identifying the parking zone by color or distance, performs a sequence of movements to park correctly, simulating a real parking maneuver.
Each software module interacts directly with the vehicle's electromagnetic components, translating programming into real physical actions.

Ultrasonic and Infrared Sensors: Sensor modules send and receive electromagnetic signals (sound and light) to measure distances and detect obstacles. The code interprets this data and determines how the vehicle should move. 

Motors and Servos:
Motor control modules send electrical control signals (PWM) to the motors, which respond by turning the wheels in the direction and speed indicated by the system algorithm. Arduino Mega Board: This is where all the modules are executed. Its input and output pins allow the connection of sensors and motors, receiving and sending electromagnetic signals that transform digital information into physical movements. 

Integration: 
The system communicates internally through variables and functions. The sensors inform the logic module, which decides how to act and gives orders to the motors and servos to execute the movements. Thus, the vehicle responds automatically and adaptively to its environment. 

Process To make the car 
1. Physical assembly of the vehicle Start by assembling the chassis and mounting the Arduino Mega board in a central position. The obstacle sensors are placed on the front and sides, while the color sensor is installed on the bottom, near the front wheels. The motors and servos are fixed to the wheels and axles, ensuring a firm and stable connection. All components are connected to the corresponding pins on the board according to the connection diagram.

2. Preparing the development environment Download and install the Arduino IDE development environment on your computer. Make sure you have the necessary libraries for the sensors and motors, such as Servo.h and the specific library for the color sensor. Download the source code from the WRO_BARRIER_BRAKERS repository and open the main file (.ino).

3. Code Configuration Review the pin and variable definitions in the code, ensuring they correspond to your physical setup. Adjust parameters such as detection distances, color sensitivity, and motor speed according to your car's characteristics.


This project is an example of how robotics integrates software modules and electromagnetic components.
