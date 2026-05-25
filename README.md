## Satellite Workshop by Space Faculty [18-22 May 2026]

 **Purpose:** Learning about satellites and basic subsystems in them.

**Hardware/Software:** 
- OBC codes built for Arduino Nano, using a teaching set of a 1U CubeSat
- ADCS codes built for Arduino Nano Every
- Uses the old bootloader CH341SER.exe as included in this folder.
- Camera module is VC0703 Adafruit

**Folders**
- **OBC/ADCS/Ground Station Component Files:** Files to be uploaded to the OBC/ADCS/Ground Station respectively for various purposes
- **Camera Files:** Files for activating the camera module of the CubeSat. Further instruction available in Camera_practice subfolder

**How to use**
- Open the desired `.ino` file in the Arduino IDE (or VS Code with the Arduino extension), select the correct board/port, then upload.
- For LoRa sketches, ensure both sender and receiver use matching LoRa frequency.