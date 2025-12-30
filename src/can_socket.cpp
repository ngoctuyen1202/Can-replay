#include "can_socket.h"
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <cstring>
#include <unistd.h>

CanSocket::CanSocket(const std::string& ifname)
{
    sock_ = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    struct ifreq ifr{};
    strncpy(ifr.ifr_name, ifname.c_str(), IFNAMSIZ - 1);
    ioctl(sock_, SIOCGIFINDEX, &ifr);

    struct sockaddr_can addr{};
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    bind(sock_, (struct sockaddr*)&addr, sizeof(addr));
}

CanSocket::~CanSocket()
{
    close(sock_);
}

void CanSocket::send(const CanFrameData& f)
{
    struct can_frame frame{};
    frame.can_id = f.can_id;
    frame.can_dlc = f.dlc;
    memcpy(frame.data, f.data, f.dlc);
    ::write(sock_, &frame, sizeof(frame));
}
