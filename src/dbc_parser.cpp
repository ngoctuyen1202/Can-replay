#include "dbc_parser.h"
#include <dbcppp/Network.h>
#include <fstream>
#include <iostream>

DbcParser::DbcParser(const std::string& dbc_file)
{
    std::ifstream dbc(dbc_file);
    if (!dbc) {
        std::cerr << "Warning: failed to open DBC file: " << dbc_file << "\n";
        return;
    }

    network_ = dbcppp::INetwork::LoadDBCFromIs(dbc);
    if (!network_) {
        std::cerr << "Warning: failed to parse DBC file: " << dbc_file << "\n";
        return;
    }
    std::cout << "[INFO] DBC parsed successfully" << std::endl;

    for (const auto& msg : network_->Messages()) {
        std::cout << "ID=0x" << std::hex << msg.Id()
                  << " TX=" << msg.Transmitter() << std::dec << "\n";
        sender_map_[msg.Id()] = msg.Transmitter();
    }
}

bool DbcParser::matchSender(uint32_t id, const std::string& sender) const
{
    auto it = sender_map_.find(id);
    
    if (it == sender_map_.end()) {
        return false;
    }

    return it->second == sender;
}
