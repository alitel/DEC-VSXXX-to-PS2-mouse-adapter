DEC VSXXX serial mouse to PS/2 mouse adapter
--------------------------------------------

The DEC VSXXX-AA (hockey puck/Hawley) and VSXXX-GA serial mice used on some 90s DEC workstations are increasingly hard/expensive to come by.
This project aims to provide instructions to build a (reasonably) cheap Arduino-based alternative adapter allowing the use of a PS/2 mouse with certain DEC vax and alpha workstatoins.
In my case, I have a VAXstation 3100 and a DEC 3000 AXP machine, so I have created both the adapter and the cables to connect it to either machine. Both systems work well.

Please keep in mind, this is a very rough little personal project. I ended up using an Arduino mega because I had one lying around -- it's bulky, but also provides a hardware serial1 port. I was unable to get the SoftwareSerial arduino libraries to work reliably on other Arduinos (eg Uno, nano), especially because the DEC mouse uses odd parity, not supported by the base SoftwareSerial library, so I had to find someone who had extended it (several options were found, but I was unable to get any to work reliably).

An Arduino Nano Every would likely work, as it has a hardware Serial1 (I believe). With a Nano Every a much smaller and more elegant adapter could be fabricated with a pretty simple pcb using proper PS2 and db9 connectors and a basic max232 type ttl-to-rs232 circuit instead of the separate adapter like I've used.

I also tried (briefly) using an ESP8266 dev board that I had lying around, but was unable to get stable operation of the PS2 mouse handling software. Probably doable though, and possibly a cheaper route than the Arduino boards.

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

The ttl-to-rs232 adapter I've used was bought off Amazon:

https://www.amazon.ca/gp/product/B07BJJ7WB9/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1

as was the Arduino Mega proto-board shield (please NOTE: the silkscreen on this board is incorrect, mislabelling the pins in several spots, so ignore it):

https://www.amazon.ca/gp/product/B07DRG2LN2/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1

I also found a DIN-7 cable on Amazon to cut up and use -- there should be options available.

Wiring:
-------

1) Arduino To PS/2 connector:
   Using the PS2 connector pinout diagram in the repo files for reference, connect power and ground to the mega's available 5v and gnd pins, then CLOCK to pin 6 and DATA to pin 5 on the mega. 

3) Arduino to RS232 adapter TTL pins:
   Connect the ttl-rs232 converter vcc and ground to available 5v and GND pins on the mega. Connect ttl-rs232 converter rxd pin to mega pin 18 txd1, ttl-rs232 converter txd pin to mega pin 19 rxd1.

4) Vaxstation: ttl-RS232 converter DB9 connector to VAXStation DIN-7 connector:
   Using the DEC mouse's DB7 connector pinout found in the VCB02 Video Subsystem document in this repo, connect the DIN7 pin 1 to DB9 pin 5 (signal ground), DIN7 pin 2 (TXD in the diagram) to DB9 pin 2, DIN7 pin 3 (RXD) to DB9 pin 3. Short DIN7 pin 7 to DIN7 pin 1 (GND) -- this is used as device detect by the VAXStation.

6) DEC 3000: ttl-RS232 converter DB9 connector to DEC 3000 DB15 connector:
   Using the DEC 3000 keyboard/mouse connector pinout diagram found in the same-named document in this repo, connect DB15 pin 1,5,8,9 or 15 (GND) to DB9 pin 5 (signal ground). Connect DB15 pin 6 (MOUSE RX) to DB9 pin 2, DB15 pin 7 (MOUSE.TX) to DB9 pin 3. There's no specific signal ground it seems on this DB15 connector, and no specific hardware sense pin.

NOTE -- I found the DEC documentation a bit confusing about what is RXD and TXD, but I usually get muddled with RS232 DTE/DCE point-of-view/reference anyway lol, so trial and error eventually works :)




