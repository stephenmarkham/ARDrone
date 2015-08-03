# ARDrone

drone-control.js is a basic javascript file used to control an ARDrone via the keyboard. It uses node-ar-drone https://github.com/felixge/node-ar-drone/) and nodejs. 

ar_drone.cpp allows an ARDrone to be controlled using individual UDP packets through C++. 
Currently speeds may only be set as {-0.05,-0.1,-0.2,-0.5,0.05,0.1,0.2 or 0.5}.
It can be controlled using the following methods

```
forward(speed)

backward(speed)

up(speed)

down(speed)

left(speed)

right(speed)

rotateRight(speed)

rotateLeft(speed)

hover()

land()

```

Each action will be performed until another is called, therefore the following

```
forward(0.5);
sleep(2);
hover();
```

will go forward for 2 seconds then hover.

A pause of at least 5 seconds should be added after the initialization of the ARDrone to ensure it has time to take off.
A similar pause should be added after the landing call, which will exit the programme.

The Example programme (example.cpp) can be compiled using the cmake using CMakeLists.txt.


-----------------------------------
How it Works
-----------------------------------

The ARDrone needs commands to be send in packets roughly every 50ms through port 5556. For this reason commands are sent on a separate thread to the main programme and is therefore will not time out waiting for the user. 

- Sequential
- float to int
- Different Types and Examples