/* DeautherX-lib — https://github.com/BlackTechX011/DeautherX-lib
   MIT License */

#pragma once

#include <Arduino.h>

/**
 * DeautherDetector — Background deauth/disassoc attack monitor.
 * 
 * Ported from spacehuhn/DeauthDetector pattern.
 * Monitors for management frames (0xA0, 0xC0) and alerts when rate exceeds threshold.
 */
class DeautherDetector {
public:
    struct Config {
        uint16_t channels    = 0x3FFF;  ///< Channel bitmask (1-14)
        uint32_t pkt_rate    = 5;       ///< Min packets/s to flag an attack
        uint32_t pkt_time    = 1;       ///< Min detector cycles before attack started
        uint16_t ch_time     = 140;     ///< Time in ms spent on each channel
        int      led_pin     = 2;       ///< LED notification pin (-1 to disable)
        bool     led_invert  = true;    ///< Invert HIGH/LOW for LED
    };

    using AttackEventCb = void (*)();

    DeautherDetector();

    void begin(Config cfg = Config{});
    void stop();
    void update(); ///< MUST be called from loop()

    bool isRunning() const;
    bool isAttacked() const;
    uint32_t getPacketRate() const;
    uint32_t getAttackCounter() const;

    void onAttackStarted(AttackEventCb cb);
    void onAttackStopped(AttackEventCb cb);

private:
    Config _cfg;
    bool _running = false;
    bool _attacked = false;

    int _ch_index = 0;
    uint32_t _pkt_rate_counter = 0;
    uint32_t _pkt_rate_result = 0;
    uint32_t _attack_counter = 0;
    
    unsigned long _last_update = 0;
    unsigned long _last_hop = 0;

    AttackEventCb _start_cb = nullptr;
    AttackEventCb _stop_cb = nullptr;

    static DeautherDetector* _instance;
    static void _snifferCb(uint8_t* buf, uint16_t len);
    void _handlePacket(uint8_t* buf, uint16_t len);
    
    void _startAttack();
    void _stopAttack();
};
