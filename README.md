## Satellite Workshop by Space Faculty [18-22 May 2026]

 **Purpose:** Learning about satellites and basic subsystems in them. [Reference Materials](https://github.com/sbwspacefaculty-png/SatBuildWorkshop)

**Hardware/Software:** 
- OBC and Ground Station codes built for Arduino Nano, using a teaching set of a 1U CubeSat
- ADCS codes built for Arduino Nano Every
- Uses the old bootloader `CH341SER.exe` as included in this folder.
- Camera module is VC0703 Adafruit

**Folders**
- **Component Files:** Component files for each board. Each file only fulfills one subtask and is not useful for a functional satellite
- **Functional Download:** Compiled file for each board based on the component files code. 

Each folder has its own README with further documentation.

_**To note:** Debugging prints have been removed from OBC and ADCS final code, as in practice these boards would not have a serial output once launched. Debug print statements can be found in component files._