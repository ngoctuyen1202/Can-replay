#ifndef CAN_SOCKET_H
#define CAN_SOCKET_H

#include "can_frame.h"
#include <string>

class CanSocket {
public:
    explicit CanSocket(const std::string& ifname);
    ~CanSocket();
    void send(const CanFrameData& f);

private:
    int sock_;
};

#endif // CAN_SOCKET_H
