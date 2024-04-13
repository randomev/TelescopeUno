#include <Arduino.h>
#include <Adafruit_MotorShield.h>

// put function declarations here:
int myFunction(int, int);



// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

//char* input = "#PG_90;10\n"; // replace this with the string read from the serial port
char command[3];
int param1, param2;
unsigned long startTime = 0;
bool isRunning = false;
unsigned long delayTime = 0;

void setup() {
  // put your setup code here, to run once:
  //int result = myFunction(2, 3);
  Serial.begin(9600);

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor->setSpeed(150);
  myMotor->run(FORWARD);
  // turn on motor
  myMotor->run(RELEASE);
}

void testrun() {
    uint8_t i;

  Serial.print("tick");

  myMotor->run(FORWARD);
  for (i=0; i<255; i++) {
    myMotor->setSpeed(i);
    delay(10);
  }
  for (i=255; i!=0; i--) {
    myMotor->setSpeed(i);
    delay(10);
  }

  Serial.print("tock");

  myMotor->run(BACKWARD);
  for (i=0; i<255; i++) {
    myMotor->setSpeed(i);
    delay(10);
  }
  for (i=255; i!=0; i--) {
    myMotor->setSpeed(i);
    delay(10);
  }

  Serial.print("tech");
  myMotor->run(RELEASE);
}

void loop() {
  //testrun();
  //delay(1000);
  //Serial.print(".");

  while (Serial.available() > 0) {
    char command[64]; // Adjust size as needed
    int len = Serial.readBytesUntil('\n', command, sizeof(command) - 1);
    command[len] = '\0'; // Null-terminate the string
    // Now you have a command, you can parse it and execute it
    char cmd[3];
    int param1, param2;
    //Serial.print("Received command: ");
    //Serial.print(command);

    if(sscanf(command, "#%2s_%d;%d\n", cmd, &param1, &param2) == 3) {
        // Successfully parsed command and parameters
        // Now you can use these values to drive your motors
        //Serial.print("Parsed command: ");
        if(strcmp(cmd, "PG") == 0) {
            //Serial.print("Parsed command: ");
            delayTime = param2;
            //Serial.print(cmd);
            //Serial.print(" ");
            //Serial.print(param1);
            //Serial.print(" ; ");
            //Serial.println(delayTime);
            if (param1 == 1)
            {
              myMotor->run(FORWARD);
            }
            else
            {
              myMotor->run(BACKWARD);
            }
            // Set the start time and mark the motor as running
            startTime = millis();
            isRunning = true;
        } else if(strcmp(cmd, "SL") == 0) {
            //Serial.print("Slewing command: ");
            // Handle the Slewing command
            // ...
            Serial.println(isRunning ? "true" : "false");

        } else {
            // Unknown command
            Serial.println("Unknown command");
        }
    } else {
        // Failed to parse the string
        Serial.println("Failed to parse the string");
    }
  }

  // Check if the motor is running and the delay time has passed
  if (isRunning && millis() - startTime >= delayTime) {
    // Stop the motor
    myMotor->run(RELEASE);
    isRunning = false;
    //Serial.println("Motor stopped");
  }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}