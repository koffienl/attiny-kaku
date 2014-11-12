/*
                    ATtiny 85
                      _____
  RESET      A0, D5 -|  A  |- VCC
  RF TRANS   A3, D3 -|  T  |- A1, D2  RSCK    RF RCV
             A2, D4 -|  t  |- D1      MISO    RELAY
                GND -|_____|- D0      MOSI

*/

#include <NewRemoteReceiver.h>
#include <NewRemoteTransmitter.h>

#define senderPin 3
#define receiverPin 2
#define relayPin 1

void setup() {
  pinMode(senderPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  NewRemoteReceiver::init(0, 2, ToggleRelay);
}

void loop() {
}

void ToggleRelay(NewRemoteCode receivedCode) {  
  // Only react on 8934706-8
  if (receivedCode.address == 8934706 && receivedCode.unit == 8) {  
    // Disable the receiver; otherwise it might pick up the retransmit as well.
    NewRemoteReceiver::disable();

    // Need interrupts for delay()
    interrupts();

    // Find out if we received ON or OFF, and change relay
    if (receivedCode.switchType == 0) {
      digitalWrite(relayPin,LOW);
    }
    if (receivedCode.switchType == 1) {
      digitalWrite(relayPin,HIGH);
    }
    // Wait 5 seconds before sending.
    delay(5000);

    // Create a new transmitter and send confirmation, use digital pin 11 as output pin
    NewRemoteTransmitter transmitter(101010, senderPin, receivedCode.period);    
    transmitter.sendUnit(5, receivedCode.switchType);

    NewRemoteReceiver::enable();
  }
}

