// primary color test: testing the functionality of two Maxim chips at the same time

// FOR EFFICIENCY: there are two separate functions use to write to either first board or second board,
// to make the code efficient, check whether both boards are being written to and write a function that
// successfully writes to boath.

// global data

int dataIn = 4;
int load = 2;
int clock = 3;
bool checkWithLED = true;
int LED = 13;
bool LEDStatus = true;

// how many max7219 units in use
int maxInUse = 2;

int e = 0;
int f = 1;
int g = 0;
double a = 0.0;

// registers for max7219
byte max7219_reg_noop        = 0x00;
byte max7219_reg_digit0      = 0x01;
byte max7219_reg_digit1      = 0x02;
byte max7219_reg_digit2      = 0x03;
byte max7219_reg_digit3      = 0x04;
byte max7219_reg_digit4      = 0x05;
byte max7219_reg_digit5      = 0x06;
byte max7219_reg_digit6      = 0x07;
byte max7219_reg_digit7      = 0x08;
byte max7219_reg_decodeMode  = 0x09;
byte max7219_reg_intensity   = 0x0a;
byte max7219_reg_scanLimit   = 0x0b;
byte max7219_reg_shutdown    = 0x0c;
byte max7219_reg_displayTest = 0x0f;

// toggle a pin
bool toggle(bool pinStatus, int Pin) {
  
  if (pinStatus) {
    digitalWrite(Pin, LOW);
    return false;
  } else {
    digitalWrite(Pin, HIGH);
    return true;
  }
}

// send a byte of data
void putByte(byte data) {
  byte i = 8;
  byte mask;
  while (i > 0) {
    mask = 0x01 << (i-1);         // define bit mask
    digitalWrite(clock, LOW);     // tick clock
    if (data & mask) {            // choose bit
      digitalWrite(dataIn, HIGH); // send 1
    } else {
      digitalWrite(dataIn, LOW);  // send 0
    }
    digitalWrite(clock, HIGH);     // tock clock
    --i;                         // to next bit
  }
}

// using a single max7219
void maxSingle(byte reg, byte col) {
  digitalWrite(load, LOW);        // begin
  putByte(reg);                   // specify register
  putByte(col);                   // ((data & 0x01) * 256) + data >> 1); // put data
  digitalWrite(load, LOW);        // load data
  digitalWrite(load, HIGH);
}

// using multiple max7219, relies on global variable maxInUse
void maxX(byte maxX, byte reg, byte col) {

  int c = 0;
  digitalWrite(load, LOW);        // begin

  for (c = maxInUse ; c > maxX ; c--) {
    putByte(0); //noOp
    putByte(0); //noOp
  }

  putByte(reg);   // specify reg
  putByte(col);   // put data

  for (c = maxX-1 ; c >= 1 ; c--) {
    putByte(0);   // noOP
    putByte(0);   // noOp
  }
  
  digitalWrite(load, LOW);        // load data
  digitalWrite(load, HIGH);
}

void clearMax() {
  for( e=1 ; e<=8 ; e++) {
    maxSingle(e, 0);
  }
}

void setup() {
  pinMode(dataIn, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(load, OUTPUT);
  pinMode(LED, OUTPUT);

  if (checkWithLED) {
    digitalWrite(LED, HIGH);
  }

  // initialize max7219 //here I am only doing it for max2 (maxInUse = 2), need to do each of these
  // for each maxInUse to ensure proper setup. (maybe write init and initAll functions)
  maxX(maxInUse, max7219_reg_scanLimit, 0x07);
  maxX(maxInUse, max7219_reg_decodeMode, 0x00);  // using an LED matrix, not digits
  maxX(maxInUse, max7219_reg_shutdown, 0x01);    // 0x00 shutdown, 0x01 normal mode
  maxSingle(max7219_reg_shutdown, 0x01); // shutting down
  maxX(maxInUse, max7219_reg_displayTest, 0x00); // 0x00 no display test // set to 0x01 if you want it.
  
  for( e=1 ; e<=8 ; e++) {
    maxX(maxInUse, e, 0);
  }
  maxX(maxInUse, max7219_reg_intensity, 0x0f & 0x0f);  // 0x0f can be between 0x00 and 0x0f, set second to desired value
  
    //Serial.begin(9600);
}

void loop() {
   
  LEDStatus = toggle(LEDStatus, LED);
  a = pow(2,g);

  //Serial.println(f);

  clearMax();
  maxSingle(f, (int) (a+0.5));
  g++;
  if (g >= 8) {
    g = 0;
    f++;
  }
  if (f >= 3) {
    f = 1;
  }
  /*
  if ( g%2 == 0) {
  maxX(2,1,1);                       //  + - - - - - - -
  maxX(2,2,2);                       //  - + - - - - - -
  maxX(2,3,4);                       //  - - + - - - - -
  maxX(2,4,8);                       //  - - - + - - - -
  maxX(2,5,16);                      //  - - - - + - - -
  maxX(2,6,32);                      //  - - - - - + - -
  maxX(2,7,64);                      //  - - - - - - + -
  maxX(2,8,128);                     //  - - - - - - - +
  } else {
  maxX(2,1,128);                       //  + - - - - - - -
  maxX(2,2,64);                       //  - + - - - - - -
  maxX(2,3,32);                       //  - - + - - - - -
  maxX(2,4,16);                       //  - - - + - - - -
  maxX(2,5,8);                      //  - - - - + - - -
  maxX(2,6,4);                      //  - - - - - + - -
  maxX(2,7,2);                      //  - - - - - - + -
  maxX(2,8,1);                     //  - - - - - - - +
  }
  
  /* clearMax();
  maxSingle(1, a);
  if (f >= 8) {
    f = 0;
    g++;
  }
  if (g >= 8) {
    g = 0;
  }
  f++;  */
  
  delay(500);
  
  /*
  clearMax();
  maxSingle(f, 255);

  if (f >= 8) {
    f = 0;
  }
  f++; 
  
  delay(500); */
  
}
