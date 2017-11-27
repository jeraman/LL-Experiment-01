/*

   VRM INTERFACE PROTOTYPE
   Name: Jeronimo Barbosa
   Date: November 27 2017.

*/

//arduino ports
const int buttonPin  = 2;
const int buttonLED  = 13;
const int  pot1port  = A1;
const int  pot2port  = A4;

//variables
int  buttonState     = 0;
int  buttonLastState = 0;
int  pot1val         = 0;
int  pot2val         = 0;
bool isButtonOn      = false;


//functions
bool needToTurnLEDOnOff()
{
  return (buttonState == 1 && buttonState != buttonLastState);
}


void arcadeButtonControl()
{
  buttonState = digitalRead(buttonPin);

  if (needToTurnLEDOnOff())
    isButtonOn = !isButtonOn;

  if (isButtonOn)
    digitalWrite(buttonLED, HIGH);

  else
    digitalWrite(buttonLED, LOW);

  buttonLastState = buttonState;
}


void potentiometersControl()
{
  pot1val = analogRead(A1);    // read the value from the sensor
  pot2val = analogRead(A4);  // read the value from the sensor
}


void debugRaw()
{
  if (isButtonOn)
    Serial.print("On! ");
  else
    Serial.print("Off... ");

  Serial.print("   pot 1: ");
  Serial.print(pot1val);
  Serial.print("   pot 2: ");
  Serial.print(pot2val);

  Serial.println();
}

//iterative loop
void setup()
{

  while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);

  pinMode(buttonLED, OUTPUT);
  pinMode(buttonPin, INPUT);

  Serial.println("Arduino initialized");
}


void loop()
{

  arcadeButtonControl();
  potentiometersControl();
  debugRaw();
  
  delay(10);
}
