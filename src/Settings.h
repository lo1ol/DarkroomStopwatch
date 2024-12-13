#pragma once

#include <stdint.h>

enum SettingId : uint8_t { PreNotifyTime, LongNotify, EachMinuteNotify, last_ };

struct Settings {
    uint8_t preNotifyTime = 0;
    bool longNotify = false;
    bool eachMinuteNotify = false;
};

constexpr Settings gDefaultSettings;
