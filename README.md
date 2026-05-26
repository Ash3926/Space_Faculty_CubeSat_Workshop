## Satellite Workshop by Space Faculty [18-22 May 2026]

 **Purpose:** Learning about satellites and basic subsystems in them. [Reference Materials](https://github.com/sbwspacefaculty-png/SatBuildWorkshop)

**Hardware/Software:** 
- OBC and Ground Station codes built for Arduino Nano, using a teaching set of a 1U CubeSat
- ADCS codes built for Arduino Nano Every
- Uses the old bootloader `CH341SER.exe` as included in this folder.
- Camera module is VC0703 Adafruit

**Folders**
- **OBC/ADCS/Ground Station Component Files:** Files to be uploaded to the OBC/ADCS/Ground Station respectively for various purposes. Will be collated into a finalized file for each board in a future commit.
- **Camera Files:** Files for activating the camera module of the CubeSat. Further instruction available in Camera_practice subfolder

**How to use**
- Component files folder has a separate README with instructions.
- Functional Download files can be uploaded directly to their labelled board to run, only the Ground Station needs to remain connected to a laptop with Serial Monitor. Available functions will be displayed on startup and after each command. `*.ino` files need to be in a similarly named folder to open in Arduino IDE and upload.

_**To note:** Debugging prints have been removed from OBC and ADCS final code, as in practice these boards would not have a serial output once launched. Debug print statements can be found in component files._