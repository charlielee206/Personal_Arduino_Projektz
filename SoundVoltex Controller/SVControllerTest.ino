#include <Keyboard.h>
#include <KeyboardLayout.h>

int KeyDelayTime = 50;

byte R_ENC_A = 2;
byte R_ENC_B = 3;
int R_count = 0;
bool R_A = HIGH;
bool RLast = HIGH;
bool R_B = HIGH;
int R_PrevCount = 0;

byte L_ENC_A = 4;
byte L_ENC_B = 5;
int L_count = 0;
bool L_A = HIGH;
bool LLast = HIGH;
bool L_B = HIGH;
int L_PrevCount = 0;

unsigned long BackgroundTimer = 0;
int R_Checkpoint = 0;
int L_Checkpoint = 0;

byte Button1 = 9;
byte Button2 = 8;
byte Button3 = 7;
byte Button4 = 6;
byte Button5 = 11;
byte Button6 = 10;

byte Button7 = 13;
byte Button8 = 12;



void setup() {
  pinMode (R_ENC_A,INPUT_PULLUP);
  pinMode (R_ENC_B,INPUT_PULLUP);
  pinMode (L_ENC_A,INPUT_PULLUP);
  pinMode (L_ENC_B,INPUT_PULLUP);

  pinMode (Button1,INPUT_PULLUP);
  pinMode (Button2,INPUT_PULLUP);
  pinMode (Button3,INPUT_PULLUP);
  pinMode (Button4,INPUT_PULLUP);
  pinMode (Button5,INPUT_PULLUP);
  pinMode (Button6,INPUT_PULLUP);
  pinMode (Button7,INPUT_PULLUP);
  pinMode (Button8,INPUT_PULLUP);



 Serial.begin (9600);
 Keyboard.begin();
}

void loop() {
  CheckEncoder();

  if(!digitalRead(Button1)){
    Serial.println("Button1");
    Keyboard.press(100); //d
    delay(KeyDelayTime);
  }

  if(!digitalRead(Button2)){
    Serial.println("Button2");
    Keyboard.press(102); //f
    delay(KeyDelayTime);
  }

  if(!digitalRead(Button3)){
    Serial.println("Button3");
    Keyboard.press(106); //j
    delay(KeyDelayTime);
  }

  if(!digitalRead(Button4)){
    Serial.println("Button4");
    Keyboard.press(107); //k
    delay(KeyDelayTime);
  }

  if(!digitalRead(Button5)){
    Serial.println("Button5");
    Keyboard.press(99); //c
    delay(KeyDelayTime);
  }

  if(!digitalRead(Button6)){
    Serial.println("Button6");
    Keyboard.press(109); //m
    delay(KeyDelayTime);
  }

  if(digitalRead(Button7)){
    Serial.println("Button7"); 
    Keyboard.releaseAll();
    Keyboard.press(177); //ESC
    delay(2000);
    Keyboard.releaseAll();
    delay(1000);
  }

  if(digitalRead(Button8)){
    Serial.println("Button8");
    Keyboard.press(49); //1
  }
  delay(1);
  Keyboard.releaseAll();


  //delay(100);
/* //Turns out this part wasn't needed lol
//==========] ROTARY ENCODER RATE OF CHANGE THINGY [==========
  if(millis() - BackgroundTimer >= 50){
    if(L_count - L_Checkpoint > 5){
      Serial.println("Beeg Left Change");
    }
    if(L_count - L_Checkpoint < -5){
      Serial.println("Beeg Left Change");
    }
    L_Checkpoint = L_count;

    if(R_count - R_Checkpoint > 5){
      Serial.println("Beeg Right Change");
    }
    if(R_count - R_Checkpoint < -5){
      Serial.println("Beeg Right Change");
    }
    R_Checkpoint = R_count;

    BackgroundTimer = millis();
  }
*/

}



void CheckEncoder(){
  R_A = digitalRead(R_ENC_A); 
  R_B = digitalRead(R_ENC_B);  

  if (R_A != RLast){       
    R_B = R_A^R_B;              // work out direction using an XOR
    if (R_B){
      R_count++;
      Serial.print ("R_Right: ");
      Serial.println(R_count);
      Keyboard.press(112); //p
      delay(1);
    }else{
      R_count--;
      Serial.print ("R_Left: ");
      Serial.println(R_count);
      Keyboard.press(111); //o
      delay(1);
    }
    RLast = R_A;
  }

  L_A = digitalRead(L_ENC_A); 
  L_B = digitalRead(L_ENC_B);  

  if (L_A != LLast){     
    L_B = L_A^L_B;              // work out direction using an XOR
    if(L_B){
      L_count++;
      Serial.print ("L_Right: ");
      Serial.println(L_count);
      Keyboard.press(101); //e
      delay(1);
    }else{
      L_count--;
      Serial.print ("L_Left: ");
      Serial.println(L_count);
      Keyboard.press(119); //w
      delay(1);
    }
    LLast = L_A;
  }
}
