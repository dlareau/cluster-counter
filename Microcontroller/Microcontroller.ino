/* code for max 7219 from maxim, 
reduced and optimised for using more than one 7219 in a row,
______________________________________

 Code History:
 --------------

The original code was written for the Wiring board by:
 * Nicholas Zambetti and Dave Mellis /Interaction Design Institute Ivrea /Dec 2004
 * http://www.potemkin.org/uploads/Wiring/MAX7219.txt

First modification by:
 * Marcus Hannerstig/  K3, malmö högskola /2006
 * http://www.xlab.se | http://arduino.berlios.de

This version is by:
 * tomek ness /FH-Potsdam / Feb 2007
 * http://design.fh-potsdam.de/ 

 * @acknowledgements: eric f. 

-----------------------------------

General notes: 

-if you are only using one max7219, then use the function maxSingle to control
 the little guy ---maxSingle(register (1-8), collum (0-255))

-if you are using more than one max7219, and they all should work the same, 
then use the function maxAll ---maxAll(register (1-8), collum (0-255))

-if you are using more than one max7219 and just want to change something
at one little guy, then use the function maxOne
---maxOne(Max you want to control (1== the first one), register (1-8), 
column (0-255))

During initiation, be sure to send every part to every max7219 and then
upload it.
For example, if you have five max7219's, you have to send the scanLimit 5 times
before you load it-- otherwise not every max7219 will get the data. the
function maxInUse keeps track of this, just tell it how many max7219 you are
using.
*/

#include <stdio.h>


int dataIn = 4;
int load = 5;
int clock1 = 6;

int maxInUse = 1;    //change this variable to set how many MAX7219's you'll use

int e = 0;           // just a variable

// define max7219 registers
byte max7219_reg_noop        = 0x00;
byte max7219_reg_decodeMode  = 0x09;
byte max7219_reg_intensity   = 0x0a;
byte max7219_reg_scanLimit   = 0x0b;
byte max7219_reg_shutdown    = 0x0c;
byte max7219_reg_displayTest = 0x0f;

byte disp_map[9] = {0,4,3,5,1,2,7,8,6};

// DP, LBottom, Middle, RTop, Bottom, RBottom, Top, LTop. 
int constants[10] = {
  0b01011111,
  0b00010100,
  0b01111010,
  0b00111110,
  0b00110101,
  0b00101111,
  0b01101111,
  0b00010110,
  0b01111111,
  0b00111111
};

void putByte(byte data) {
  byte i = 8;
  byte mask;
  while(i > 0) {
    mask = 0x01 << (i - 1);      // get bitmask
    digitalWrite( clock1, LOW);   // tick
    if (data & mask){            // choose bit
      digitalWrite(dataIn, HIGH);// send 1
    }else{
      digitalWrite(dataIn, LOW); // send 0
    }
    digitalWrite(clock1, HIGH);   // tock
    --i;                         // move to lesser bit
  }
}

void maxSingle( byte reg, byte col) {    
//maxSingle is the "easy"  function to use for a single max7219
  reg = disp_map[reg];
  digitalWrite(load, LOW);       // begin     
  putByte(reg);                  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data   
  digitalWrite(load, LOW);       // and load da stuff
  digitalWrite(load,HIGH); 
}

void maxAll (byte reg, byte col) {    // initialize  all  MAX7219's in the system
  int c = 0;
  digitalWrite(load, LOW);  // begin     
  for ( c =1; c<= maxInUse; c++) {
  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
    }
  digitalWrite(load, LOW);
  digitalWrite(load,HIGH);
}

void setup () {

  pinMode(dataIn, OUTPUT);
  pinMode(clock1,  OUTPUT);
  pinMode(load,   OUTPUT);

  digitalWrite(13, HIGH);  

//initiation of the max 7219d
  maxAll(max7219_reg_scanLimit, 0x07);      
  maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxAll(max7219_reg_displayTest, 0x00); // no display test
   for (e=1; e<=8; e++) {    // empty registers, turn all LEDs off 
    maxAll(e,0);
  }
  maxAll(max7219_reg_intensity, 0x0f & 0x0f);    // the first 0x0f is the value you can set
                                                  // range: 0x00 to 0x0f
}

byte char_val(char in_char){
  switch(in_char) {
    case 'A':
      return 0x77;
    case 'B':
      return 0x6D;
    case 'C':
      return 0x4B;
    case 'D':
      return 0x7C;
    case 'E':
      return 0x6B;
    case 'F':
      return 0x63;
    case 'G':
      return 0x3F;
    case 'H':
      return 0x75;
    case 'I':
      return 0x04;
    case 'J':
      return 0x5C;
    case 'L':
      return 0x49;
    case 'N':
      return 0x64;
    case 'O':
      return 0x5F;
    case 'P':
      return 0x73;
    case 'R':
      return 0x60;
    case 'S':
      return 0x2F;
    case 'T':
      return 0x69;
    case 'U':
      return 0x5D;
    case 'Y':
      return 0x35;
    default:
      return 0b00000000;
  }
}

void disp_word(char in_word[]){
  for(int i = 0; i < 8; i++){
    maxSingle(8-i, char_val(in_word[i]));
  }
}

void disp_num(int num, int start_seg){
  if(start_seg == 9){
    return;
  }
  if(start_seg == 1 && num == 0){
    maxSingle(start_seg, constants[0]);
  }
  else if(num == 0){
    maxSingle(start_seg, 0b00000000);
  } else{
    maxSingle(start_seg, constants[num%10]);
  }
  disp_num(num/10, start_seg+1);
}

void loop () {
  //FILE* file = fopen ("/sketch/temp.txt", "r");
  FILE *file = popen("curl http://jlareau.club.cc.cmu.edu:8000/query", "r");
  int i = 0;
  fscanf (file, "%d", &i);
  fclose (file);
  
  if(i % 1000 == 989){
    disp_word(" CLUSTER");
  } else if (i % 1000 == 990){
    disp_word(" COUNTER");
  } else {
    disp_num(i, 1);
  }
  delay(200);
}
