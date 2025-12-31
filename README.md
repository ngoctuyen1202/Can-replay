# CAN Replay

Purpose
- Read a DBC file (`dbc/test.dbc`) (link: https://github.com/mireo/can-utils/blob/main/example/example.dbc) and an ASC CAN log (`log_input/input.asc`), filter messages by sender, and replay them on a SocketCAN interface (default `vcan0`).

Build

make all

Run

./can_replay

Clean
 
make clean

Capture CAN output (create new terminal and run)

candump vcan0 -L > replay_output/replay_can.log


Notes
- Defaults: `dbc/test.dbc`, `log_input/input.asc`, interface `vcan0`.
- The program currently excludes sender `ChassisBus` by default. Edit `src/main.cpp` to change filters.
