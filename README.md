# Haptic_Perception_VR
To develop an immersive VR game that utilizes IMU for object tracking and movement control along with providing haptic feedback that adds more immersion to the experience.

**(CS6650 Smart sensing for Internet of things Course project)**


ESP32 code: 

1) In Arduino IDE, set the board type and port accordingly 
2) Download "WebsocketServer" library from Library manager in IDE 
3) Change the ssid and password in the code according to wifi being used 
4) Upload the code to the development board with the IMU connected properly based on the connection diagram given in the report 


Unity Code: 
 
1) Given "WebsocketClient.cs" code is the main script that gets receives data and transforms the gameObject. 
2) But the entire codebase has to be downloaded for running the program 
3) Unity version used: 2021.3.8f1, XR package used: Oculus Integration, Build: Windows platform build 
4) To run the program, download the unity package provided, import the package in Unity, check for any dependency issue. 
5) In the WebsocketClient.cs file, IP address has to be changed based on the IP address the board is choosing 
6) Load one of the two given scenes(Maze_IMU, Fluid_IMU). Have the ESP32 on and ready, click play in Unity and BOOM there you go!!!!! 
7) For VR setup, machine should have Oculus app installed and Quest 2 has to be connected via Oculus link and played.

### Output:

[![Watch the video](https://i.stack.imgur.com/Vp2cE.png)](https://youtu.be/i0hpC0Y8F1Q)
