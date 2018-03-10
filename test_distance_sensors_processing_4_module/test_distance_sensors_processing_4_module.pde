import processing.serial.*; //<>//

Serial myPort;                       // The serial port
int[] serialInArray = new int[100];    // Where we'll put what we receive
int serialCount = 0;                 // A count of how many bytes we receive
int size_of_grid = 10; //(10x10)

boolean binary_data = true; // got raw readings or binary 1/0?

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

    if (binary_data) {
      if (serialInArray[i]==1)
        fill(255);
      else
        fill(0);
    } else fill(serialInArray[i]);


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