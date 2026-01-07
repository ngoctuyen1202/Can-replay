#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

#include "can_frame.h"
#include "dbc_parser.h"
#include "asc_parser.h"
#include "can_socket.h"
#include "config.h"

int main(int argc, char** argv)
{
    // Init config
    ReplayConfig cfg;

    if (!loadConfig("config/cfg_replay.json", cfg)) {
        return 1;
    }

    DbcParser dbc(cfg.dbc_file);
    AscParser asc;
    CanSocket can(cfg.ifname);

    std::ifstream fin(cfg.log_file);
    std::ofstream flog(cfg.program_log);

    std::string exclude_sender = cfg.exclude_sender;
    if (!fin) {
        std::cerr << "Failed open log file: " << cfg.log_file << "\n";
        return 2;
    }

    if (!flog) {
        std::cerr << "Failed open program log: " << cfg.program_log << "\n";
        return 2;
    }

    std::vector<ReplayFrame> replay_frames;

    std::string line;
    bool first_frame = true;
    double start_log_ts = 0.0;

    while (std::getline(fin, line)) {
        CanFrameData f{};
        if (!asc.parseLine(line, f)) {
            flog << "SKIP LINE: " << line << "\n";
            continue;
        }

        if (first_frame) {
            start_log_ts = f.timestamp;
            first_frame = false;
        }

        replay_frames.push_back({f, {}});
    }
    auto start_wall = std::chrono::steady_clock::now();

    for (auto& rf : replay_frames) {
        double log_delta = rf.frame.timestamp - start_log_ts;

        rf.send_time = start_wall
                    + std::chrono::duration_cast<std::chrono::steady_clock::duration>(
                    std::chrono::duration<double>(log_delta));
    }

    std::cout << "=== CAN Replay Start ===\n";
    flog << "=== CAN Replay Start ===\n";

    for (const auto& rf : replay_frames) {

        std::this_thread::sleep_until(rf.send_time);

        // Apply exclude filters
        if (!exclude_sender.empty()) {
            if (dbc.matchSender(rf.frame.can_id, exclude_sender)) {
                flog << "EXCLUDED by sender " << exclude_sender
                    << " id=" << rf.frame.can_id << "\n";
                continue;
            }
        }
        // Send CAN frame
        can.send(rf.frame);

        std::cout << "SEND RAW ID=" << rf.frame.can_id
                << " DLC=" << (int)rf.frame.dlc << "\n";
        flog << "SEND RAW ID=" << rf.frame.can_id
            << " DLC=" << (int)rf.frame.dlc << "\n";
    }

    std::cout << "=== CAN Replay End ===\n";
    flog << "=== CAN Replay End ===\n";
    return 0;
}
