/***********************************
 * 
 *    Creation and computation Experiment 4 
 * 
 *    Orlando Bascunan
 * 
 *    November 2016
 * 
 *    Based on Adafruit IO Publish Example
 *    
 *    
 *    The main code is on the loop function, I look for high and low values, 
 *    store them and send them as data to the feed.
 *    
 *    The sensibility added is too avoid false positives.
 *    
 */ //////////////////////////////////
 

// 
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

#include "config.h"

int count = 0;
int sensorPin = A0;
int sensorValue = 0;
int lastValue = 0;

int highValue = 0;
int lowValue = 1024;

int sensibility = 5;
bool expanding = true;


AdafruitIO_Feed *rFeed = io.feed("breathing-B");

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while (! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  sensorValue = analogRead(sensorPin);

  if (expanding) {
    if (sensorValue > highValue) {
      highValue = sensorValue;
    }

    if (sensorValue < highValue - sensibility) {
      //highest point
      expanding = false;
      rFeed->save(highValue);
      highValue = 0;
      Serial.println("exalando");
    }
  } else {
    if (sensorValue < lowValue) {
      lowValue = sensorValue;
    }
    if (sensorValue > lowValue + sensibility) {
      //lowest point
      expanding = true;
      rFeed->save(lowValue);      
      lowValue = 1024;
      Serial.println("inhalando");
    }
  }

  lastValue = sensorValue;

  // save count to the 'counter' feed on Adafruit IO
  //Serial.print("sending -> ");
  Serial.println(sensorValue);
 // rFeed->save(sensorValue);

  // wait one second (1000 milliseconds == 1 second)
  delay(20);

}

