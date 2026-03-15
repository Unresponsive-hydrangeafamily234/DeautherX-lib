/* DeautherX-lib — Master header
   https://github.com/BlackTechX011/DeautherX-lib
   MIT License

   Include this ONE file to access the entire library.

   #include <DeautherLib.h>
*/

#pragma once

// ─── Layer 0: Utilities ─────────────────────────────────────────────────────
#include "utils/DeautherMac.h"
#include "utils/DeautherChannel.h"
#include "utils/DeautherStrHelper.h"
#include "utils/DeautherVendor.h"

// ─── Layer 0: Radio ─────────────────────────────────────────────────────────
#include "radio/DeautherRadio.h"

// ─── Layer 1: Packet Builders ───────────────────────────────────────────────
#include "packets/DeautherPackets.h"

// ─── Layer 2: Data Structures ───────────────────────────────────────────────
#include "data/DeautherAP.h"
#include "data/DeautherStation.h"
#include "data/DeautherAPList.h"
#include "data/DeautherStationList.h"
#include "data/DeautherSSIDList.h"
#include "data/DeautherTargetList.h"

// ─── Layer 3: Scanner ───────────────────────────────────────────────────────
#include "scan/DeautherScanner.h"

// ─── Layer 4: Attacks ───────────────────────────────────────────────────────
#include "attack/DeautherAttackConfig.h"
#include "attack/DeautherAttack.h"

// ─── Layer 5: Data Harvesting / Fake APs ────────────────────────────────────
#include "eviltwin/DeautherEvilTwin.h" // Clone + Password Stealer Combo
#include "rogueap/DeautherRogueAP.h"   // Custom AP + Data Harvesting Portal

// ─── Layer 6: Persistence & Settings ────────────────────────────────────────
#include "storage/DeautherStorage.h"   // SPIFFS data harvesting storage
#include "settings/DeautherSettings.h" // EEPROM config storage

// ─── Layer 7: Monitoring & Detection ────────────────────────────────────────
#include "detector/DeautherDetector.h"
