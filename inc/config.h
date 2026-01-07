#ifndef CONFIG_H
#define CONFIG_H
#include <string>

struct ReplayConfig  {
    std::string dbc_file;
    std::string log_file;
    std::string ifname;
    std::string exclude_sender;
    std::string program_log;
};
bool loadConfig(const std::string& path, ReplayConfig& cfg);
#endif // CONFIG_H