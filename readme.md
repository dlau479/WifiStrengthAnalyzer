# Wifi Strength Analyzer 

The purpose of the project is to measure the strength of a wifi access point that you are connected to. 

To make use of this you will need 

1. Arduino
2. Laptop
3. Cable to connect to your computer and Arduino

Before uploading the code make sure to create a header class called 

```c++
    secret.h
```

In this header file you must then put in your wifi credentials.

```c++
    #define SECRET_SSID "Your ssid name"
    #define SECRET_PASS "Your ssid password"
```

Afterwards you can now upload the code into the Arduino. Next run the code. There are two places you need to put your attention to. 

The first being that the termianl or console on your side. The notifications you will  get are that the Arduino is waiting to connect to the Wifi. The thing that thie program will do is print out the local IP address. 

The next thing to do is to open up a web browser. With the web browser are are to connect to the local ip address given to you by the code. I by default have binded the local ip address to port 80. 

When you input the local ip address and port. You will see a basic webpage that will collect ip addresses and will find the min, mean and maximum values of your wifi. 

With this you can carry around your laptop with arduino to different spots to measure the wifi strength in your area.