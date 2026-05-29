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


enum class CommandType : uint8_t{
    None = 0,
    SetColor = 1,
    SetPreset = 2,
    SetMode = 3,
    ClearLeds = 4,
    SetBrightness = 5
};

enum class PresetCategory : uint8_t {
    Idle = 1,
    Detected = 2,
    Intermediate = 3
};

enum class GateMode : uint8_t {
    Remote = 1,
    Offline = 2
};

enum class AckStatus : uint8_t {
    Ok = 1,
    Error = 2,
    UnknownCommand = 3,
    InvalidTarget = 4,
    InvalidValue = 5
};

#pragma pack(push, 1) //Removes padding to increse ESPNOW stability

struct GateTriggerMessage{
    uint8_t version;
    MessageType type;
    uint8_t gateId;
    uint16_t eventCounter;
};

struct GateStatusMessage{
    uint8_t version;
    MessageType type;
    uint8_t gateId;
    GateMode mode;
    uint16_t lastEventCounter;
};

struct BrokerCommandMessage {
    uint8_t version;
    MessageType type;
    uint8_t targetGateId;

    uint16_t commandId;

    CommandType commandType;

    uint8_t param1;
    uint8_t param2;
    uint8_t param3;
    uint8_t param4;
};

struct AckMessage {
    uint8_t version;
    MessageType type;
    uint8_t gateId;

    uint16_t refId;
    MessageType refType;
    AckStatus status;
};

# pragma pack(pop)
