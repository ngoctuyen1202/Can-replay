#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

#include "can_frame.h"
#include "dbc_parser.h"
#include "asc_parser.h"
#include "can_socket.h"

int main(int argc, char** argv)
{
    // Use default files and interface
    std::string dbc_file = "dbc/test.dbc";
    std::string log_file = "log_input/input.asc";
    std::string ifname = "vcan0";
    std::string exclude_sender = "ChassisBus";
    std::string program_log = "replay_output/replay_program.log";

    DbcParser dbc(dbc_file);
    AscParser asc;
    CanSocket can(ifname);

    std::ifstream fin(log_file);
    if (!fin) {
        std::cerr << "Failed open log file: " << log_file << "\n";
        return 2;
    }

    std::ofstream flog(program_log);
    if (!flog) {
        std::cerr << "Failed open program log: " << program_log << "\n";
        return 2;
    }

    std::string line;
    double last_ts = -1.0;

    std::cout << "=== CAN Replay Start ===\n";
    flog << "=== CAN Replay Start ===\n";

    while (std::getline(fin, line)) {
        CanFrameData f{};
        if (!asc.parseLine(line, f)) {
            std::cout << "SKIP LINE: " << line << "\n";
            flog << "SKIP LINE: " << line << "\n";
            continue;
        }

        // Apply exclude filters by sender name in DBC
        if (!exclude_sender.empty()) {
            if (dbc.matchSender(f.can_id, exclude_sender)) {
                flog << "EXCLUDED by sender " << exclude_sender << " id=" << f.can_id << "(dec)"<<"\n";
                continue;
            }
        }


        if (last_ts >= 0)
            std::this_thread::sleep_for(
                std::chrono::duration<double>(f.timestamp - last_ts));
        last_ts = f.timestamp;

        std::cout << "SEND RAW ID=" << f.can_id << " DLC=" << (int)f.dlc << "\n";
        flog << "SEND RAW ID=" << f.can_id << " DLC=" << (int)f.dlc << "\n";

        can.send(f);

        std::cout << "Sent ID=0x" << std::hex << f.can_id << std::dec << "\n";
        flog << "Sent ID=0x" << std::hex << f.can_id << std::dec << "\n";
    }

    std::cout << "=== CAN Replay End ===\n";
    flog << "=== CAN Replay End ===\n";
    return 0;
}
