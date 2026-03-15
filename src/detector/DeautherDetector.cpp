/* DeautherX-lib — https://github.com/BlackTechX011/DeautherX-lib
   MIT License */

#include "DeautherDetector.h"
#include "../radio/DeautherRadio.h"
#include "../utils/DeautherChannel.h"

DeautherDetector* DeautherDetector::_instance = nullptr;

DeautherDetector::DeautherDetector() {
    _instance = this;
}

void DeautherDetector::begin(Config cfg) {
    _cfg = cfg;
    _running = true;
    _attacked = false;
    _attack_counter = 0;
    _pkt_rate_counter = 0;
    _pkt_rate_result = 0;

    if (_cfg.led_pin >= 0) {
        pinMode(_cfg.led_pin, OUTPUT);
        digitalWrite(_cfg.led_pin, _cfg.led_invert ? HIGH : LOW);
    }

    _last_update = millis();
    _last_hop = millis();
    _ch_index = 0;

    DRadio::enablePromiscuous(_snifferCb);
    DChannel::set(DChannel::next(_cfg.channels));
}

void DeautherDetector::stop() {
    _running = false;
    DRadio::disablePromiscuous();
    if (_cfg.led_pin >= 0) {
        digitalWrite(_cfg.led_pin, _cfg.led_invert ? HIGH : LOW);
    }
}

void DeautherDetector::update() {
    if (!_running) return;

    unsigned long now = millis();
    uint8_t ch_count = DChannel::count(_cfg.channels);
    unsigned long update_interval = (unsigned long)ch_count * _cfg.ch_time;

    // Update stats per cycle
    if (now - _last_update >= update_interval) {
        _last_update = now;
        _pkt_rate_result = _pkt_rate_counter;

        if (_pkt_rate_counter >= _cfg.pkt_rate) {
            _attack_counter++;
        } else {
            if (_attack_counter >= _cfg.pkt_time) {
                _stopAttack();
            }
            _attack_counter = 0;
        }

        if (_attack_counter == _cfg.pkt_time) {
            _startAttack();
        }

        _pkt_rate_counter = 0;
    }

    // Channel hopping
    if (ch_count > 1 && now - _last_hop >= _cfg.ch_time) {
        _last_hop = now;
        DChannel::set(DChannel::next(_cfg.channels));
    }
}

bool DeautherDetector::isRunning() const {
    return _running;
}

bool DeautherDetector::isAttacked() const {
    return _attacked;
}

uint32_t DeautherDetector::getPacketRate() const {
    return _pkt_rate_result;
}

uint32_t DeautherDetector::getAttackCounter() const {
    return _attack_counter;
}

void DeautherDetector::onAttackStarted(AttackEventCb cb) {
    _start_cb = cb;
}

void DeautherDetector::onAttackStopped(AttackEventCb cb) {
    _stop_cb = cb;
}

void DeautherDetector::_snifferCb(uint8_t* buf, uint16_t len) {
    if (_instance) _instance->_handlePacket(buf, len);
}

void DeautherDetector::_handlePacket(uint8_t* buf, uint16_t len) {
    if (!buf || len < 28) return;

    // Frame control field: buf[0] and buf[1]
    // buf[0] contains Type and Subtype
    // Management frames: Type 00
    // Deauth: Subtype 1100 (0xC0)
    // Disassoc: Subtype 1010 (0xA0)
    
    uint8_t type = buf[0];
    if (type == 0xC0 || type == 0xA0) {
        _pkt_rate_counter++;
    }
}

void DeautherDetector::_startAttack() {
    _attacked = true;
    if (_cfg.led_pin >= 0) {
        digitalWrite(_cfg.led_pin, _cfg.led_invert ? LOW : HIGH);
    }
    if (_start_cb) _start_cb();
}

void DeautherDetector::_stopAttack() {
    _attacked = false;
    if (_cfg.led_pin >= 0) {
        digitalWrite(_cfg.led_pin, _cfg.led_invert ? HIGH : LOW);
    }
    if (_stop_cb) _stop_cb();
}
