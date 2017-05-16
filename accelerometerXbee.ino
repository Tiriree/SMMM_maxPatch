//Accelerometer data send to xbee 
//accelerometer code based on http://bildr.org/?s=adxl335
//by Sebastian Morales

//Analog read pins
const int xPin = 0;
const int yPin = 1;
const int zPin = 2;

//The minimum and maximum values that came from
//the accelerometer while standing still
//You very well may need to change these
int minVal = 265;
int maxVal = 402;

double x;
double y;
double z;
double oldX = 0;
double oldY = 0;
double oldZ = 0;

int lastSend = 0;
int graceTime = 3000;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}


void loop() {
  //read the analog values from the accelerometer
  int xRead = analogRead(xPin);
  int yRead = analogRead(yPin);
  int zRead = analogRead(zPin);

  //convert read values to degrees -90 to 90 - Needed for atan2
  int xAng = map(xRead, minVal, maxVal, -90, 90);
  int yAng = map(yRead, minVal, maxVal, -90, 90);
  int zAng = map(zRead, minVal, maxVal, -90, 90);

  //Caculate 360deg values like so: atan2(-yAng, -zAng)
  //atan2 outputs the value of -π to π (radians)
  //We are then converting the radians to degrees
  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  //  if (oldX != x || oldY != y || oldZ != z) {
  if (check(oldX, x) || check(oldY, y) || check(oldZ, z)) {
    oldX = x;
    oldY = y;
    oldZ = z;
    //Output the caculations
    //    Serial.print( "{\"broom2\": ");
    //    Serial.print("{\"x\": \""); Serial.print(x); Serial.print("\", \"y\": \""); Serial.print(y);
    //    Serial.print("\", \"z\": \""); Serial.print(z); Serial.println("\"}}");

    //    Serial1.print( "{\"broom2\": ");
    //    Serial1.print("{\"x\": \""); Serial1.print(x); Serial1.print("\", \"y\": \""); Serial1.print(y);
    //    Serial1.print("\", \"z\": \""); Serial1.print(z); Serial1.println("\"}}");

    Serial1.print(x); Serial1.print(", "); Serial1.print(y); Serial1.print(", "); Serial1.println(z);
    Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.println(z);
    lastSend = millis();
  }

  if (millis() - graceTime > lastSend) {
    Serial1.print(0); Serial1.print(", "); Serial1.print(0); Serial1.print(", "); Serial1.println(0);
    lastSend = millis();
  }
  
  delay(10);//just here to slow down the serial output - Easier to read
}

boolean check(double old, double current) {
  int range = 25;
  if (old - range < current && old + range > current) {
    return (false);
  } else {
    return (true);
  }
}

