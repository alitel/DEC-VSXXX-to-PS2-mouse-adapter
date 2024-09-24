DEC VSXXX serial mouse to PS/2 mouse adapter
--------------------------------------------

The DEC VSXXX-AA (hockey puck/Hawley) and VSXXX-GA serial mice used on some 90s DEC workstations are increasingly hard and expensive to come by.
This project aims to provide instructions to build a (reasonably) cheap Arduino-based alternative adapter allowing the use of a PS/2 mouse with certain DEC vax and alpha workstatoins.
In my case, I have a VAXstation 3100 and a DEC 3000 AXP machine, so I have created both the adapter and cables to connect it to both machines. Both systems work well.

Please keep in mind, this is a very rough little personal project. I ended up using an Arduino mega because I had one lying around -- it's bulky, but also provides a hardware serial1 port. I was unable to get the SoftwareSerial arduino libraries to work reliably on other Arduinos (eg Uno, nano). An Adruino Nano Every would likely work, and a MUCh smaller and more elegant adapter could be fabricated with a pretty simple pcb using a proper PS2 and db9 connector and a basic max232 type ttl-to-rs232 circuit instead of the separate adapter like I've used.

I also tried (briefly) using an ESP8266 dev board that I had also lying around, but was unable to get stable operation of the PS2 handling software. Probably doable though, and possibly a cheaper route than the Arduino boards.

This project is a personal adaptation to the work already done by David Kuder. All credit for the design goes to them. Their work can be found at:

https://hackaday.io/project/19576-dec-mouse-adapter.

PS/2 Mouse and Serial DB9 pinouts can be found here (png files also included in this repository):

https://www.networktechinc.com/technote.html

The full DEC VCB02 Video Subsystem Technical Manual can be found here (I have included the mouse section in this repo): 

https://ia600203.us.archive.org/16/items/104aatm1/104aatm1_text.pdf

The full DEC 3000 Alpha AXP system pinout document can be found here (I have included the 15-pin mouse/keyboard connector in this repo):

https://www.manualslib.com/manual/1744465/Digital-Equipment-Dec-3000-Series.html?page=392#manual

The PS/2 Mouse handling portion of the project (talking to the arduino) is taken directly from this post, and all credit goes to the original creator:

https://bytesnbits.co.uk/arduino-ps2-mouse/

The code used in this project for ps2 mouse communication can be obtained at their github: 

https://github.com/getis/Arduino-PS2-Mouse-Handler



