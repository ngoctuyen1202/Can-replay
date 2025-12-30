#ifndef CAN_FRAME_H
#define CAN_FRAME_H

#include <cstdint>

struct CanFrameData {
    double   timestamp;
    uint32_t can_id;
    uint8_t  dlc;
    uint8_t  data[8];
};

#endif // CAN_FRAME_H
