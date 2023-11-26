
import processing.serial.*;

String portName = "/dev/cu.usbmodem11101";
Serial port;
float []data = new float [3];

void setup() 
{
  lights();
  size(300, 300, P3D);
  port = new Serial(this, portName, 115200);
}

void draw() {
  background(230);
  translate(width / 2, height / 2, 0);
  rotateX(radians(data[1])*90);
  rotateZ(radians(data[0])*90);

  int size = 10;
  box(20 * size, 1 * size, 15 * size);
  translate(0, -4 * size, 0);
  
  //saveFrame("frames/######.png");
}

void serialEvent(Serial p)
{
  String inString = port.readStringUntil('\n');
  if (inString != null)
  {
    inString = trim(inString);
    data = float(split(inString, ','));
    println(data);
  }
}
