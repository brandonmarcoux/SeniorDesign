// Secondary Color Test: single max7219 display driver triggering 1 row at a time
// The cathode of 8 LEDs will be connected to a single ROW, all anodes to individual columns
// This will be done twice, allowing utilization of 16 controls, each of which
// will cascade through the entire row.
// global data

int dataIn = 4;
int load = 2;
int clock = 3;
bool checkWithLED = true;
int LED = 13;
bool LEDStatus = true;

// how many max7219 units in use
int maxInUse = 1;

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

  // initialize max7219
  maxSingle(max7219_reg_scanLimit, 0x07);
  maxSingle(max7219_reg_decodeMode, 0x00);  // using an LED matrix, not digits
  maxSingle(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxSingle(max7219_reg_displayTest, 0x00); // no display test
  for( e=1 ; e<=8 ; e++) {
    maxSingle(e, 0);
  }
  maxSingle(max7219_reg_intensity, 0x0f & 0x07);  // 0x0f can be between 0x00 and 0x0f, set second to desired value
  
    Serial.begin(9600);
}

void loop() {
   
  LEDStatus = toggle(LEDStatus, LED);
  a = pow(2,g);

  Serial.println(f);

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
