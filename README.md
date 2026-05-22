## Satellite Workshop by Space Faculty [18-22 May 2026]

 **Purpose:** Learning about satellites and basic subsystems in them.

**Hardware/Software:** 
- Built for Arduino Nano, using a teaching set of a 1U CubeSat
- Uses the old bootloader CH341SER.exe as included in this folder.

**Folders**
- **LoRa_SendbasedonRequest:** Arduino sketch that sends a LoRa packet in response to an incoming request. Open in the Arduino IDE and upload to the satellite OBC board.
- **LoRaRequestdataSend:** Arduino sketch that requests data over LoRa and prints received responses to the Serial Monitor. Upload to the Ground Station board.
- **OBC_satellite_D2:** On-Board Computer sketch for Day 2 lab. Contains basic code used in workshop demos.
- **LoRaWarningsSender:** Code to passively monitor status of the OBC and send warning to ground station if necessary. Also returns status when requested from GS.
- **SerialPlotterExercise:** Simple practice to display TEMPOBC, TEMPEPS and VBAT on the Serial Plotter in Arduino IDE
- **Camera_sample:** Code provided by workshop trainers to test functionality of the camera module on board.
- **Camera_practice:** Own attempt on using both USB Serial and LoRa to transmit and display an image.

**How to use**
- Open the desired `.ino` file in the Arduino IDE (or VS Code with the Arduino extension), select the correct board/port, then upload.
- For LoRa sketches, ensure both sender and receiver use matching LoRa frequency.