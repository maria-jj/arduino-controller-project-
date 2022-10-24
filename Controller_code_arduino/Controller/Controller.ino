//TILT
const int tiltPin = 13;     // the number of the pushtilt pin
int tiltState = 0;  // variable for reading the pushtilt status\

//JOYSTICK
const int SW_pin = 8; // digital pin connected to switch output
const int vrX_pin = A0; // analog pin connected to X output
const int vrY_pin = A1; // analog pin connected to Y output

//TOUCH
const int touchPin = 4;

void setup() {
  //TILT
  pinMode(tiltPin, INPUT);

  //JOYSTICK
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  
  //TOUCH
  pinMode(touchPin, INPUT);

  
  Serial.begin(115200); 
}

void loop() {

  //TILT
  tiltState = digitalRead(tiltPin);
  Serial.print("Tilt State:");
  Serial.print(tiltState);
  Serial.print("\n");
  
  //JOYSTICK
  Serial.print("Switch:  ");
  Serial.print(digitalRead(SW_pin));
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(analogRead(vrX_pin));
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(analogRead(vrY_pin));
  Serial.print("\n");

  //TOUCH
  if (digitalRead(touchPin) == HIGH){
    Serial.println("Sensor is touched");
    Serial.print("\n");
  }


  Serial.print("\n");
  delay(500);

}
