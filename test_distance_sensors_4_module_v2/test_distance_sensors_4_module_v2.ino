//works with test_distance_sensors_processing_4_module

#include <Adafruit_NeoPixel.h>
#define PIXEL_PIN    6
#define PIXEL_COUNT 100

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

byte threshold = 15;

byte reshuffle_tab[100]={
  0,1,2,3,4,       25,26,27,28,29,
  5,6,7,8,9,       30,31,32,33,34,
  10,11,12,13,14,  35,36,37,38,39,
  15,16,17,18,19,  40,41,42,43,44,
  20,21,22,23,24,  45,46,47,48,49,
  
  50,51,52,53,54,  75,76,77,78,79,
  55,56,57,58,59,  80,81,82,83,84,
  60,61,62,63,64,  85,86,87,88,89,
  65,66,67,68,69,  90,91,92,93,94,
  70,71,72,73,74,  95,96,97,98,99, 
};

int A_PIN = 8;
int B_PIN = 9;
int C_PIN = 10;
int D_PIN = 11;
int E_PIN = 12;
int F_PIN = 4;
int G_PIN = 5;
int INH_PIN = 13;
int SENSOR_PIN = A0;
int POT_PIN = A1;

void updateThreshold() {
  threshold = map(analogRead(POT_PIN), 0, 1023, 0, 255);
}

class Module {
  private:
    byte SENSOR_PIN;
  public:
    Module(byte SENSOR_PIN = A0): SENSOR_PIN(SENSOR_PIN) { pinMode(SENSOR_PIN, INPUT); }
    byte readings[25] = {0};
    void collect_data() {
      for (int i = 0; i < 25; i++) {
        PORTB = i; //PORTB - 8...13 pin
        //select(23);
        if (!(i % 8)) delayMicroseconds(100);
        readings[i] = analogRead(SENSOR_PIN) / 4;
       // delay(100); for debugging
      }
      digitalWrite(INH_PIN, HIGH); // disable ir shiet
    }
    void sendDataViaSerial() {
      for (int i = 0; i < 25; i++)
        Serial.write(readings[i]);
        // Serial.write(abs(readings[i]));
    }

    void setSensorPin(byte SENSOR_PIN){
      this->SENSOR_PIN = SENSOR_PIN;
      pinMode(SENSOR_PIN, INPUT);
    }
};
class IRPannel {
  private:
    Module modules[4];

    byte F_PIN;
    byte G_PIN;
    byte INH_PIN;
    
    void fillBigTab() {
      byte i, j;
      for (j = 0; j < 25; j+=5)
        for (i = 0; i < 5; i++){
          readings[i + (j << 1)] = modules[0].readings[i + j];
          readings[i + (j << 1) + 5] = modules[1].readings[i + j];
          readings[i + (j << 1) + 50] = modules[2].readings[i + j];
          readings[i + (j << 1) + 55] = modules[3].readings[i + j];
        }
    }
    
  public:
    byte readings[100] = {0};
    int refference_table[100] = {0};
      void createRefferenceTable(byte how_many_averages = 3){
       for (int i = 0; i < 100; i++) // fill reference table with zeros
         refference_table[i] = 0;   
       for(int j = 0; j < how_many_averages; j++){
        collect_data();
        for (int i = 0; i < 100; i++)
         refference_table[i] += readings[i];
       }
       for (int i = 0; i < 100; i++) // fill reference table with zeros
         refference_table[i] /= how_many_averages;          
      }

    IRPannel(byte F_PIN, byte G_PIN, byte INH_PIN, byte SENSOR_PIN): F_PIN(F_PIN), G_PIN(G_PIN), INH_PIN(INH_PIN) {
      for (int i = 0; i < 4; i++)
          modules[i].setSensorPin(SENSOR_PIN);      
          
      for (int i = 8; i < 13; i++)
        pinMode(i, OUTPUT); //sets all pins from 8 to 12 to output
        
      pinMode(F_PIN, OUTPUT);
      pinMode(G_PIN, OUTPUT);
      pinMode(INH_PIN, OUTPUT);
      pinMode(SENSOR_PIN, INPUT);
      digitalWrite(F_PIN, LOW);
      digitalWrite(G_PIN, LOW);
      digitalWrite(INH_PIN, HIGH);

      for (int i = 0; i < 100; i++) // for debug
        readings[i] = i;
    }
    
    void selectModule(byte id) {
      switch (id) {
        case 0:
          digitalWrite(F_PIN, LOW);
          digitalWrite(G_PIN, LOW);
          break;
        case 1:
          digitalWrite(F_PIN, HIGH);
          digitalWrite(G_PIN, LOW);
          break;
        case 2:
          digitalWrite(F_PIN, LOW);
          digitalWrite(G_PIN, HIGH);
          break;
        case 3:
          digitalWrite(F_PIN, HIGH);
          digitalWrite(G_PIN, HIGH);
          break;
      }
    }

    void collect_data() {
      digitalWrite(INH_PIN, LOW); //enable use power for ir
      for (int i = 0; i < 4; i++) {
        selectModule(i);
        delayMicroseconds(100); //give some time for 4052 & 4051 chips
        modules[i].collect_data();
      }
     digitalWrite(INH_PIN, HIGH); //disable use power for ir
      fillBigTab(); // rewrite values from 4 smaller tabs to one big
    }

    void sendDataViaSerial() {
      for (int i = 0; i < 100; i++)
        Serial.write(abs(readings[i] - refference_table[i]));
        //Serial.write(readings[i]);
    }

} panel(4, 5, 13, A0);

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

int rainbow_counter = 0;

void do_rainbow_tick() {
  for (int i = 0; i < strip.numPixels(); i++) {
    if(abs(panel.readings[i] - panel.refference_table[i])>threshold)
     strip.setPixelColor(reshuffle_tab[i], Wheel((i + rainbow_counter+128) & 255));
     else 
     strip.setPixelColor(reshuffle_tab[i], Wheel((i + rainbow_counter) & 255));
  }
  rainbow_counter++;
  if (rainbow_counter > 255) rainbow_counter = 0;

  //delay(20);
}

void setup() {
  strip.begin();
  // strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
  // start serial port at 115200 bps:
  Serial.begin(115200);
  pinMode(A_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  pinMode(C_PIN, OUTPUT);
  pinMode(D_PIN, OUTPUT);
  pinMode(E_PIN, OUTPUT);
  pinMode(F_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(INH_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);

  digitalWrite(INH_PIN, HIGH); // disable ir shiet
  digitalWrite(F_PIN, LOW); // select 1st module just for debug
  digitalWrite(G_PIN, LOW); // select 1st module just for debug

  panel.createRefferenceTable(10);
  
  establishContact();  // send a byte to establish contact until receiver responds
}

void loop() {
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {

    // get incoming byte:
    int inByte = Serial.read();
    panel.sendDataViaSerial();
  }
  panel.collect_data();
  do_rainbow_tick(); 
  // do_rainbow_tick();
    strip.show();
    if(analogRead(A1)>200) panel.createRefferenceTable(10);
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

/*
  Processing sketch to run with this example:



*/
