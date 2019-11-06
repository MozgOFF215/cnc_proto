#ifndef TEST_PC_CPP
#include <Arduino.h>
#else
#include "arduino.h"
#endif

#include "macros.h"
#include "pins.h"
#include "main.h"
#include "interrupts.h"
#include "controller.h"
#include "parser_my.h"

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

  initController(&X_pidState);

  initInterrupts();

  Serial.begin(38400);

  X_state.endMovingFunction = nullptr;

  if (!X_config.IsEndMinus() && !X_config.IsEndPlus())
  {
    X_state.isStoped = false;
  }

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
#ifndef TEST_PC_CPP
    SHOW_MESSAGE("command: " + inputString);
#else
    printf("command: %s", inputString);
#endif

    stringComplete = false;
    parse_my(inputString.c_str());
    inputString = "";
  }

  controller(&X_config, &X_state, &X_pidState);
}