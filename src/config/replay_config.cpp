#include "config.h"
#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

bool loadConfig(const std::string& path, ReplayConfig& cfg)
{
    std::ifstream fin(path);
    if (!fin) {
        std::cerr << "Cannot open config file: " << path << "\n";
        return false;
    }

    json j;
    fin >> j;

    cfg.dbc_file       = j.at("dbc_file").get<std::string>();
    cfg.log_file       = j.at("log_file").get<std::string>();
    cfg.ifname         = j.at("ifname").get<std::string>();
    cfg.exclude_sender = j.at("exclude_sender").get<std::string>();
    cfg.program_log    = j.at("program_log").get<std::string>();

    return true;
}
