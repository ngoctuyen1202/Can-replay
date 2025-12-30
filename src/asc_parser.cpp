#include "asc_parser.h"
#include <sstream>
#include <iostream>

bool AscParser::parseLine(const std::string& line, CanFrameData& f)
{
    std::stringstream ss(line);

    std::string channel;
    std::string direction;
    std::string data_flag;
    std::string can_id_text;
    std::string byte_text;

    if (!(ss >> f.timestamp))
        return false;
    if (!(ss >> channel))
        return false;
    if (!(ss >> can_id_text))
        return false;
    if (!(ss >> direction))
        return false;
    if (!(ss >> data_flag))
        return false;

    unsigned int dlc_tmp;
    if (!(ss >> dlc_tmp))
        return false;
    if (dlc_tmp > 8)
        return false;
    f.dlc = static_cast<uint8_t>(dlc_tmp);

    try {
        if (can_id_text.find("0x") == 0)
            f.can_id = std::stoul(can_id_text, nullptr, 16);
        else
            f.can_id = std::stoul(can_id_text, nullptr, 10);
    }
    catch (...) {
        return false;
    }

    for (int i = 0; i < f.dlc; ++i) {
        if (!(ss >> byte_text))
            return false;
        try {
            f.data[i] = static_cast<uint8_t>(std::stoul(byte_text, nullptr, 16));
        }
        catch (...) {
            return false;
        }
    }

    std::cout << "PARSED ts=" << f.timestamp
              << " id=0x" << std::hex << f.can_id << std::dec
              << " dlc=" << static_cast<int>(f.dlc) << "\n";

    return true;
}
