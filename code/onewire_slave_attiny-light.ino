#include <OneWireSlave.h>

// unsigned char rom[8] = {0x01, 0xAD, 0xDA, 0xCE, 0x0F, 0x00, 0x00, 0x00};
unsigned char rom[8] = {0xAA, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00};
char buf[] = {0x10,0x20,0x30,0x40,0x50,0x60};
byte bufcrc = 0;    
char data[30];
#define PWM_PIN 0
int cur_state = 0;
OneWireSlave ds(4);
#define SMALL_VAL 5
#define MEDIUM_VAl 25
 
void setup() {
    pinMode(PWM_PIN,OUTPUT);
    analogWrite(PWM_PIN,0);
    ds.setRom(rom);
    bufcrc = OneWireSlave::crc8(buf,sizeof(buf)) % 256;
  }




void changePwmState(int value) {
  cur_state = value;
  if (cur_state > 255) { cur_state = 255; }
  if (cur_state < 0) { cur_state = 0; }  
  byte val = cur_state;
  analogWrite(PWM_PIN,cur_state);
  }

void moveForPosition(byte value, unsigned int delay_x_10ms = 0) {
  if (value > cur_state) {
    while(value > cur_state) {
      cur_state = cur_state + 5;
      if (cur_state > value) { cur_state = value; }
      byte val = cur_state;
      analogWrite(PWM_PIN,val);
      delay(delay_x_10ms*10+1);
    }  
  } else if (value < cur_state) {
  
    while(value < cur_state) {
      cur_state = cur_state - 5;
      if (cur_state < value) { cur_state = value; }
      byte val = cur_state;
      analogWrite(PWM_PIN,val);
      delay(delay_x_10ms*10+1);
      }
    }
    } 

void loop() {
    /*
    moveForPosition(240,2);
    delay(1000);
    moveForPosition(0,5);
    delay(1000);
    moveForPosition(240,5);
    delay(1000);
    moveForPosition(0,10);
    delay(1000);

    
    return;
    */
    ds.waitForRequest(false);
    byte c=ds.recvData(data,30);
    if (c > 0) {
      if ((byte)data[c-1] == (byte)OneWireSlave::crc8(data,c-1)) {
        // когда валидный код к нам приходит
        switch((byte)data[0]) {
          case 0x01: // запрос данных с устройства (не испоользуется) 
          for(byte i = 0; i < sizeof(buf); i++ ) {
            ds.send(buf[i]);
            }
          ds.send(bufcrc);
          break;
          case 0x02: // установка конкретного значения
            cur_state = (byte)data[1];
            analogWrite(PWM_PIN,cur_state);
          break;
          case 0x03: // + малый шаг к тек.значению
          changePwmState(SMALL_VAL);
          break;
          case 0x04: // - малый шаг от тек.значения
          changePwmState(-SMALL_VAL);
          break;
          case 0x05: // + средний шаг к тек.значению
          changePwmState(SMALL_VAL);
          break;
          case 0x06: // - средний шаг от тек.значения
          changePwmState(-SMALL_VAL);
          break;
          case 0x07: // перейти к позиции бфстро
          moveForPosition((byte)data[1],2);
          break;    
          case 0x08: // перецти к позиции, плавно
          moveForPosition((byte)data[1],5);
          break;
          case 0x09: // перецти к такой позиции, медленно
          moveForPosition((byte)data[1],10);
          break; 
          }
        }
      }
  }
