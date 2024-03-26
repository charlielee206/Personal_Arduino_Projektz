/*
This is basically a Copy-Paste of two projects:
"Arduino PS2 Mouse to Amiga Adapter" by glitterkitty
https://github.com/glitterkitty/Arduino-PS2-Mouse-to-Amiga-Adapter

and "mac plus ps2" by trekawek.
https://github.com/trekawek/mac-plus-ps2

I only changed the pinouts and merged the two sketches.
All credit for the sketch goes to those two original creators.
*/

/*
Connections:
Arduino digital pins:
D2: Keyboard-Input-CLK (Pin 5 on the Mini-DIN Connector)
D3: Keyboard-Input-Data (Pin 1 on the Mini-DIN Connector)

D4: Keyboard-Output-CLK (Pin 2 on the RJ10 Socket)
D5: Keyboard-Output-Data (Pin 3 on the RJ10 Socket)

D6: Mouse-Output-YA (Pin 9 on the DB9 Connector)
D7: Mouse-Output-YB (Pin 8 on the DB9 Connector)
D8: Mouse-Output-Button (Pin 7 on the DB9 Connector)
D9: NC (Pin 6 on the DB9 Connector)
D10: Mouse-Output-XB (Pin 5 on the DB9 Connector)
D11: Mouse-Output-XA (Pin 4 on the DB9 Connector)
D12: NC (Pin 3 on the DB9 Connector)

D13: Mouse-Input-Data (Pin 1 on the Mini-DIN Connector)
A0: Mouse-Input-Data (Pin 5 on the Mini-DIN Connector)


Pin Diagrams because I'm too lazy to add an image:
All of the images are pins when looking at the connector straight-on to the mating face.


Mini-DIN 6 (PS/2)
Female Connector
       ______
      /      \
   __|  1  2  |__
  /      __      \
  |  3  |  |  4  |
  |  5  |__|  6  |
  \_____/__\_____/ 

  1: Data
  2: NC
  3: GND
  4: Vcc
  5: CLK
  6: NC

//==========//

RJ10(4P4C) (Macintosh Keyboard)
Female Socket
 ___________
|-|1|2|3|4|-|
|           |
|           |
|           |
|---|   |---|
|   |___|   |
|-----------|    

1: GND
2: CLK
3: Data
4: Vcc

//==========//

DB9 (Macintosh Mouse)
Male Socket

_________________________
\                       /
 \  1   2   3   4   5  /
  \                   /
   \  6   7   8   9  /
    \_______________/

1: GND
2: Vcc
3: NC
4: XA (H-Pulse)
5: XB (HQ-Pulse)
6: NC
7: Button
8: YB (VQ-Pulse)
9: YA (V-Pulse)

*/

#include <PS2Mouse.h>

#define MOUSE_DATA  13
#define MOUSE_CLOCK A0

PS2Mouse mouse(MOUSE_CLOCK, MOUSE_DATA, STREAM);



// quadrature encoding used by the amiga mouse protocol
//
uint8_t H[4]  = { LOW, LOW, HIGH, HIGH};
uint8_t HQ[4] = { LOW, HIGH, HIGH, LOW};

uint8_t QX = 3;
uint8_t QY = 3;

uint8_t XSTEPS;
uint8_t YSTEPS;
uint8_t XSIGN;
uint8_t YSIGN;



#define ADELAY  150



// pins from amiga-port to arduino 
//
#define V_PULSE  6 //YA
#define H_PULSE  11 //XA
#define VQ_PLSE  7 //YB
#define HQ_PLSE  10 //XB

#define LMB      8
#define RMB      8
#define MMB      8



// MB handling
//
void LeftButtonUp() {
    digitalWrite(LMB, HIGH);
}
void LeftButtonDown() {
    digitalWrite(LMB, LOW);
}

void RightButtonUp() {
    digitalWrite(RMB, HIGH);
}
void RightButtonDown() {
    digitalWrite(RMB, LOW);
}

void MiddleButtonUp() {
    digitalWrite(MMB, HIGH);
}
void MiddleButtonDown() {
    digitalWrite(MMB, LOW);
}



// x/y handling
//
void AMIGAHorizontalMove() {
  
    // set bits acc. to curr. position in quadr. sequence
    digitalWrite(H_PULSE, H[QX]);
    digitalWrite(HQ_PLSE, HQ[QX]);

    delayMicroseconds(ADELAY);
}


void AMIGAVerticalMove() {
  
    digitalWrite(V_PULSE, H[QY]);
    digitalWrite(VQ_PLSE, HQ[QY]);
    
    delayMicroseconds(ADELAY);
}


void AMIGA_Left() {
    
    // do a move by setting the port
    AMIGAHorizontalMove();

    // advance in the quadr. sequence
    QX = (QX >= 3) ? 0 : ++QX;
    
}

void AMIGA_Right() {
    AMIGAHorizontalMove();
    QX = (QX <= 0) ? 3 : --QX;
}

void AMIGA_Down() {
    AMIGAVerticalMove();
    QY = QY <= 0 ? 3 : --QY;
}

void AMIGA_Up() {
    AMIGAVerticalMove();
    QY = QY >= 3 ? 0 : ++QY;
}


void MausLoop() {

    
    // get data from mouse
    //
    int16_t data[3];
    mouse.report(data);

    

    // handle buttons
    //
    if( data[0] & 1) 
      LeftButtonDown();
    else
      LeftButtonUp();
      
    if( data[0] & 2) 
      RightButtonDown();
    else
      RightButtonUp();
      
    if( data[0] & 4) 
      MiddleButtonDown();
    else
      MiddleButtonUp();




    // calc x/y movement
    //
    XSTEPS = abs(data[1]);
    YSTEPS = abs(data[2]);
    XSIGN = (data[1] > 0 ? 1 : 0) ;
    YSIGN = (data[2] > 0 ? 1 : 0) ;


    
    // handle x/y movement 
    //
    while ((XSTEPS | YSTEPS) != 0) {

        // steps left?
        if (XSTEPS != 0) {
          
            // direction
            if (XSIGN)
                AMIGA_Right();
            else
                AMIGA_Left();

            // decrease steps    
            XSTEPS--;
        }

        
        if (YSTEPS != 0) {
            if (YSIGN)
                AMIGA_Up(); 
            else
               AMIGA_Down();
            YSTEPS--;
        }
        
    }    
}



#include "PS2Keyboard.h"
//==========





//#define SERIAL_DEBUG

#define NUMPAD  0x0100
#define NUMPAD2 0x0200

#define PS2_DATA_PIN 3
#define PS2_CLOCK_PIN 2

#define MAC_DATA_PIN 5
#define MAC_CLOCK_PIN 4

#define NULL_TRANSITION 0x7b
#define CAPS_LOCK       0x73

PS2Keyboard keyboard;
unsigned int scanCodesTable[256];
unsigned int extScanCodesTable[256];

void setup() {

mouse.initialize();
    pinMode(V_PULSE, OUTPUT);  // V-Pulse
    pinMode(H_PULSE, OUTPUT);  // H-Pulse
    pinMode(VQ_PLSE, OUTPUT);  // VQ-pulse
    pinMode(HQ_PLSE, OUTPUT);  // HQ-pulse
    pinMode(LMB, OUTPUT);  // LMB
    pinMode(RMB, OUTPUT);  // RMB
    pinMode(MMB, OUTPUT);  // MMB
    
        
    // Set quadrature output pins to zero
    digitalWrite(V_PULSE, LOW);
    digitalWrite(H_PULSE, LOW);
    digitalWrite(VQ_PLSE, LOW);
    digitalWrite(HQ_PLSE, LOW);
    
    
    // Set mouse button output pins to on, coz they are inverted
    digitalWrite(LMB, HIGH);
    digitalWrite(RMB, HIGH);
    digitalWrite(MMB, HIGH);
    
    delay(200);

#ifdef SERIAL_DEBUG
  Serial.begin(9600);
#endif
  initScancodes();

  keyboard.begin(PS2_DATA_PIN, PS2_CLOCK_PIN);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MAC_CLOCK_PIN, OUTPUT);
  pinMode(MAC_DATA_PIN, INPUT_PULLUP);
  
  waitForInitSignal();
  delayMicroseconds(180);
}

void waitForInitSignal() {
  unsigned long t = millis();
  boolean led = false;
  while (digitalRead(MAC_CLOCK_PIN) != LOW) {
    if (millis() - t > 100) {
      t = millis();
      led = !led;
      digitalWrite(LED_BUILTIN, led);
    }
  }
}

void loop() {
  switch (readCmd()) {
    case 0x10:
      inquiry();
      break;

    case 0x14: // instant
      sendKey(getKeyTransition());
      break;

    case 0x16: // model number
      sendByte(0x0b);
      break;

    case 0x36: // test
      sendByte(0x7d);
      break;
  }
  MausLoop();
}

// reads the command, operates the diagnostic LED and switches pin mode back to OUTPUT
byte readCmd() {
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(MAC_DATA_PIN, INPUT_PULLUP);
  delayMicroseconds(20);
  
  while (digitalRead(MAC_DATA_PIN) != LOW);
  delayMicroseconds(400); // this is apparently required so we don't lose the first digit
  byte cmd = readByte();
  while (digitalRead(MAC_DATA_PIN) != HIGH);
  
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(MAC_DATA_PIN, OUTPUT);
  delayMicroseconds(20);
  return cmd;
}

void inquiry() {
  unsigned long start = millis();
  while ((millis() - start) < 250) {
    int key = getKeyTransition();
    if (key != NULL_TRANSITION) {
      sendKey(key);
      return;
    }
  }
  sendByte(NULL_TRANSITION);
}

void sendKey(unsigned int key) {
  if (key & NUMPAD) {
    sendByte(0x79); readCmd();
    sendByte(key);
  } else if (key & NUMPAD2) {
    sendByte(0x71); readCmd();
    sendByte(0x79); readCmd();
    sendByte(key);
  } else {
    sendByte(key);
  }
}

byte readByte() {
  byte b = 0;
  for (byte i = 0; i < 8; i++) {
    digitalWrite(MAC_CLOCK_PIN, LOW);
    delayMicroseconds(180);
    digitalWrite(MAC_CLOCK_PIN, HIGH);
    delayMicroseconds(80);
    b = (b << 1) | digitalRead(MAC_DATA_PIN);
    delayMicroseconds(140);
  }
#ifdef SERIAL_DEBUG
  Serial.print(b, HEX);
  Serial.print(" -> ");
#endif
  return b;
}

void sendByte(byte b) {
#ifdef SERIAL_DEBUG
  Serial.print(b, HEX);
  Serial.println();
#endif
  for (byte m = 128; m > 0; m >>= 1) {
    digitalWrite(MAC_DATA_PIN, !(b & m) ? LOW : HIGH);
    delayMicroseconds(40);
    digitalWrite(MAC_CLOCK_PIN, LOW);
    delayMicroseconds(120);
    digitalWrite(MAC_CLOCK_PIN, HIGH);
    delayMicroseconds(170);
  }
  digitalWrite(MAC_DATA_PIN, HIGH);
}

unsigned int getKeyTransition() {
  byte c = keyboard.getScanCode();
  if (c == 0) {
    return NULL_TRANSITION;
  } else if (c == 0xf0) {
    return translate(waitForScanCode(), false, true);
  } else if (c == 0xe0) {
    return getExtendedTransition();
  } else {
    return translate(c, false, false);
  }
}

unsigned int getExtendedTransition() {
  byte c = waitForScanCode();
  if (c == 0xf0) {
    return translate(waitForScanCode(), true, true);
  } else {
    return translate(c, true, false);
  }
}

unsigned int translate(byte scanCode, boolean extended, boolean released) {
  unsigned int translated = extended ? extScanCodesTable[scanCode] : scanCodesTable[scanCode];
  if (translated == NULL_TRANSITION) {
    return NULL_TRANSITION;
  } else if (released) {
    if (translated == CAPS_LOCK) {
      return handleCapsLockRelease();
    } else {
      return translated | 0x80;
    }
  } else {
    return translated;
  }
}

boolean capsLockPressed = false;

unsigned int handleCapsLockRelease() {
  if (capsLockPressed) {
    capsLockPressed = false;
    return CAPS_LOCK | 0x80;
  } else {
    capsLockPressed = true;
    return NULL_TRANSITION;
  }
}

byte waitForScanCode() {
  while (true) {
    byte s = keyboard.getScanCode();
    if (s) {
      return s;
    }
  }
}

