#include "GateComms.h"

GateComms* GateComms::s_instance = nullptr;

bool GateComms::begin(const uint8_t brokerMac[6], uint8_t gateId) {
    m_gateId = gateId;

    memcpy(m_brokerMac, brokerMac, 6);

    s_instance = this;

    // WiFi must already be in STA mode.
    // OTAHandler already calls WiFi.mode(WIFI_STA).
    if (esp_now_init() != ESP_OK) {
        Serial.println("[ESP-NOW] Init failed");
        return false;
    }

    esp_now_register_recv_cb(GateComms::onDataReceived);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, m_brokerMac, 6);

    // 0 means: use current WiFi channel.
    // This is important for OTA + ESP-NOW coexistence.
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("[ESP-NOW] Add broker peer failed");
        return false;
    }

    Serial.println("[ESP-NOW] Gate communication ready");
    return true;
}

bool GateComms::sendTrigger() {
    GateTriggerMessage message = {};

    message.version = PROTOCOL_VERSION;
    message.type = MSG_TRIGGER;
    message.gateId = m_gateId;
    message.eventCounter = ++m_eventCounter;

    esp_err_t result = esp_now_send(
        m_brokerMac,
        reinterpret_cast<uint8_t*>(&message),
        sizeof(message)
    );

    return result == ESP_OK;
}

bool GateComms::hasCommand() const {
    return m_hasCommand;
}

BrokerCommandMessage GateComms::readCommand() {
    m_hasCommand = false;
    return m_lastCommand;
}

void GateComms::onDataReceived(const uint8_t* mac,
                               const uint8_t* data,
                               int len) {
    if (s_instance == nullptr) {
        return;
    }

    s_instance->handleReceivedData(data, len);
}

void GateComms::handleReceivedData(const uint8_t* data, int len) {
    if (len < 2) {
        return;
    }

    uint8_t version = data[0];
    uint8_t type = data[1];

    if (version != PROTOCOL_VERSION) {
        return;
    }

    if (type != MSG_COMMAND) {
        return;
    }

    if (len != sizeof(BrokerCommandMessage)) {
        return;
    }

    BrokerCommandMessage command = {};
    memcpy(&command, data, sizeof(command));

    bool isForThisGate =
        command.targetGateId == m_gateId ||
        command.targetGateId == BROADCAST_GATE_ID;

    if (!isForThisGate) {
        return;
    }

    m_lastCommand = command;
    m_hasCommand = true;
}