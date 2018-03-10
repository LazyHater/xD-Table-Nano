#include <Adafruit_NeoPixel.h>
#define PIXEL_PIN    6 
#define PIXEL_COUNT 25

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

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
 
int readings[25] = {0};

int threshold = 150;
void updateThreshold(){
  threshold = map(analogRead(POT_PIN),0,1023,0,255);
}

void select(byte tmp){
  PORTB = tmp;
}
void collect_data(){
  for (int i = 0; i < 25; i++){
    select(i);
    if (!(i%8)) delayMicroseconds(100);
    readings[i] = analogRead(SENSOR_PIN) / 4;
  }
  digitalWrite(INH_PIN, HIGH); // disable ir shiet
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

int rainbow_counter = 0;

void do_rainbow_tick(){    
  for(int i=0; i<strip.numPixels(); i++) {
    if(readings[i] > threshold)
     strip.setPixelColor(i, Wheel((i+rainbow_counter) & 255));
     else
     strip.setPixelColor(i,255,255,255);
  }
  rainbow_counter++;
  if (rainbow_counter > 255) rainbow_counter = 0;

  delay(20);
}

void setup() {

  strip.begin();
 // strip.setBrightness(255);
  strip.show(); // Initialize all pixels to 'off'
  // start serial port at 9600 bps:
  Serial.begin(9600);
  pinMode(A_PIN,OUTPUT);
  pinMode(B_PIN,OUTPUT);
  pinMode(C_PIN,OUTPUT);
  pinMode(D_PIN,OUTPUT);
  pinMode(E_PIN,OUTPUT);
  pinMode(F_PIN,OUTPUT);
  pinMode(G_PIN,OUTPUT);
  pinMode(INH_PIN,OUTPUT);
  pinMode(SENSOR_PIN,INPUT);

  digitalWrite(INH_PIN, HIGH); // disable ir shiet
  digitalWrite(F_PIN, LOW); // select 1st module just for debug
  digitalWrite(G_PIN, LOW); // select 1st module just for debug
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  for (int i = 0; i<25; i++)
    readings[i] = i*10;

  establishContact();  // send a byte to establish contact until receiver responds
}

void loop() {
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    
    // get incoming byte:
    int inByte = Serial.read();
  //  long d_t = millis();
  //  for (int i = 0; i<40; i++)
    
   // readings[0] = millis() - d_t;
    for (int i = 0; i<25; i++)
     Serial.write(readings[i]);
  }
  updateThreshold();
  collect_data();
  do_rainbow_tick();
  strip.show();
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
