#include <ps2dev.h>    //Emulate a PS/2 device

#define PS2clk 3
#define PS2data 2

PS2dev keyboard(PS2clk, PS2data);  //clock, data

int enabled = 0; // pseudo variable for state of "keyboard"
boolean serialConnected = false;
int incomingByte = 0;

void ack() {
  //acknowledge commands
  while(keyboard.write(0xFA));
}

int kbdCmd(int command) {
  unsigned char val;
  switch (command) {
  case 0xFF: //reset
    ack();
    //the while loop lets us wait for the host to be ready
    while(keyboard.write(0xAA)!=0);
    break;
  case 0xFE: //resend
    ack();
    break;
  case 0xF6: //set defaults
    //enter stream mode
    ack();
    break;
  case 0xF5: //disable data reporting
    //FM
    enabled = 0;
    ack();
    break;
  case 0xF4: //enable data reporting
    //FM
    enabled = 1;
    ack();
    break;
  case 0xF3: //set typematic rate
    ack();
    keyboard.read(&val); //do nothing with the rate
    ack();
    break;
  case 0xF2: //get device id
    ack();
    keyboard.write(0xAB);
    keyboard.write(0x83);
    break;
  case 0xF0: //set scan code set
    ack();
    keyboard.read(&val); //do nothing with the rate
    ack();
    break;
  case 0xEE: //echo
    //ack();
    keyboard.write(0xEE);
    break;
  case 0xED: //set/reset LEDs
    ack();
    keyboard.read(&val); //do nothing with the rate
    ack();
    break;
  }
}

void connectHost() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

void setup() {
  pinMode(13, OUTPUT);
  //establish serial connection with host
  Serial.begin(9600);
  // establish ps/2 connection with target
  while(keyboard.write(0xAA)!=0){
    digitalWrite(13, HIGH);
    delay(500); 
    digitalWrite(13, LOW);
    delay(500);
  }
  delay(100);  
  
  connectHost();
  Serial.println("\nSerial Host Connected");
  Serial.read();
  Serial.flush();
}

void handleEsc() {
  unsigned long timecnt;

  // Wait for 5 milliseconds to see if a new character has arrived on the serial port
  // If it has, we have entered an escape sequence.
  timecnt=millis();
  while (millis()<timecnt+5);
  
  if (Serial.available()>0) { //Receiving an escabe sequence
    incomingByte=Serial.read();
    if (incomingByte==0x5B) { // Arrow keys pressed
      while (Serial.available()==0); // Wait for last byte of arrow escabe sequence
      incomingByte=Serial.read();
      switch (incomingByte) {
        case 0x41:  keyboard.keyboard_press_special(PS2dev::UP_ARROW);
                    delay(2);
                    keyboard.keyboard_release_special(PS2dev::UP_ARROW);
                    break;
        case 0x42:  keyboard.keyboard_press_special(PS2dev::DOWN_ARROW);
                    delay(2);
                    keyboard.keyboard_release_special(PS2dev::DOWN_ARROW);
                    break;
        case 0x43:  keyboard.keyboard_press_special(PS2dev::RIGHT_ARROW);
                    delay(2);
                    keyboard.keyboard_release_special(PS2dev::RIGHT_ARROW);
                    break;
        case 0x44:  keyboard.keyboard_press_special(PS2dev::LEFT_ARROW);
                    delay(2);
                    keyboard.keyboard_release_special(PS2dev::LEFT_ARROW);
                    break;
      }
    }
  } else {
    keyboard.write(PS2dev::ESCAPE);
    Serial.println("ESC");
  }
}

void writeNormal(unsigned char ch) {
  keyboard.write(ch);
}

void writeShifted(unsigned char ch) {
  keyboard.keyboard_press(PS2dev::RIGHT_SHIFT);
  delay(2);
  keyboard.keyboard_press(ch);
  delay(2);
  keyboard.keyboard_release(ch);
  delay(2);
  keyboard.keyboard_release(PS2dev::RIGHT_SHIFT);
}

void writeControlled(unsigned char ch) {
  keyboard.keyboard_press(PS2dev::LEFT_CONTROL);
  delay(2);
  keyboard.keyboard_press(ch);
  delay(2);
  keyboard.keyboard_release(ch);
  delay(2);
  keyboard.keyboard_release(PS2dev::LEFT_CONTROL);
}

void handleChars() {
  Serial.println(incomingByte);
  switch (incomingByte) {
    case 0x03: writeControlled(PS2dev::C); break;
    case 0x18: writeControlled(PS2dev::X); break;
    case 0x1B: keyboard.write(PS2dev::ESCAPE); break;
    case 0x08: keyboard.write(PS2dev::BACKSPACE); break;
    case 0x0D: keyboard.write(PS2dev::ENTER); break;
    case 'a': keyboard.write(PS2dev::A); break;
    case 'b': keyboard.write(PS2dev::B); break;
    case 'c': keyboard.write(PS2dev::C); break;
    case 'd': keyboard.write(PS2dev::D); break;
    case 'e': keyboard.write(PS2dev::E); break;
    case 'f': keyboard.write(PS2dev::F); break;
    case 'g': keyboard.write(PS2dev::G); break;
    case 'h': keyboard.write(PS2dev::H); break;
    case 'i': keyboard.write(PS2dev::I); break;
    case 'j': keyboard.write(PS2dev::J); break;
    case 'k': keyboard.write(PS2dev::K); break;
    case 'l': keyboard.write(PS2dev::L); break;
    case 'm': keyboard.write(PS2dev::M); break;
    case 'n': keyboard.write(PS2dev::N); break;
    case 'o': keyboard.write(PS2dev::O); break;
    case 'p': keyboard.write(PS2dev::P); break;
    case 'q': keyboard.write(PS2dev::Q); break;
    case 'r': keyboard.write(PS2dev::R); break;
    case 's': keyboard.write(PS2dev::S); break;
    case 't': keyboard.write(PS2dev::T); break;
    case 'u': keyboard.write(PS2dev::U); break;
    case 'v': keyboard.write(PS2dev::V); break;
    case 'w': keyboard.write(PS2dev::W); break;
    case 'x': keyboard.write(PS2dev::X); break;
    case 'y': keyboard.write(PS2dev::Y); break;
    case 'z': keyboard.write(PS2dev::Z); break;
    case '0': keyboard.write(PS2dev::ZERO); break;
    case '1': keyboard.write(PS2dev::ONE); break;
    case '2': keyboard.write(PS2dev::TWO); break;
    case '3': keyboard.write(PS2dev::THREE); break;
    case '4': keyboard.write(PS2dev::FOUR); break;
    case '5': keyboard.write(PS2dev::FIVE); break;
    case '6': keyboard.write(PS2dev::SIX); break;
    case '7': keyboard.write(PS2dev::SEVEN); break;
    case '8': keyboard.write(PS2dev::EIGHT); break;
    case '9': keyboard.write(PS2dev::NINE); break;
    case '-': keyboard.write(PS2dev::MINUS); break;
    case '=': keyboard.write(PS2dev::EQUAL); break;
    case 0x09: keyboard.write(PS2dev::TAB); break;
    case '\\': keyboard.write(PS2dev::BACKSLASH); break;
    case ';': keyboard.write(PS2dev::SEMI_COLON); break;
    case ',': keyboard.write(PS2dev::COMMA); break;
    case '.': keyboard.write(PS2dev::PERIOD); break;
    case '/': keyboard.write(PS2dev::SLASH); break;
    case ' ': keyboard.write(PS2dev::SPACE); break;
    case '*': keyboard.write(PS2dev::ASTERISK); break;
    case '+': keyboard.write(PS2dev::PLUS); break;
    case '\'': keyboard.write(PS2dev::TICK_MARK); break;
    case '"': writeShifted(PS2dev::TICK_MARK); break;
    case '@': writeShifted(PS2dev::TWO); break;
    case '$': writeShifted(PS2dev::FOUR); break;
    case ':': writeShifted(PS2dev::SEMI_COLON); break;
  }
}

void loop() {
  unsigned char c;
  if(keyboard.available()) {
    if(digitalRead(PS2data)==LOW){
      Serial.println("PS2data is LOW");
    } else {
      Serial.println("PS2clk is LOW");
    }
    while(keyboard.read(&c));
    kbdCmd(c);
    Serial.print("Target: 0x");
    Serial.println(c, HEX);
  }  
  else {//if host device wants to send a command:
    //echo ASCII code from terminal and write to ps/2
    if(Serial.available() > 0) {
      incomingByte = Serial.read();
      if (incomingByte==0x1B) {
        handleEsc();
      } else {
        handleChars();
      }
//      Serial.print("ASCII: 0x");
//      Serial.print(incomingByte, HEX);
//      incomingByte = asciiToScanCode(incomingByte);
      
//      keyboard.write(incomingByte);      
//      keyboard.keyboard_press(incomingByte);
//      delay(5);
//      keyboard.keyboard_release(incomingByte);
//      Serial.print(" Scan: 0x");
//      Serial.println(incomingByte, HEX);
    }
  }
}
