/*
  DeautherX-lib — Deauth Detector Example
  This example demonstrates how to use the DeautherDetector class
  to detect deauthentication and disassociation attacks in the background.

  Based on the original DeauthDetector by spacehuhn.
*/

#include <ESP8266WiFi.h>
#include <DeautherLib.h>

DeautherDetector detector;

void onAttackStarted() {
  Serial.println("!!! ATTACK DETECTED !!!");
}

void onAttackStopped() {
  Serial.println("... attack stopped ...");
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nDeautherX - Deauth Detector");

  // Configuration
  DeautherDetector::Config cfg;
  cfg.channels = DCHANNEL_COMMON; // Scan channels 1-11
  cfg.pkt_rate = 5;               // Alert if > 5 packets/cycle
  cfg.pkt_time = 1;               // Number of detection cycles to trigger alert
  cfg.led_pin  = 2;               // Built-in LED on ESP-12
  cfg.led_invert = true;          // ESP-12 LED is active-low

  // Setup callbacks
  detector.onAttackStarted(onAttackStarted);
  detector.onAttackStopped(onAttackStopped);

  // Start monitoring
  detector.begin(cfg);

  Serial.println("Monitoring started. Send some deauth packets to test!");
}

void loop() {
  // Update detector logic
  detector.update();

  // Print stats every second (approx)
  static unsigned long last_print = 0;
  if (millis() - last_print >= 1000) {
    last_print = millis();
    
    if (detector.isAttacked()) {
      Serial.print("[!] ATTACKING | ");
    } else {
      Serial.print("[.] Monitoring | ");
    }
    
    Serial.print("Packets/cycle: ");
    Serial.println(detector.getPacketRate());
  }
}
