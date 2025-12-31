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

    // enable CAN FD for socket
    int enable_fd = 1;
    setsockopt(sock_, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &enable_fd, sizeof(enable_fd));

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
    if (f.is_fd || f.data_len > CAN_CLASSIC_PAYLOAD_MAX) {
        struct canfd_frame frame{};
        frame.can_id = f.can_id;
        frame.len = f.data_len;
        memcpy(frame.data, f.data, frame.len);
        ::write(sock_, &frame, sizeof(struct canfd_frame));
    } else {
        struct can_frame frame{};
        frame.can_id = f.can_id;
        frame.can_dlc = static_cast<__u8>(f.data_len);
        memcpy(frame.data, f.data, frame.can_dlc);
        ::write(sock_, &frame, sizeof(struct can_frame));
    }
}
