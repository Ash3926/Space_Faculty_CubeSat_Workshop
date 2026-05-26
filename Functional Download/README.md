## ADCS_Final.ino
This file should be uploaded to the Arduino Nano Every on the Attitude Determination Control System of the satellite.

The program passively checks for sun sensor readings every 5 seconds, updating the sun sensor feedback flag every time the readings are checked. At every loop() run, it checks if the satellite is experiencing a shake and responds accordingly. 

It actively checks for serial command input from the OBC, and provides the corresponding response on the following functions:
- `TEMPADCS`: Returns the temperature of ADCS board
- `SUN` : Triggers the `rotatenow()` function to turn the satellite to face away from the Sun.
- `POS_State` : Returns the position state of the satellite based on sunsensor values (0 if no movement needed, 1 if needed to rotate left, 2 if needed to rotate right)
- `IMUDATA` : Returns **ALL** IMU data
- `DETUMBLE` : Triggers the `detumbling()` function to rotate the satellite clockwise and counterclockwise repeatedly. Used to stabilize the satellite in case of any collisions. Triggers once at startup to stabilize after the expected initial separation from launch rocket.

## GS_Final.ino
This file should be uploaded to the Arduino Nano on the Ground Station. The Ground Station board should be connected to a laptop with the IDE open to access Serial Monitor. Constantly listens for data sent from the Satellite OBC, and transmits commands when input is detected.

After sending the `PHOTO` command, the `image_receiver.py` file should be opened on the laptop to listen for data and receive the image.

## OBC_Final.ino
This file should be uploaded to the Arduino Nano on the On-Board Computer of the satellite.

The program passively checks for high board temperatures and low battery power, sending a warning if detected. Communicates with ADCS over serial for the necessary data .

It actively listens for any commands from the Ground Station over LoRa telemetry, and provides the corresponding response on the following functions:
- `ALL` : Returns data of the satellite in the form `[timestamp] [team name] TEMPOBC | TEMPEPS | TEMPADCS | VBAT | Camera_pointing_status`
- `TEMPOBC` : Returns the temperature of the OBC board
- `TEMPEPS` : Returns the temperature of the EPS (Electrical Power System) board
- `TEMPADCS` : Returns the temperature of the ADCS board
- `VBAT` : Returns the remaining battery charge in V
- `PHOTO` : Takes a photo from the satellite and transmits the raw data back in bytes. There is a 10 second delay upon receiving the command to ensure sufficient time to switch port access to the `image_receiver.py` program at GS.
- `POS_State` : Returns the camera pointing status
- `SUN` : Triggers rotation in response to an incorrect pointing status
- `IMUDATA` : Returns the data of the IMU
- `DETUMBLE` : Triggers the detumbling of the satellite
- `HELP` : Displays list of available commands