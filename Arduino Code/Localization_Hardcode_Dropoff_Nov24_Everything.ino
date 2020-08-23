#include <NewPing.h>
//#include <SoftwareSerial1.h>
#include <Servo.h>
//SoftwareSerial BTSerial(10, 11); // RX | TX

#define SONAR_NUM     7 // Number of sensors.
#define MAX_DISTANCE 200 // 200 Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).
#define TRIGGER_PIN 10
#define TRIGGER_PIN2 7
#define TRIGGER_PIN3 5
#define TRIGGER_PIN4 44
#define TRIGGER_PIN5 45
#define TRIGGER_PIN6 46
#define TRIGGER_PIN7 8
#define ECHO_PIN 32
#define ECHO_PIN2  34
#define ECHO_PIN3 36
#define ECHO_PIN4  38
#define ECHO_PIN5  40
#define ECHO_PIN6  42
#define ECHO_PIN7 47
#define motorSpeed  100
#define motorSpeedB  109 //correct motor B when running forward
#define OneBlock 4300 //time to travel 1 block
#define HalfBlock 2000
int change_direction = 17;
int allowed = 30; // allowable direction
int turnTime = 1900; //1650
bool tooclose = false;

// ---------------------------------------------------------------------------------------------------------
float cm[SONAR_NUM];         // Where the ping distances are stored.
float median[SONAR_NUM];
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.
Servo servo_test;
int angle = 0;
bool initial;
bool loadingZone;
bool blockPickUp;
bool zeroStage;
bool firstStage;
bool secondStage;
bool thirdStage;
bool fourthStage;
int turnInterval;
bool leftScan;
bool rightScan;
bool comingFromBottom;
bool comingFromTop;
bool scan;
bool stopRobot;
bool turnComplete;
bool blockFound;
bool loadingZoneExited;
bool returnToOriginalPosition;
char dir;
bool case1 = false;
bool case2 = false;

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  //[0]: front, [1]: back, [2]: right, [3]: left, [4]: right back, [5]: left back, [6]: block sensor
  NewPing(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE),
  NewPing(TRIGGER_PIN3, ECHO_PIN3, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(TRIGGER_PIN4, ECHO_PIN4, MAX_DISTANCE),
  NewPing(TRIGGER_PIN5, ECHO_PIN5, MAX_DISTANCE),
  NewPing(TRIGGER_PIN6, ECHO_PIN6, MAX_DISTANCE),
  NewPing(TRIGGER_PIN7, ECHO_PIN7, MAX_DISTANCE)
};

//calibration values for US sensors
float conversion[SONAR_NUM][2] = {
  {0.0177, 0.9478}, //front
  {0.0173, 2.4013}, //back
  {0.0168, -0.6628}, //right front
  {0.0182, -1.1491}, //left front
  {0.0181, -0.8816}, //right back
  {0.0172, -0.684}, //left back
  {0.0175, 2.55} //door sensor
};
//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


float distance1 = 0;
float distance2 = 0;
float distance3 = 0;
float distance4 = 0;
float distance5 = 0;
float distance6 = 0;


char state = '0';
int i = 0;
bool blockAhead1 = false;
bool blockAhead2 = false;
bool blockAhead3 = false;
bool blockSearchSuccess = false;
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  //motor setup
  //DO NOT CHANGE PIN VALUES FOR MOTOR
  pinMode(13, OUTPUT); //Initiates Motor Channel B pin -- right of front
  pinMode(24, OUTPUT);  //Initiates Brake Channel B pin
  //setup Channel A
  pinMode(12, OUTPUT); //Initiates the Motor Channel A pin -- left of front
  pinMode(22, OUTPUT); //Initiates the Brake channel A pin
  //setup Channel C
  pinMode(30, OUTPUT);
  pinMode(26, OUTPUT);
  // servo_test.attach(2); // attach the signal pin of servo to pin9 of arduino
  fs(); //prevents sudden surge to one of the motors
  initial = false;
  blockPickUp = false;
  secondStage = false;
  delay(2000);
  closeDoor();
  delay(2000);
}

void loop() {

  if (Serial1.available() > 0) { // Checks whether data is comming from the serial port
    state = Serial1.read(); // Reads the data from the serial port
  }
  /////////////////////////////////////////////////////////////////////////////////////
  if (state == 'A') { //"D:Data collection"
    for (int i = 0; i < SONAR_NUM; i++) {
      median[i] = sonar[i].ping_median(5); //median off 5 values
      if (i == 0 || i == 1) {
        cm[i] = sonar[i].convert_cm(median[i]); //convert that to cm, replace "cm" with "in" for inches
      }
      else {
        cm[i] = conversion[i][0] * median[i] + conversion[i][1];
      }
    }
    distance1 = cm[0];
    distance2 = cm[1];
    distance3 = cm[2];
    distance4 = cm[3];
    distance5 = cm[4];
    distance6 = cm[5];


    if (cm[0] > allowed) {
      dir = 'w';
    }
    else if (cm[2]  > allowed ) {
      dir = 'd';
    }
    else if (cm[3] > allowed) {
      dir = 'a';
    }
    else {
      dir = 's';
    }

    //  printFxn();

    // transmitt data for mutiple times to guarantee MATLAB receives at least one
    // for (int i = 0; i < 1; i++) {
    Serial1.print(distance1);
    Serial1.print("!");
    Serial1.print(distance2);
    Serial1.print(",");
    Serial1.print(distance3);
    Serial1.print(";");
    Serial1.print(distance4);
    Serial1.print(":");
    Serial1.print(distance5);
    Serial1.print("?");
    Serial1.print(distance6);
    Serial1.print("#");
    Serial1.println(dir);
    //}

  }


  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (state == 'B') { // "M:Moving"

    if (dir == 'w') {
      //move forward 1 block;
      turnComplete = false;
      fs();
      turnCorrection();
      delay(20);
      runForward();
      delay(OneBlock);//
      //adjust
      fs();
      turnComplete = false;
      fs();
      turnCorrection();
      delay(20);
      fs();
      state = '9'; //"break"
    }
    else if (dir == 'd' ) {
      //turn right 90
      turnComplete = false;
      fs();
      turnCorrection();
      delay(20);
      turnRight(turnTime);
      fs();
      delay(1000);
      turnComplete = false;
      fs();
      turnCorrection();
      delay(20);
      //move forward 1 block;
      runForward();
      delay(OneBlock);//
      fs();
      //adjust
      turnComplete = false;
      fs();
      turnCorrection();
      delay(20);
      fs();
      state = '9'; //"break"
    }
    else if (dir == 'a') {
      //turn left 90
      turnComplete = false;
      fs();
      turnCorrection();
      delay(20);
      turnLeft(turnTime);
      fs();
      delay(1000);
      turnComplete = false;
      fs();
      turnCorrection();
      delay(20);
      //move forward 1 block;
      runForward();
      delay(OneBlock);//

      //adjust
      fs();
      turnComplete = false;
      fs();
      turnCorrection();
      delay(20);
      state = '9'; //"break loop and go to idle"
    }
    else if (dir == 's') {
      //turn right 180
      turnComplete = false;
      fs();
      turnCorrection();
      delay(20);
      turnRight(turnTime);
      fs();
      delay(500); turnComplete = false;
      fs();
      turnCorrection();
      delay(20);
      turnRight(turnTime);
      fs();
      delay(500); turnComplete = false;
      fs();
      turnCorrection();
      delay(20);
      //move forward 1 block;
      runForward();
      delay(OneBlock);//
      //adjust
      fs();
      turnComplete = false;
      fs();
      turnCorrection();
      delay(20);
      state = '9'; //"break loop and go to idle"
    }
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////Hardcode command//////////////////////////////


  //go forward if receive g----------------------------------------------------------------------------------------
  if (state == 'g') {

    while (tooclose == false) {

      // insert ultrasonic sensor code
      readUS();

      if (cm[0] < change_direction) {
        fs();
        delay(50);
        state = '9';
        for (int i = 0; i <= 50; i++) {
          Serial1.print('9');
          Serial1.println('!');
          tooclose = true;
        }
      }
      else {
        if (cm[2] < 5.5 || (cm[3] > 7 && cm[3] < 13) || (cm[2] < 10 && cm[4] - cm[2] > 1.5) || (cm[3] < 10 && (cm[3] - cm[5] > 1.5))) {
          //adjusting if close to right wall
          runA(110, false); //used to be 90
          runB(107, true);
        }
        else if (cm[3] < 5.5 || (cm[2] > 7 && cm[2] < 13) || (cm[3] < 10 && cm[5] - cm[3] > 1.5) || (cm[2] < 10 && (cm[2] - cm[4] > 1.5))) {
          //adjusting if close to left wall
          runA(100, false);
          runB(114, true); // used to be 90
        }
        else {
          runForward();
        }
      }
    }
    tooclose = false;
  }

  // turn left if receive l-------------------------------------------------------------------------------------
  else if (state == 'l') {

    turnLeft(turnTime);
    turnComplete = false;
    fs();
    turnCorrection();
    delay(20);
    state = '9';
    for (int i = 0; i <= 50; i++) {
      Serial1.print('9');
      Serial1.println('!');
    }
  }

  // turn right if receive r------------------------------------------------------------------------------------
  else if (state == 'r') {
    turnRight(turnTime);
    turnComplete = false;
    fs();
    turnCorrection();
    delay(20);
    state = '9';
    for (int i = 0; i <= 50; i++) {
      Serial1.print('9');
      Serial1.println('!');
    }
  }


  //move one block forward if receive n-------------------------------------------------------------------------
  else if (state == 'n') {
    //move one block forward
    turnComplete = false;
    turnCorrection();
    runForward(); //do we want it to be run with correction?
    delay(OneBlock);
    fs();
    turnComplete = false;
    fs();
    turnCorrection();
    delay(50);
    state = '9';
    for (int i = 0; i <= 50; i++) {
      Serial1.print('9');
      Serial1.println('!');
      //    }
    }
  }

  //move two blocks forward if receive m------------------------------------------------------------------------
  else if (state == 'm') {
    turnComplete = false;
    turnCorrection();
    runForward(); //runwithCorrection?
    delay(OneBlock * 2);
    fs();
    delay(50);
    state = '9';
    for (int i = 0; i <= 50; i++) {
      Serial1.print('9');
      Serial1.println('!');
    }
  }

  //go backward one block if receive h------------------------------------------------------------------------------
  else if (state == 'h') {
    runBackwards();
    delay(OneBlock / 2);
    fs();
    delay(50);
    state = '9';
    for (int i = 0; i <= 50; i++) {
      Serial1.print('9');
      Serial1.println('!');
    }
  }

  //move half block if receive p----------------------------------------------------------
  else if (state == 'p') {
    turnComplete = false;
    turnCorrection();
    runForward();
    delay(3500);
    fs();
    delay(50);
    state = '9';
    for (int i = 0; i <= 50; i++) {
      Serial1.print('9');
      Serial1.println('!');
    }
  }

  else if (state == 'o') {
    openDoor();
    state = '9';
    for (int i = 0; i <= 50; i++) {
      Serial1.print('9');
      Serial1.println('!');
    }
  }


  else if (state == 'c') {
    closeDoor();
    state = '9';
    for (int i = 0; i <= 50; i++) {
      Serial1.print('9');
      Serial1.println('!');
    }
  }

  //last move to the loading zone if receive z---------------------------------------------------------------
  else if (state == 'z') {
    //move one block forward to be at the edge of the loading zone

    bool blockSearchSuccess = false;
    //move one block forward to be at the edge of the loading zone
    fs();
    turnComplete = false;
    turnCorrection();
    delay(50);
    runForward();
    delay(OneBlock);
    fs();
    turnComplete = false;
    turnCorrection();
    delay(50);
    Blink();
    for (int i = 0; i <= 50; i++) {
      Serial1.print('9');
      Serial1.println('!');

    }
    //checking to see if the block is right at the entrance of the loading zone. if it is [blcokAhead1] , will grab it now
    for (int j = 0; j <= 5; j++) {
      readUS();
      if (cm[6] < 35 && (cm[0] - cm[6] > 12)) {
        blockAhead1 = true;
      }
    }
    if (blockAhead1 == true) {
      Blink();
      openDoor();
      int timeToBlock = cm[6] * 160; // Time to reach the block
      runForward();
      delay(timeToBlock);
      fs();
      closeDoor();
      //      blockPickUp = true;
      for (int i = 0; i <= 50; i++) {
        Serial1.print('8');
        Serial1.println('!');
        blockSearchSuccess = true;
      }
    }

    //if there is no block at the entrance, robot enters and faces south, then lower statement executes
    else {
      runForward();
      delay(OneBlock);
      turnComplete = false;
      turnCorrection();
      turnLeft(turnTime);
      fs();
      delay(200);

      // scan across and ahead until block is found

      for (int j = 0; j <= 5; j++) {
        readUS();
      }




      while (cm[0] > 10) {
        turnRight(turnTime);
        fs();
        delay(200);
        for (int j = 0; j <= 5; j++) {
          readUS();
        }
        if  (cm[6] < 30 && (cm[0] - cm[6] > 12))  {
          blockAhead2 = true;
          Blink();
          openDoor();
          int timeToBlock = cm[6] * 160; // Time to reach the block
          runForward();
          delay(timeToBlock);
          fs();
          closeDoor();
          //blockPickUp = true;
          for (int i = 0; i <= 50; i++) {
            Serial1.print('8');
            Serial1.println('!');
            blockSearchSuccess = true;
          }
          break;
        }
        else {
          turnLeft(turnTime);
          fs();
          delay(200);
          for (int j = 0; j <= 5; j++) {
            readUS();
          }
          if  (cm[6] < 15 && (cm[0] - cm[6] > 12)) {
            blockAhead3 = true;
            Blink();
            openDoor();
            int timeToBlock = cm[6] * 160; // Time to reach the block
            runForward();
            delay(timeToBlock);
            fs();
            closeDoor();
            //blockPickUp = true;
            for (int i = 0; i <= 50; i++) {
              Serial1.print('7');
              Serial1.println('!');
              blockSearchSuccess = true;
            }
            break;
          }
          else {
            runForward();
            delay(800);//each increment
            fs();
            delay(200);
            for (int j = 0; j <= 5; j++) {
              readUS();
            }
          }
        }

      }

      if (blockSearchSuccess == false) {  //in case it didnt see the block and needs assistance
        runBackwards();
        delay(1500);
        fs();
        delay(50);
        for (int j = 0; j <= 5; j++) {
          readUS();
        }
        if  (cm[6] < 15 && (cm[0] - cm[6] > 12)) {
          blockAhead3 = true;
          Blink();
          openDoor();
          int timeToBlock = cm[6] * 160; // Time to reach the block
          runForward();
          delay(timeToBlock);
          fs();
          closeDoor();
          //blockPickUp = true;
          for (int i = 0; i <= 50; i++) {
             Serial1.print('7');
            Serial1.println('!');
            blockSearchSuccess = true;
          }
        }


      }
    }


  }
}







//////////////////////////////////////////////////////////////












