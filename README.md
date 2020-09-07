# Autonomous-Rover

[image1]: ./Images/Rover_Maze.jpg "Rover in the maze"
[image2]: ./Images/Rover.jpg "Rover"
[image3]: ./Images/Localization.jpg "Localization"
[image4]: ./Images/VideoCapture.jpg "Video"

The goal of the project is to design and build a rover that can navigate a given maze without contacting the maze walls, locate itself within the maze, pick up an object, and drop it off in a designated area of the maze. The video above documents our final test.


Our team used an Arduino Mega as our microcontroller board, three omni wheels for the movement, a sweeper mechanism for object pickup and drop-off, and seven ultrasonic sensors for the detection of the surrounding environment. We have modelled and implemented a finite state machine to acheive the given tasks.


The localization strategy relied on the translation of the sensor data to information on the surrounding wall patterns and the generation of a probability map in MATLAB. To achieve localization at a random spot and orientation, the robot first aligns itself with the maze walls. Then, our localization algorithm on MATLAB would evaluate all four possible initial headings (N,S,E,W) simultaneously and quickly converge on the correct location.


In our final test, our rover completed all the tasks effectively and achieved the top score in the class. 

Pictures of the Rover
---
Here are some pictures of the final rover:

![alt text][image2]

![alt text][image1]


Localization in MATLAB
---

![alt text][image3]


Project Video
---
Here's a [link to my project video](./Images/Autonomous_Rover.mp4)
![alt text][image4]