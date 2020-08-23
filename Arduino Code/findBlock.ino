//void findBlock() {
//  int scanInterval = 200;
//  int scanIncrements = 4;
//  while (!returnToOriginalPosition) {
//    if (stopRobot) {
//      fs();
//    }
//    readUS();
//    //if block is detected
//    if ((cm[6] < 20 && (cm[0] - cm[6] > 12)) && !blockFound) {
//      blockFound = true;
//      openDoor();
//      int timeToBlock = cm[6] * 200; // Time to reach the block
//      runForward();
//      delay(timeToBlock);
//      fs();
//      closeDoor();
//      blockPickUp = true;
//      case1 = true;
//
//      //return back to original position when block was sensed
//      runBackwards();
//      delay(timeToBlock);
//      for (int i = 0; i < turnInterval; i++) {
//        if (!leftScan) {
//          turnRight(scanInterval);
//          fs();
//          delay(250);
//        }
//        else if (!rightScan && leftScan) {
//          turnLeft(scanInterval);
//          fs();
//          delay(250);
//        }
//      }
//      //might need to set the stage that it's in to false and then go to the next stage?
//      scan = false;
//      returnToOriginalPosition = true;
//    }
//    //if block has not been detected, need to scan and navigate loading zone
//    //scans left, returns to original position than scans to the right
//    else {
//      if (scan) {
//        stopRobot = true;
//        if (cm[2] < 8 && !zeroStage) {
//          rightScan = true;
//        }
//        if (cm[3] < 8 && !zeroStage) {
//          leftScan = true;
//        }
//        if (!leftScan) {
//          if (turnInterval < scanIncrements) {
//            turnLeft(scanInterval);
//            fs();
//            turnInterval++;
//          }
//          else if (turnInterval == scanIncrements) {
//            leftScan = true; //left scan is done
//            for (int i = 0; i < scanIncrements; i++) {
//              turnRight(scanInterval);
//              fs();
//              delay(250);
//            }
//            turnInterval = 0;
//          }
//        }
//        else {
//          if (!rightScan) {
//            if (turnInterval < scanIncrements) {
//              turnRight(scanInterval);
//              turnInterval++;
//              fs();
//            }
//            else if (turnInterval == scanIncrements) {
//              fs();
//              rightScan = true; //right scan is done, return to starting position
//              for (int i = 0; i < scanIncrements; i++) {
//                turnLeft(scanInterval);
//                fs();
//                delay(250);
//              }
//              turnInterval = 0;
//              scan = false;
//              fs();
//            }
//          }
//        }
//      }
//      //once a scan is complete, the robot can move
//      if ((leftScan && rightScan) || blockPickUp) {
//        if (zeroStage) { //outside loading zone - move into loading zone after scanning
//          //fix setup
//          leftScan = false;
//          rightScan = false;
//          runForward();
//          delay(OneBlock);//arbitrary value need to verify what is needed to run forward by one block
//          fs();
//          zeroStage = false;
//          firstStage = true; //move to first stage
//          if (!blockPickUp) {
//            scan = true;
//          }
//        }
//        else if (firstStage) {
//          leftScan = false;
//          rightScan = false;
////          if (cm[2] > 20) { //coming from bottom, heading direction = 90
////            turnRight(turnTime);
////            fs();
////            turnComplete = false;
////            turnCorrection();
////            comingFromBottom = true;
////            comingFromTop = false;
////          }
////          else if (cm[3] > 20) { //coming from top, heading direction = 180
//            turnLeft(turnTime);
//            fs();
//            turnComplete = false;
//            turnCorrection();
////            comingFromTop = true;
////            comingFromBottom = false;
////          }
//          firstStage = false;
//          secondStage = true;
//          if (!blockPickUp) {
//            scan = true;
//          }
//        }
//        else if (secondStage) {
//          scan = false;
//          runForwardWithCorrection();
//          stopRobot = false;
//          if (cm[0] < change_direction) {
//            if (cm[2]  > 15) { //coming from the bottom or heading direction = 0
//              turnRight(turnTime);
//              fs();
//              turnComplete = false;
//              turnCorrection();
//              fs();
//            }
//            else if (cm[3] > 15) {//coming from the top or heading direction = 180
//              turnLeft(turnTime);
//              fs();
//              turnComplete = false;
//              turnCorrection();
//              fs();
//            }
//            secondStage = false;
//            leftScan = false;
//            rightScan = false;
//            if (!blockPickUp) {
//              scan = true;
//            }
//            thirdStage = true;
//          }
//        }
//        //run straight and then make third turn
//        else if (thirdStage) {
//          scan = false;
//          runForwardWithCorrection();
//          stopRobot = false;
//          if (cm[0] < change_direction)
//          {
////            if (comingFromBottom) { //coming from the bottom or heading direction = 90
////              turnLeft(turnTime); //need to add turnCorrection functions here
////              fs();
////              turnComplete = false;
////              turnCorrection();
////              fs();
////            }
////            else if (comingFromTop) {//coming from the top or heading direction = 0
//              turnRight(turnTime);
//              fs();
//              turnComplete = false;
//              turnCorrection();
//              fs();
////            }
//            leftScan = false;
//            rightScan = false;
//            thirdStage = false;
//            fourthStage = true;
//            if (!blockPickUp) {
//              scan = true;
//            }
//          }
//        }
//        else if (fourthStage) {
//          //basically just continue until you sense walls on both sides of the robot then you know you've exited the loading zone
//          runForward();
//          if (cm[2] < 8 && cm[3] < 8) {
//            delay(OneBlock);
//            fs();
//            loadingZoneExited = true;
//          }
//        }
//      }
//    }
//  }
//}
