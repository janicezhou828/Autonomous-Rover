////move one block until we're inside the loading zone
////turn left once
////move one block, turn right, scan
////if it sees it, open door, move forward, close, turn 180 degrees and go back towards the wall
////turn left once it gets back to the wall
////move up until it's going to hit the wall, turn right, drive one block and then send code to matlab
//
//void loadingZone2() {
//  //need to get robot facing south:
//  turnRight(turnTime);
//  fs();
//  Serial.print("robot has turnedright");
//  readUS();
//  delay(500);
//  if (cm[6] < 30 && (cm[0] - cm[6] > 12)) {
//    blockFound = true;
//    openDoor();
//    int timeToBlock = cm[6] * 160; // Time to reach the block
//    runForward();
//    delay(timeToBlock);
//    fs();
//    closeDoor();
//    blockPickUp = true;
//    case2 = true;
//  }
//  else {
//    turnLeft(turnTime);
//    fs();
//    Serial.print("robot has turned left");
//    readUS();
//    delay(500);
//    Serial.print("US readings collected");
//    if (cm[6] < 15 && (cm[0] - cm[6] > 12)) {
//      blockFound = true;
//      openDoor();
//      int timeToBlock = cm[6] * 160; // Time to reach the block
//      runForward();
//      delay(timeToBlock);
//      fs();
//      closeDoor();
//      blockPickUp = true;
//      case1 = true;
//    }
//    else {
//      runForward();
//      delay(OneBlock);
//      Serial.print("robot running");
//    }
//
//  }
//
//if (!blockPickUp && cm[0] < change_direction) {
//  fs();
//  turnRight(turnTime);
//  fs();
//  if (cm[6] < 15 && (cm[0] - cm[6] > 12)) {
//    blockFound = true;
//    openDoor();
//    int timeToBlock = cm[6] * 200; // Time to reach the block
//    runForward();
//    delay(timeToBlock);
//    fs();
//    closeDoor();
//    blockPickUp = true;
//    case2 = true;
//  }
//  else {
//    turnLeft(turnTime);
//    fs();
//  }
//
//}
//}
