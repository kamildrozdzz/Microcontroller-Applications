#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "amcom.h"

/// Start of packet character
const uint8_t  AMCOM_SOP         = 0xA1;
const uint16_t AMCOM_INITIAL_CRC = 0xFFFF;

static uint16_t AMCOM_UpdateCRC(uint8_t byte, uint16_t crc)
{
    byte ^= (uint8_t)(crc & 0x00ff);
    byte ^= (uint8_t)(byte << 4);
    return ((((uint16_t)byte << 8) | (uint8_t)(crc >> 8))
            ^ (uint8_t)(byte >> 4)
            ^ ((uint16_t)byte << 3));
}

void AMCOM_InitReceiver(AMCOM_Receiver* receiver, AMCOM_PacketHandler packetHandlerCallback, void* userContext) {
    assert(receiver != NULL);
    receiver->receivedPacketState = AMCOM_PACKET_STATE_EMPTY;
    receiver->payloadCounter      = 0;
    receiver->packetHandler       = packetHandlerCallback;
    receiver->userContext         = userContext;
    receiver->crc                 = AMCOM_INITIAL_CRC;
}

size_t AMCOM_Serialize(uint8_t packetType, const void* payload, size_t payloadSize, uint8_t* destinationBuffer) {
    if (!destinationBuffer || payloadSize > AMCOM_MAX_PAYLOAD_SIZE) {
        return 0;
    }

    uint8_t* p = destinationBuffer;
    *p++ = AMCOM_SOP;
    *p++ = packetType;
    *p++ = (uint8_t)payloadSize;

    uint16_t crc = AMCOM_INITIAL_CRC;
    crc = AMCOM_UpdateCRC(packetType, crc);
    crc = AMCOM_UpdateCRC((uint8_t)payloadSize, crc);
    if (payload && payloadSize) {
        const uint8_t* data = (const uint8_t*)payload;
        for (size_t i = 0; i < payloadSize; ++i) {
            crc = AMCOM_UpdateCRC(data[i], crc);
        }
    }

    *p++ = (uint8_t)(crc & 0xFF);
    *p++ = (uint8_t)(crc >> 8);

    if (payload && payloadSize) {
        memcpy(p, payload, payloadSize);
        p += payloadSize;
    }

    return (size_t)(p - destinationBuffer);
}

void AMCOM_Deserialize(AMCOM_Receiver* receiver, const void* data, size_t dataSize) {
    assert(receiver && data);
    const uint8_t* bytes = (const uint8_t*)data;

    for (size_t i = 0; i < dataSize; ++i) {
        uint8_t b = bytes[i];
        switch (receiver->receivedPacketState) {

        case AMCOM_PACKET_STATE_EMPTY:
            if (b == AMCOM_SOP) {
                receiver->receivedPacket.header.sop = b;
                receiver->crc = AMCOM_INITIAL_CRC;
                receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_SOP;
                receiver->payloadCounter = 0;
            }
            break;

        case AMCOM_PACKET_STATE_GOT_SOP:
            receiver->receivedPacket.header.type = b;
            receiver->crc = AMCOM_UpdateCRC(b, receiver->crc);
            receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_TYPE;
            break;

        case AMCOM_PACKET_STATE_GOT_TYPE:
            receiver->receivedPacket.header.length = b;
            receiver->crc = AMCOM_UpdateCRC(b, receiver->crc);
            if (b > AMCOM_MAX_PAYLOAD_SIZE) {
                receiver->receivedPacketState = AMCOM_PACKET_STATE_EMPTY;
            } else {
                receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_LENGTH;
            }
            break;

        case AMCOM_PACKET_STATE_GOT_LENGTH:
            receiver->receivedPacket.header.crc = b;
            receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_CRC_LO;
            break;

        case AMCOM_PACKET_STATE_GOT_CRC_LO:
            receiver->receivedPacket.header.crc |= (uint16_t)b << 8;
            receiver->receivedPacketState =
                (receiver->receivedPacket.header.length > 0)
                  ? AMCOM_PACKET_STATE_GETTING_PAYLOAD
                  : AMCOM_PACKET_STATE_GOT_WHOLE_PACKET;
            break;

        case AMCOM_PACKET_STATE_GETTING_PAYLOAD:
            receiver->receivedPacket.payload[receiver->payloadCounter++] = b;
            receiver->crc = AMCOM_UpdateCRC(b, receiver->crc);
            if (receiver->payloadCounter >= receiver->receivedPacket.header.length) {
                receiver->receivedPacketState = AMCOM_PACKET_STATE_GOT_WHOLE_PACKET;
            }
            break;

        default:
            break;
        }

        if (receiver->receivedPacketState == AMCOM_PACKET_STATE_GOT_WHOLE_PACKET) {
            if (receiver->crc == receiver->receivedPacket.header.crc) {
                if (receiver->packetHandler) {
                    receiver->packetHandler(&receiver->receivedPacket, receiver->userContext);
                }
            }
            receiver->receivedPacketState = AMCOM_PACKET_STATE_EMPTY;
            receiver->payloadCounter = 0;
        }
    }
}
