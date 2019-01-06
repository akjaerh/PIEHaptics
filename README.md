# PIEHaptics

Vision:
We want to explore what it does to relations over a distance, when it becomes possible, and easily available, to induce physical sensations remotely.

Our approach is to create two identical devices that are connected to each other over the internet. A button and a motor are attached to the devices where a button click on the <b>1. device</b> is able to deliver a haptic sensation on the <b>2. device</b> and vice versa.

## Structure of this Git.
This Git-repository will contain the relevant source code and resources necessary for you to make a device identical to what we have made. The code-pieces is structured in a way for you to see what have been done (i.e. added) in the iterations we have gone through. The source code for a respective iteration can be found in folders named in the following way "X. Iteration", where X describes which iteration is at play. A rule of thumb would be to use the source code found in the folder where the X is highest, at that code would be the prettiest.

Below is a brief explanation of the code in each respective iteration:

### 1. Iteration
Source code for using the MQTT-protocol, where the ADAFruit.io was used for a graphical control interface as well as a MQTT-broker.

### 2. Iteration
This iteration primarily focused on subscribing to a MQTT-feed, and when change was is detected courses specific code to be executed. The code executed is activating a vibration motor for a fixed amount of time.

### 3. Iteration
In this iteration we wanted to make the haptic sensations more dynamic. We did this by shifting from the approach using a fixed value of active motor time, to an approach where the lenght a button press on the <b>1. device</b> would determine the length the motor time on the <b>2. device</b>.
This code is programmed for device 1. Remember to change the variables for 'id' and 'idOther' for the 2. device.
Remember also to type in your own Wifi name and password.
The code is made for a device which uses motor B. If you use motor A or both, then remember to uncomment them in the code.

Additionally you will be able to find the .SVG-file for lasercutting the case for the device. This file is named "HAPTICCASE-3MM.svg"

![1. experiment: Good vibrations multiband](images/good%20vibrations%20multi%20band.jpg)

## How to create an internet enabled haptic sensoric sender and receiver

### 1. Preparing the necessary parts and pieces
#### Parts:
* WEMOS D1 Mini (https://wiki.wemos.cc/products:d1:d1_mini) or similar ESP8266 microcontroller 
* Motor Shield (https://wiki.wemos.cc/products:d1_mini_shields:motor_shield) 
* Button
* Motor of your choice (our favourite is the vibration motor)
* Wires
#### Tools:
* Soldering iron and solder
* Laser cutter

![Parts](images/parts.jpg)

#### Fritzing diagram
We have created a solution based on the Fritzing diagram below
![Fritzing diagram](images/vibration.png)


### 2. Soldering the pins and connecting the motor shield

### 3. Soldering on the vibration motor

### 4. Soldering on the button

### 5. Creating the enclosure
![](images/20181112_144646.jpg)
For this exact iteration, we wanted to create an enclosure to strap to the body. For creating and mounting the device, we did the following steps:

- We used a lasercutter to cut the box, and then used a gluegun to put it together. The file for our enclosure, can be found in this repository.
- Next, we put straps inside the enclosure, to make sure that we could wear the device.
- For mounting the device itself, we screwed the top of the button off, put the device into the enclosure, and the disassempled button through the small hole seen in the picture of the enclosure. 

### 6. Putting it all together

