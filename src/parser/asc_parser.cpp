#include "asc_parser.h"
#include <sstream>
#include <iostream>


bool AscParser::parseLine(const std::string& line, CanFrameData& f)
{
    std::stringstream ss(line);

    // 1. Read timestamp
    if (!(ss >> f.timestamp)) return false;

    // 2. Read channel / type. Type is CAN FD, channel is Classic.
    std::string channel;
    if (!(ss >> channel)) return false;

    if (channel == "CANFD") {
        /* ==== CAN FD ==== */
        f.is_fd = true;

        std::string iface, dir, can_id_text;
        if (!(ss >> iface >> dir >> can_id_text)) return false;

        // Skip signal name (may contain spaces)
        std::string signal_name;
        ss >> signal_name;

        unsigned int dlc_tmp = 0, reserved1 = 0, payload_len = 0;
        if (!(ss >> dlc_tmp >> reserved1 >> payload_len)) return false;

        // DLC + data length checks
        f.dlc = static_cast<uint8_t>(dlc_tmp);
        f.data_len = static_cast<uint8_t>(payload_len);

        if (f.dlc > CAN_FD_DLC_MAX) return false;

        if (f.data_len > CAN_FD_PAYLOAD_MAX) return false;
        // Read CAN ID
        try {
            f.can_id = std::stoul(can_id_text, nullptr, 16);
        } catch (...) { return false; }

        // Read payload_len bytes of data
        for (unsigned int i = 0; i < payload_len; ++i) {
            std::string byte_text;
            if (!(ss >> byte_text)) return false;
            try {
                f.data[i] = static_cast<uint8_t>(std::stoul(byte_text, nullptr, 16));
            } catch (...) { return false; }
        }

    } else if (channel == "1" || channel == "2") {
        /* ==== CAN Classic ==== */
        f.is_fd = false;

        std::string can_id_text, dir, data_flag;
        if (!(ss >> can_id_text >> dir >> data_flag)) return false;

        // DLC + data length checks
        unsigned int dlc_tmp = 0;
        if (!(ss >> dlc_tmp)) return false;
        if (dlc_tmp > CAN_CLASSIC_PAYLOAD_MAX) return false;

        f.dlc = static_cast<uint8_t>(dlc_tmp);
        f.data_len = f.dlc;

        // Read CAN ID
        try {
            f.can_id = std::stoul(can_id_text, nullptr, 16);
        } catch (...) { return false; }

        // Read dlc bytes of data
        for (unsigned int i = 0; i < f.data_len; ++i) {
            std::string byte_text;
            if (!(ss >> byte_text)) return false;
            try {
                f.data[i] = static_cast<uint8_t>(std::stoul(byte_text, nullptr, 16));
            } catch (...) { return false; }
        }

    } else {
        // Unknown channel/type
        return false;
    }

    // Add log debug 
    std::cout << "PARSED " << (f.is_fd ? "FD" : "Classic")
              << " ts=" << f.timestamp
              << " id=0x" << std::hex << f.can_id << std::dec
              << " len=" << static_cast<int>(f.data_len)
              << " dlc=" << static_cast<int>(f.dlc)
              << "\n";

    return true;
}


