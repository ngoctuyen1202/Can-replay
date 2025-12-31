#ifndef CAN_FRAME_H
#define CAN_FRAME_H

#include <cstdint>

// define constants
#define CAN_FD_PAYLOAD_MAX           64
#define CAN_FD_DLC_MAX               15
#define CAN_CLASSIC_PAYLOAD_MAX      8


struct CanFrameData {
    double   timestamp;
    uint32_t can_id;
    uint8_t  dlc;       // raw DLC value (0-15)
    uint8_t  data_len;  // actual data length in bytes
    bool     is_fd;     // true if CAN FD frame
    uint8_t  data[CAN_FD_PAYLOAD_MAX]; // max 64 bytes for CAN FD
};

#endif // CAN_FRAME_H
