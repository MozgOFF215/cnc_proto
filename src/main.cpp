#include "header.h"
#include "main.h"

String inputString = "";     // a String to hold incoming data
bool stringComplete = false; // whether the string is complete

void setup()
{
  pinMode(p_led, OUTPUT);
  pinMode(p_turnFwd, OUTPUT);
  pinMode(p_turnBwd, OUTPUT);
  pinMode(p_enA, OUTPUT);
  pinMode(p_end1, INPUT_PULLUP);
  pinMode(p_end2, INPUT_PULLUP);

  initInterrupts();

  Serial.begin(38400);

  state.endMovingFunction = nullptr;

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
}