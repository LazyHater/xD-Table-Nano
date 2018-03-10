//works with test_distance_sensors_processing_4_module

#include "Led_Table_Pixels.h"
#include "IRPanel.h"
#include "EffectMenager.h"
#include "Switch.h"

#define PIXEL_COUNT 100
#define PIXEL_PIN    6
#define A_PIN  8
#define B_PIN  9
#define C_PIN  10
#define D_PIN  11
#define E_PIN  12
#define F_PIN  4
#define G_PIN  5
#define INH_PIN  13
#define IR_SENSOR_PIN  A0
#define SWITCH_PIN  A1
#define POT_PIN A2

Led_Table_Pixels strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
IRPanel irPanel(F_PIN, G_PIN, INH_PIN, IR_SENSOR_PIN, 10);
EffectMenager effectMenager(&strip, &irPanel);
Switch swith(SWITCH_PIN);

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println('A');   // send a capital A
    //Serial.println(swith.readSwitchState());   // send a capital A
    delay(300);
  }
}

void setup() {
  randomSeed((unsigned long int) analogRead((unsigned char)  A3  ) );
  strip.begin();
  strip.setBrightness(255);
  strip.show(); // Initialize all pixels to 'off'
  // start serial port at 115200 bps:
  Serial.begin(115200);
  irPanel.createRefferenceTable();

  //activeEffect = &rainbow;
  //establishContact();  // send a byte to establish contact until receiver responds
}

void loop() {
  // if we get a valid byte, read analog ins:

  switch (swith.readSwitchState()) {
  case Switch::ONE:
        break;
  case Switch::TWO:
      if (Serial.available() > 0) {
    	  irPanel.resolveKMeans();
    	  Serial.read(); // dela with that byte
        //irPanel.sendDataViaSerial();
      }

      irPanel.collectData();
      irPanel.convertReadingsToBoll();
      effectMenager.nextFrame();
      effectMenager.show();

      break;

    case Switch::THREE:
      irPanel.createRefferenceTable();
      static int tmp = strip.getBrightness();
      strip.setBrightness(0);
      strip.show();
      strip.setBrightness(tmp);
      while (swith.readSwitchState() == Switch::THREE); //wait until state changed
      break;
    default:
      break;
  }

  if((swith.getState() == Switch::ONE)&&(swith.getPreviousState() == Switch::TWO)){
	  effectMenager.nextEffect();
  }

}


/*
  Processing sketch to run with this example:
  //in arduino sketch uncomment establishContact();

  import processing.serial.*;

  Serial myPort;                       // The serial port
  int[] serialInArray = new int[100];    // Where we'll put what we receive
  int serialCount = 0;                 // A count of how many bytes we receive

  int threshold = 10; // if 0 displays raw values, othervise colors that rects which value is above threshold
  int size_of_grid = 10; //(10x10)

  boolean firstContact = false;        // Whether we've heard from the microcontroller
  long startTime = 0;
  long render_time = 0;
  void setup() {
  size(1000, 1000);  // Stage size


  // Print a list of the serial ports for debugging purposes
  // if using Processing 2.1 or later, use Serial.printArray()
  println((Object[])Serial.list());

  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 115200);
  }



  void drawRectangles() {
  int w_rect = width / size_of_grid;
  int h_rect = height / size_of_grid;

  for (int i = 0; i < size_of_grid*size_of_grid; i++) {
    int x =(i % size_of_grid)*w_rect;
    int y =(i / size_of_grid)*h_rect;
    //fill(i*10); // for test

    if (threshold!=0) {
      if (serialInArray[i] > threshold)
      fill(255);
      else
        fill(0);
    } else {
      fill(serialInArray[i]);
    }

    rect(x, y, x+w_rect, y+h_rect);
    //rectangle has drawn, so now is time for text in the center of rectangle
    fill(0, 255, 255); // lets try light blue
    textAlign(CENTER, CENTER);
    textSize(30);
    float voltage = (serialInArray[i]*5.0f/255.0f);
    int tmp = (int)(voltage * 100);
    voltage = tmp/100.0f;
    String s = "";
    s+=serialInArray[i]+"\n"+voltage+"V";

    text(s, x+w_rect/2.0, y+h_rect/2); // int the center of each rectangle
  }
  }

  void draw() {
  background(0);
  long startv2 = millis();
  drawRectangles();
  render_time = millis()-startv2;
  }

  void serialEvent(Serial myPort) {
  // read a byte from the serial port:
  int inByte = myPort.read();
  // if this is the first byte received, and it's an A,
  // clear the serial buffer and note that you've
  // had first contact from the microcontroller.
  // Otherwise, add the incoming byte to the array:
  if (firstContact == false) {
    if (inByte == 'A') {
      myPort.clear();          // clear the serial port buffer
      firstContact = true;     // you've had first contact from the microcontroller
      myPort.write('A');       // ask for more
    }
  } else {
    // Add the latest byte from the serial port to array:
    serialInArray[serialCount] = inByte;
    serialCount++;

    // If we have 25 bytes:
    if (serialCount > 99 ) {
      // print the values (for debugging purposes only):
      println("frame lenght: "+(millis()-startTime)+" render time: "+render_time);
      // Send a capital A to request new sensor readings:
      myPort.write('A');
      startTime = millis();
      // Reset serialCount:
      serialCount = 0;
    }
  }
  }


*/

