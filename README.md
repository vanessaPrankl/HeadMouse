# HeadMouse
 
This project provides hardware, enclosure and software for the head-motion based computermouse HeadMouse.
<img src="https://github.com/user-attachments/assets/242b319d-4295-4035-bf34-ba87881bf833" width="250">
<img src="https://github.com/user-attachments/assets/53edb21c-fba6-442c-be43-0cbd3da14713" width="240">

## Hardware
A BOM and PCB-files are provided in the hardware folder. A 600mAh LiPo-battery has been used for the prototyping, the battery only lasts for 3.5h in this case which is mainly caused by the high power consumption of the ESP32-S3-WROOM buetooth module. 

<img src="https://github.com/user-attachments/assets/e8dc3c74-9c7b-420b-9847-541bd62fd511" width="250">

## Software
The following external libraries are utilized:
| Library | Provider | Licence |  
|------|----------|---------|
| [BleMouse](https://github.com/T-vK/ESP32-BLE-Mouse) | [T-vK](https://github.com/T-vK) | MIT License |
| [AdafruitBNO055](https://github.com/adafruit/Adafruit_BNO055) | [Adafruit](https://github.com/adafruit) | MIT License |
| [ESP32_TimerInterrupt](https://github.com/khoih-prog/ESP32TimerInterrupt) | [khoih-prog](https://github.com/khoih-prog) | MIT License|

Additionally, a HeadMouse library including major HeadMouse functions is provided. 

<img src="https://github.com/user-attachments/assets/3bfb5a4b-99bb-4097-b9dc-a7019b962e08" width="500">

The hardware pins are configured in the hm_board_config_v1_0.hpp file of the headmouse library.
The default preferences are set in default_preferences.hpp (include folder of project).

## Enclosure
The enclosure consists of 2 3D-printed parts, 2 screws and according nuts for assembly and a sticky clip for mounting the deivce on the user's head. 

<img src="https://github.com/user-attachments/assets/691f40d2-dd48-4273-9170-20e9b692c577" width="500">

