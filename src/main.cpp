#include "header.h"

String inputString = "";     // a String to hold incoming data
bool stringComplete = false; // whether the string is complete

void setup()
{
  pinMode(p_led, OUTPUT);
  pinMode(X_turnFwd, OUTPUT);
  pinMode(X_turnBwd, OUTPUT);
  pinMode(X_enA, OUTPUT);
  pinMode(X_end1, INPUT_PULLUP);
  pinMode(X_end2, INPUT_PULLUP);

  //initController();

  initInterrupts();

  Serial.begin(38400);

  X_state.endMovingFunction = nullptr;

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop()
{
  if (Serial.available())
  {
    char inChar = (char)Serial.read();
    Serial.print(inChar);
    if (inChar == '\n')
      stringComplete = true;
    else
      inputString += inChar;
  }

  if (stringComplete)
  {
    SHOW_MESSAGE("command: " + inputString);
    stringComplete = false;
    parse_my(inputString.toUpperCase().c_str());
    inputString = "";
  }

  //controller();
}