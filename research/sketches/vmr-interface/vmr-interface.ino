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

//button control variables
int   buttonState     = 0;
int   buttonLastState = 0;
float elapsedDownTime = 0;
bool  isButtonLightOn = false;
unsigned long  buttonPressedTimestamp = 0;

//potentiometer variables
int  pot1val         = 0;
int  pot2val         = 0;


//functions
void turnOnOffLED()
{
  isButtonLightOn = !isButtonLightOn;

  if (isButtonLightOn)
    digitalWrite(buttonLED, HIGH);
  else
    digitalWrite(buttonLED, LOW);
}


bool isButtonDown()
{
  return (buttonState == 1);
}


bool buttonHasChangedState()
{
  return (buttonState != buttonLastState);
}


bool buttonHasBeenPressed()
{
  return (isButtonDown() && buttonHasChangedState());
}


bool buttonHasBeenDownForTwoSeconds()
{
  if (buttonPressedTimestamp == 0) return false;
  elapsedDownTime = (millis() - buttonPressedTimestamp) / 1000;
  return (isButtonDown() && !buttonHasChangedState() && elapsedDownTime >= 2.0);
}


bool isSecondTimeButtonPressedInASecond()
{
  if (buttonPressedTimestamp == 0) return false;
  elapsedDownTime = (millis() - buttonPressedTimestamp) / 1000;
  return (elapsedDownTime < 1.0);
}


void resetButtonDownCounter()
{
  buttonPressedTimestamp = 0;
  elapsedDownTime = 0;
}


void arcadeButtonControl()
{
  if (buttonHasBeenPressed())
  {
    turnOnOffLED();

    if (isSecondTimeButtonPressedInASecond())
      buttonPressedTwiceEvent();
    else  
      buttonPressedEvent();
  }

  if (buttonHasBeenDownForTwoSeconds())
    buttonPressedAndHoldEvent();
  else if (elapsedDownTime >= 2.0)
    resetButtonDownCounter();


  buttonLastState = buttonState;
}


void potentiometersControl()
{
  pot1val = analogRead(A1);    // read the value from the sensor
  pot2val = analogRead(A4);  // read the value from the sensor
}


void buttonPressedEvent()
{
  buttonPressedTimestamp = millis();
  Serial.println("Red button pressed!");
}


void buttonPressedTwiceEvent()
{
  resetButtonDownCounter();
  Serial.println("Red button pressed TWICE in a second!");
}


void buttonPressedAndHoldEvent()
{
  resetButtonDownCounter();
  Serial.println("Red button pressed and HOLD for 2 seconds!");
}


void debugRaw()
{
  if (isButtonLightOn)
    Serial.print("Light On! ");
  else
    Serial.print("Light Off... ");

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
  buttonState = digitalRead(buttonPin);

  arcadeButtonControl();
  potentiometersControl();
  //debugRaw();


  delay(10);
}
