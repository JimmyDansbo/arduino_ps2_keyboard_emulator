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

void CtrlAltDelete() {
  keyboard.keyboard_press(PS2dev::LEFT_CONTROL);
  delay(2);
  keyboard.keyboard_press(PS2dev::LEFT_ALT);
  delay(2);
  keyboard.keyboard_press_special(PS2dev::DELETE);
  delay(1);
  keyboard.keyboard_release_special(PS2dev::DELETE);
  delay(1);
  keyboard.keyboard_release(PS2dev::LEFT_ALT);
  delay(1);
  keyboard.keyboard_release(PS2dev::LEFT_CONTROL);
}

void handleChars() {
  Serial.println(incomingByte);
  switch (incomingByte) {
    case 0x03: writeControlled(PS2dev::C);        break; // CTRL+C
    case 0x18: writeControlled(PS2dev::X);        break; // CTRL+X
    case 0x1B: writeNormal(PS2dev::ESCAPE);       break;
    case 0x08: writeNormal(PS2dev::BACKSPACE);    break;
    case 0x0D: writeNormal(PS2dev::ENTER);        break;
    case 'a':  writeNormal(PS2dev::A);            break;
    case 'b':  writeNormal(PS2dev::B);            break;
    case 'c':  writeNormal(PS2dev::C);            break;
    case 'd':  writeNormal(PS2dev::D);            break;
    case 'e':  writeNormal(PS2dev::E);            break;
    case 'f':  writeNormal(PS2dev::F);            break;
    case 'g':  writeNormal(PS2dev::G);            break;
    case 'h':  writeNormal(PS2dev::H);            break;
    case 'i':  writeNormal(PS2dev::I);            break;
    case 'j':  writeNormal(PS2dev::J);            break;
    case 'k':  writeNormal(PS2dev::K);            break;
    case 'l':  writeNormal(PS2dev::L);            break;
    case 'm':  writeNormal(PS2dev::M);            break;
    case 'n':  writeNormal(PS2dev::N);            break;
    case 'o':  writeNormal(PS2dev::O);            break;
    case 'p':  writeNormal(PS2dev::P);            break;
    case 'q':  writeNormal(PS2dev::Q);            break;
    case 'r':  writeNormal(PS2dev::R);            break;
    case 's':  writeNormal(PS2dev::S);            break;
    case 't':  writeNormal(PS2dev::T);            break;
    case 'u':  writeNormal(PS2dev::U);            break;
    case 'v':  writeNormal(PS2dev::V);            break;
    case 'w':  writeNormal(PS2dev::W);            break;
    case 'x':  writeNormal(PS2dev::X);            break;
    case 'y':  writeNormal(PS2dev::Y);            break;
    case 'z':  writeNormal(PS2dev::Z);            break;
    case 'A':  writeShifted(PS2dev::A);           break;
    case 'B':  writeShifted(PS2dev::B);           break;
    case 'C':  writeShifted(PS2dev::C);           break;
    case 'D':  writeShifted(PS2dev::D);           break;
    case 'E':  writeShifted(PS2dev::E);           break;
    case 'F':  writeShifted(PS2dev::F);           break;
    case 'G':  writeShifted(PS2dev::G);           break;
    case 'H':  writeShifted(PS2dev::H);           break;
    case 'I':  writeShifted(PS2dev::I);           break;
    case 'J':  writeShifted(PS2dev::J);           break;
    case 'K':  writeShifted(PS2dev::K);           break;
    case 'L':  writeShifted(PS2dev::L);           break;
    case 'M':  writeShifted(PS2dev::M);           break;
    case 'N':  writeShifted(PS2dev::N);           break;
    case 'O':  writeShifted(PS2dev::O);           break;
    case 'P':  writeShifted(PS2dev::P);           break;
    case 'Q':  writeShifted(PS2dev::Q);           break;
    case 'R':  writeShifted(PS2dev::R);           break;
    case 'S':  writeShifted(PS2dev::S);           break;
    case 'T':  writeShifted(PS2dev::T);           break;
    case 'U':  writeShifted(PS2dev::U);           break;
    case 'V':  writeShifted(PS2dev::B);           break;
    case 'W':  writeShifted(PS2dev::W);           break;
    case 'X':  writeShifted(PS2dev::X);           break;
    case 'Y':  writeShifted(PS2dev::Y);           break;
    case 'Z':  writeShifted(PS2dev::Z);           break;
    case '0':  writeNormal(PS2dev::ZERO);         break;
    case '1':  writeNormal(PS2dev::ONE);          break;
    case '2':  writeNormal(PS2dev::TWO);          break;
    case '3':  writeNormal(PS2dev::THREE);        break;
    case '4':  writeNormal(PS2dev::FOUR);         break;
    case '5':  writeNormal(PS2dev::FIVE);         break;
    case '6':  writeNormal(PS2dev::SIX);          break;
    case '7':  writeNormal(PS2dev::SEVEN);        break;
    case '8':  writeNormal(PS2dev::EIGHT);        break;
    case '9':  writeNormal(PS2dev::NINE);         break;
    case '-':  writeNormal(PS2dev::MINUS);        break;
    case '=':  writeNormal(PS2dev::EQUAL);        break;
    case 0x09: writeNormal(PS2dev::TAB);          break;
    case '\\': writeNormal(PS2dev::BACKSLASH);    break;
    case ';':  writeNormal(PS2dev::SEMI_COLON);   break;
    case ',':  writeNormal(PS2dev::COMMA);        break;
    case '.':  writeNormal(PS2dev::PERIOD);       break;
    case '/':  writeNormal(PS2dev::SLASH);        break;
    case ' ':  writeNormal(PS2dev::SPACE);        break;
    case '*':  writeNormal(PS2dev::ASTERISK);     break;
    case '+':  writeNormal(PS2dev::PLUS);         break;
    case '\'': writeNormal(PS2dev::ACCENT);    break;
    case '"':  writeShifted(PS2dev::TICK_MARK);   break;
    case '@':  writeShifted(PS2dev::TWO);         break;
    case '$':  writeShifted(PS2dev::FOUR);        break;
    case ':':  writeShifted(PS2dev::SEMI_COLON);  break;
    case '!':  writeShifted(PS2dev::ONE);         break;
    case '#':  writeShifted(PS2dev::THREE);       break;
    case '%':  writeShifted(PS2dev::FIVE);        break;
    case 18:   CtrlAltDelete();                   break; //CTRL+R
    case '[':  writeNormal(PS2dev::OPEN_BRACKET); break;
    case ']':  writeNormal(PS2dev::CLOSE_BRACKET);break;
    case '(':  writeShifted(PS2dev::NINE);        break;
    case ')':  writeShifted(PS2dev::ZERO);        break;
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
