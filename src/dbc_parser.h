#ifndef DBC_PARSER_H
#define DBC_PARSER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <dbcppp/Network.h>
#include <cstdint>

class DbcParser {
public:
    explicit DbcParser(const std::string& dbc_file);
    bool matchSender(uint32_t id, const std::string& sender) const;

private:
    std::unique_ptr<dbcppp::INetwork> network_;
    std::unordered_map<uint32_t, std::string> sender_map_;
};

#endif // DBC_PARSER_H
