# Il-Matto avrcontroller to draw images (bitmap)

It shows how to draw image on avr-controller ( Eg : ATMega644P)

## Setup the Environment

1. Install AVR 
http://winavr.sourceforge.net/helpme.html
2.Install AVR Plugin from Eclipse
3.Download Zadig (Change Driver to libusb-win32 (v1.2.6.0))
https://zadig.akeo.ie/
3. Make a C Project -> Choose Empty Project under AVR Cross Target Application
4. Set BUild Settings -> Tick Generate Hex File for Flash Memory, Print Size, AVR Dude
5. Set AVR Target Settings -> Choose USBasp
6. For first time user on Windows 10, replace the file C:\WinAVR-20100110\utils\bin\msys-1.0.dll 
to http://www.madwizard.org/download/electronics/msys-1.0-vista64.zip
(This will solve *avr-gcc.exe application error " The application was unable to start correctly(0xc0000142)."*
7. Build and Upload to Target Device TADA!

## Setup the Images

1. Download LCD Image Converter to convert png to bitmap, resize the image if it's too big
https://sourceforge.net/projects/lcd-image-converter/
2. Image Colour R5G6B5 , size 8 bits eg 0xFF
3. Copy the source code 

