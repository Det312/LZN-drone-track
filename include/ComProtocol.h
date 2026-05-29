# pragma once
# include <Arduino.h>

constexpr uint8_t PROTOCOL_VERSION = 1;
constexpr uint8_t BROADCAST_GATE_ID = 255;

enum class MessageType : uint8_t {
    Trigger = 1,
    Status = 2,
    Command = 3,
    Ack = 4,
    Ping = 5,
    Pong = 6
};

