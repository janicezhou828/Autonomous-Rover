void closeDoor(){ //was openDoor

  //will need to change small angle to less than -30 degrees
  servo_test.attach(2); 
  for (angle = 50; angle >= -10; angle -= 1) // command to move from 100 degrees to 10 degrees
  {
    servo_test.write(angle);  //command to rotate the servo to the specified angle
    delay(2);
  }
  servo_test.detach();
}

void openDoor(){ //was closeDoor
  servo_test.attach(2);
  for (angle = -10; angle <= 50; angle += 1) // command to move from 10 degrees to 100 degrees
    {
      servo_test.write(angle); //command to rotate the servo to the specified angle
      delay(5);
    }
    servo_test.detach();
}
