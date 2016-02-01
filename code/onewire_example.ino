#include <OneWire.h>
OneWire  ds(8);
unsigned int k_i = 0;
unsigned int k_j = 0;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
}

void sendCommand(byte* buf, byte count  ) {
  //byte cmd[count];
  buf[count] = OneWire::crc8(buf, count) % 256;
  for(byte i = 0; i <= count; i++) {
    ds.write(buf[i], 1);
    }

  for(byte i = 0; i <= count; i++) {
    Serial.print(' ');
    Serial.print(buf[i], HEX);
    }
  Serial.println();
  }


void loop() {
  // put your main code here, to run repeatedly:
  byte i;
  byte present = 0;
  byte type_s;
  byte addr[12];
  byte bufcrc = 0;
  uint8_t data[9];
  
  if ( !ds.search(addr)) {
    //Serial.println("No more addresses.");
    //Serial.println();
    ds.reset_search();
    Serial.println("No more addresses.!");
    delay(1000);
    return;
  }

  
  //*
  
  Serial.print("Found device, ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
    }
  Serial.println();  
  //*/
  ///*
  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println("SEND CMD:.. ");
  delay(50);
  ds.reset();
  ds.select(addr);
  byte cmd[] = {0x08, 100,0};
  sendCommand(cmd,2);
  delay(2000);    

  ds.reset();
  ds.select(addr);
  cmd[0] = 0x07;
  cmd[1] = 0x00;
  sendCommand(cmd,2);
  delay(2000);
  /*
  bool valid = false;
  while(!valid) { 
      ds.reset();
      ds.select(addr);
      ds.write(0x01, 1);        // start conversion, with parasite power on at the end
      delayMicroseconds(163);
      ds.read_bytes(data,9);
      if (OneWire::crc8(data, 6) == data[6]) {
        valid = true;
        }
      }
    Serial.println("VALID");  
  */
  delay(100);
}
