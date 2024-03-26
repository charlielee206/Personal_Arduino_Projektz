This is basically a Copy-Paste of two projects:</br>
"Arduino PS2 Mouse to Amiga Adapter" by glitterkitty</br>
https://github.com/glitterkitty/Arduino-PS2-Mouse-to-Amiga-Adapter</br>

and "mac plus ps2" by trekawek.</br>
https://github.com/trekawek/mac-plus-ps2</br>

I only changed the pinouts and merged the two sketches.</br>
All credit for the sketch goes to those two original creators.</br>

<pre>
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
</pre>
<pre>
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

</pre>
