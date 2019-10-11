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

  attachInterrupt(p_enc1, encoderInterrupt, CHANGE);

  attachInterrupt(p_end1, endStopInterrupt, RISING);
  attachInterrupt(p_end2, endStopInterrupt, RISING);

  Serial.begin(38400);

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
    Serial.println("command: " + inputString);
    inputString = "";
    stringComplete = false;
    parse_my(inputString.c_str());
  }

  // moveTo(0);
  // delay(config.timeDrive);
  // moveTo(3500);
  // delay(config.timeDrive);
}

void moveTo(long pos)
{
  if (state.currentPos == pos || digitalRead(p_end1) || digitalRead(p_end2))
    return;

  if (state.currentPos < pos)
  {
    if ((pos - state.currentPos) < config.slowDistance)
      goForward(pos, config.minSpeed);
    else
      goForward(pos, config.maxSpeed);
    return;
  }

  if ((state.currentPos - pos) < config.slowDistance)
    goBack(pos, config.minSpeed);
  else
    goBack(pos, config.maxSpeed);
}

void goForward(long pos, int speed)
{
  digitalWrite(p_led, HIGH);
  digitalWrite(p_turnFwd, HIGH);
  digitalWrite(p_turnBwd, LOW);
  analogWrite(p_enA, speed);
  state.requiredPos = pos;
  state.currentDirect = true;
  state.isStoped = false;

  Serial.println((String) "Forward to " + pos + " s: " + speed);
}

void goBack(long pos, int speed)
{
  digitalWrite(p_led, LOW);
  digitalWrite(p_turnFwd, LOW);
  digitalWrite(p_turnBwd, HIGH);
  analogWrite(p_enA, speed);
  state.requiredPos = pos;
  state.currentDirect = false;
  state.isStoped = false;

  Serial.println((String) "Back to " + pos + " s: " + speed);
}

void stop()
{
  digitalWrite(p_led, LOW);
  digitalWrite(p_turnFwd, LOW);
  digitalWrite(p_turnBwd, LOW);
  analogWrite(p_enA, config.maxSpeed);
  state.isStoped = true;

  Serial.println((String) "Stop. Required:" + state.requiredPos);
  Serial.println((String) " current:" + state.currentPos);
}

void encoderInterrupt()
{
  if (digitalRead(p_enc1) && digitalRead(p_enc2))
    state.currentPos++;
  else
    state.currentPos--;
  if (!digitalRead(p_enc1) && digitalRead(p_enc2))
    state.currentPos--;
  else
    state.currentPos++;

  if (state.isStoped)
    return;

  if (!state.currentDirect)
  {
    if (state.currentPos <= state.requiredPos)
    {
      stop();
      return;
    }

    if ((state.currentPos - state.requiredPos) < config.slowDistance)
      analogWrite(p_enA, config.minSpeed);
  }

  if (state.currentDirect)
  {
    if (state.currentPos >= state.requiredPos)
    {
      stop();
      return;
    }

    if ((state.requiredPos - state.currentPos) < config.slowDistance)
      analogWrite(p_enA, config.minSpeed);
  }
}

void endStopInterrupt()
{
  stop();
}