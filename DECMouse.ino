/*
 * an arduino sketch to interface with a ps/2 mouse.
 * Also uses serial protocol to talk back to the vaxstation host
 * and report what it finds.
 */

// NOTES:
// On arduino mega, purple wire ttl-rs232 converter rxd pin -- connect to pin 18 txd1,
// grey ttl-rs232 converter txd pin connect to mega pin 19 rxd1 -- ie crossover is here)
// On nano/uno connect purple rxd to pin 3 txd, grey txd wire to pin 2 rxd
// No luck yet using software serial library -- vaxstation does not see mouse, though commands are received.

#include <PS2MouseHandler.h>
//#include <SoftwareSerialParity.h>

#define MOUSE_DATA 5
#define MOUSE_CLOCK 6

PS2MouseHandler mouse(MOUSE_CLOCK, MOUSE_DATA, PS2_MOUSE_REMOTE);

// Set up a new SoftwareSerial object
//const byte VS_SERIAL_RX_PIN = 2;
//const byte VS_SERIAL_TX_PIN = 3;
//SoftwareSerialParity Serial1 (VS_SERIAL_RX_PIN, VS_SERIAL_TX_PIN);

void setup()
{
  //Serial.begin(57600);
  delay(150);

  // Define pin modes for VS_SERIAL_TX/RX
  //pinMode(VS_SERIAL_RX_PIN, INPUT);
  //pinMode(VS_SERIAL_TX_PIN, OUTPUT);

  // Set the baud rate for the mouse (with odd parity)
  Serial1.begin(4800, SERIAL_8O1);
  delay(150);

  if(mouse.initialise() != 0) {
    if(Serial) Serial.println("PS/2 Mouse error!");
  }

  if(Serial) {
    Serial.println(mouse.device_id());
    Serial.println("Init complete.");
  }
}

//
// Main program loop
//

unsigned long lastRunRepCnt = millis();
unsigned long reportCount = 0; // track number of reports sent per min for calibrating to actual mouse hardware
uint8_t DECmode = 'T'; // start mouse in test mode to send report on startup

void loop()
{

  char mstat;
  char mx;
  char my;

/*
  if(Serial) {
    mouse.get_data();
    Serial.print(mouse.device_id()); // device id
    Serial.print(":");
    Serial.print(mouse.status()); // Status Byte
    Serial.print(":");
    Serial.print(mouse.x_movement()); // X Movement Data
    Serial.print(",");
    Serial.print(mouse.y_movement()); // Y Movement Data
    Serial.print(",");
    Serial.print(mouse.z_movement()); // Z Movement Data - scroll wheel
    Serial.print(",");
    Serial.print(mouse.button(0));
    Serial.print(",");
    Serial.print(mouse.button(1));
    Serial.print(",");
    Serial.print(mouse.button(2));
    Serial.print(",");
    Serial.print(mouse.clicked(0));
    Serial.print(",");
    Serial.print(mouse.clicked(1));
    Serial.print(",");
    Serial.print(mouse.clicked(2));
    Serial.println();
  }  
*/
  

  // Track number of reports being generated each min. Max 95/s in prompt (D) mode, 55/s in stream (R) mode
  if( millis() - lastRunRepCnt > 5000) {
    lastRunRepCnt = millis();
    if(Serial) {
      Serial.print("Mouse Mode: ");
      Serial.print(char(DECmode));
      Serial.print(", reports/s: ");
      Serial.println(reportCount/5);
    }
    reportCount = 0;
  }

  if(Serial1.available()) {
    if(Serial) Serial.print("Command received: ");

    uint8_t incomingByte = Serial1.read();
    switch(incomingByte) {
      case 'R':
        DECmode = 'R';
        if(Serial) Serial.print("Stream Mode\r\n");
        break;
      case 'P':
        DECmode = 'P';
        if(Serial) Serial.print("Poll Read\r\n");
        break;
      case 'D':
        DECmode = 'D';
        if(Serial) Serial.print("Poll Mode\r\n");
        break;
      case 'T':
        DECmode = 'T';
        if(Serial) Serial.print("Self Test\r\n");
        break;
      default:
        if(Serial) {
          Serial.print(incomingByte, HEX);
          Serial.print(" -- Unknown command - send self test report\r\n");
          DECmode = 'T';
        }
        break;
    }
  }

  switch(DECmode) {
    case 'R':
    case 'P':
      // get a reading from the mouse
      mouse.get_data();
      mstat = mouse.status(); // Status Byte
      mx = mouse.x_movement(); // X Movement Data
      my = mouse.y_movement(); // Y Movement Data
      if(mx & 0x80) mx ^= 0x7f;
      if(my & 0x80) my ^= 0x7f;
      mx ^= 0x80;
      my ^= 0x80;
      //
      // TODO: in stream mode 'R', only send if mouse position or buttons have changed
      //
      // Send DEC Serial Mouse Packet
      Serial1.write(0x80 | ((mx & 0x80)>>3) | ((my & 0x80) >> 4) | ((mstat & 0x1) << 2) | (mstat & 0x2) | ((mstat & 0x4) >> 2));
      Serial1.write(mx & 0x7f);
      Serial1.write(my & 0x7f);
      reportCount++;
      if(DECmode == 'P') DECmode = 'D'; // return to prompt mode if responding to 'P' command -- stay in stream mode 'R' if in that mode
      break;
    case 'T':
      Serial1.write(0xA0);  // Self Test Report, REV0
      Serial1.write(0x02);  // Manufacturing ID 0, Mouse
      Serial1.write((byte)0x00);  // No Errors
      Serial1.write((byte)0x00);  // No Buttons Held
      if(DECmode == 'T') DECmode = 'D'; // return to prompt mode
      break;
  }

  // These may need tweaking -- 
  // in Prompt mode (D): up to 95 reports/s (on req)
  // in Stream mode (R) : up to 55 reports/s on mouse state change 
  DECmode == 'D' ? delay(6) : delay(12); 

}

