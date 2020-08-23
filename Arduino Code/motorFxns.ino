void runA(int speed, boolean rev) {
  if (rev) {
    //Establishes backward direction of Channel A
    digitalWrite(12, LOW);
  } else {
    //Establishes forward direction of Channel A
    digitalWrite(12, HIGH);
  }
  digitalWrite(22, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, speed);   //Spins the motor on Channel A at full speed
}

void runB(int speed, boolean rev) {
  if (rev) {
    digitalWrite(13, LOW); //Establishes backwards direction of Channel B
  } else {
    digitalWrite(13, HIGH); //Establishes forward direction of Channel B
  }
  digitalWrite(24, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, speed);   //Spins the motor on Channel B at full speed
}

void runC(int speed, boolean rev) {
  if (rev) {
    digitalWrite(30, LOW); //Establishes backwards direction of Channel C
  } else {
    digitalWrite(30, HIGH); //Establishes forward direction of Channel C
  }
  digitalWrite(26, LOW);   //Disengage the Brake for Channel C
  analogWrite(6, speed);   //Spins the motor on Channel C at full speed
}

//fullstop
void fs() {
  digitalWrite(22, HIGH);
  digitalWrite(24, HIGH);
  digitalWrite(26, HIGH);
}

void runForward() {
  runA(motorSpeed, false);
  runB(motorSpeedB, true);
}

void turnRight(int delayTime) {
  runA(motorSpeed, true);
  runB(motorSpeed, true);
  runC(motorSpeed, true);
  delay(delayTime); //1700
}

void turnLeft(int delayTime) {
  runA(motorSpeed, false);
  runB(motorSpeed, false);
  runC(motorSpeed, false);
  delay(delayTime);
}

void runBackwards() {
  runA(200, true);
  runB(200, false);
}

void runForwardWithCorrection() {
  if (cm[2] < 5.5 || (cm[3] > 7 && cm[3] < 13) || (cm[2] < 10 && cm[4] < 10 && (cm[4] - cm[2] > 1.5)) || (cm[3] < 10 && cm[5] < 10 && (cm[3] - cm[5] > 1.5))) {
    //adjusting if close to right wall
    runA(107, false); //used to be 90
    runB(107, true);
  }
  else if (cm[3] < 5.5 || (cm[2] > 7 && cm[2] < 13) || (cm[3] < 10 && cm[5] < 10 && (cm[5] - cm[3] > 1.5)) || (cm[2] < 10 && cm[4] < 10 && (cm[2] - cm[4] > 1.5))) {
    //adjusting if close to left wall
    runA(100, false);
    runB(114, true); // used to be 90
  }
  else {
    runForward();
  }
}

void turnCorrection() {
  while (!turnComplete) {
    readUS();
    if ((cm[2] < 10 && cm[4] < 10 && cm[4] - cm[2] > 1) || (cm[3] < 10 && cm[5] < 10 && (cm[3] - cm[5] > 1))) {
      turnLeft(100);
      fs();
    }
    else if ((cm[3] < 10 && cm[5] < 10 && cm[5] - cm[3] > 1) || (cm[2] < 10 && cm[4] < 10 && (cm[2] - cm[4] > 1))) {
      turnRight(100);
      fs();
    }
    else {
      turnComplete = true;
    }
  }

}
