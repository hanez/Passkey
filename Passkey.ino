// Passkey for Arduino using the Digispark Rev.3 board.
// Copyright 2022 by Johannes Findeisen <you@hanez.org>
// Licensed under the terms of the MIT license.

// Required to make Passkey a HID device. You need to install it.
// You can define your keyboard layout in:
// ~/.arduino15/packages/digistump/hardware/avr/1.7.5/libraries/DigisparkKeyboard/keylayouts.h
// By default US keyboard layout is used.
#include <DigiKeyboard.h>

// Passkey includes the Blinker library from:
// https://github.com/MajenkoLibraries/Blinker
// Since the code is included in Passkey you do not have 
// to do anything to compile Passkey.
// Blinker is licensed under the BSD-3-Clause license.
// See Blinker.LICENSE for the full license of Blinker.
#include "Blinker.h"

// You need to edit this file to add your passwords.
#include "Passwords.h"

// The button pin
#define BUTTON_PIN 0
// The LED pin
#define LED_PIN 1
// The pause after last button press before executing a command
#define PAUSE 2000

// Local variables
unsigned long last;
unsigned int count;

// Create blinker object
Blinker blinker(LED_PIN);

void setup() {
  // Set button pin to high.
  pinMode(BUTTON_PIN, INPUT);
  // Set BUTTON_PIN pin to high because the button is connected 
  // to GND when pressed and will go LOW
  digitalWrite(BUTTON_PIN, HIGH);

  // Set blink effect (ON, OFF) in milliseconds
  blinker.setDelay(25, 9075);
  blinker.start();

  // Initialize HID
  DigiKeyboard.delay(0);
  DigiKeyboard.sendKeyStroke(0);

  // Light up the LED for a second to show that passkey is ready
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // Start LED blink effect
  blinker.blink();

  // Read button state
  if (digitalRead(BUTTON_PIN) == LOW) {
    // Button is pressed
    count++;
    digitalWrite(LED_PIN, HIGH);
    // Wait until button is released
    while (digitalRead(BUTTON_PIN) == LOW) {
      delay(1);
    }
    // Set the last press timestamp
    last = millis();
    digitalWrite(LED_PIN, LOW);
  }

  // Wait PAUSE time before executing a command
  if (count > 0 && (millis() - last) >= PAUSE) {
    // The switch case is the number of button presses
    switch (count) {
      case 1:
        DigiKeyboard.println(passwords[0]);
        break;
      case 2:
        DigiKeyboard.println(passwords[1]);
        break;
      case 3:
        DigiKeyboard.println(passwords[2]);
        break;
      case 5:
        // You can use print instead of println if you don't want 
        // to hit enter automatically after inserting the password
        DigiKeyboard.print(passwords[3]);
        break;
      default:
        break;
    }
    // Reset the counter
    count = 0;
  }
}
