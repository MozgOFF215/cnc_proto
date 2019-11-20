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

  pinMode(Y_turnFwd, OUTPUT);
  pinMode(Y_turnBwd, OUTPUT);
  pinMode(Y_enB, OUTPUT);
  pinMode(Y_end1, INPUT_PULLUP);
  pinMode(Y_end2, INPUT_PULLUP);

  pinMode(Z_dir, OUTPUT);
  pinMode(Z_step, OUTPUT);
  pinMode(Z_limitMax, INPUT_PULLUP);

  initControllerX();
  initControllerY();
  initControllerZ();

  initInterrupts();

  Serial.begin(38400);

  if (!X_state.IsEndMinus() && !X_state.IsEndPlus())
  {
    X_state.isStoped = false;
  }

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void send(const char *str)
{
  Serial.printf("%s", str);
}

void loop()
{
  if (Serial.available())
  {
    char inChar = (char)Serial.read();
    //Serial.print(inChar);
    if (inChar == '\n')
      stringComplete = true;
    else
      inputString += inChar;
  }

  if (stringComplete)
  {
#ifndef TEST_PC_CPP
    //SHOW_MESSAGE(inputString);
#else
    printf("command: %s", inputString);
#endif

    stringComplete = false;
    parse_my(inputString.c_str(), &send);
    inputString = "";
  }

  controller(&X_state);
  controller(&Y_state);
  controller(&Z_state);
}