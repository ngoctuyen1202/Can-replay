# CAN Replay

Purpose
- Read a DBC file (`dbc/vehicle.dbc`) and an ASC CAN log (`log_input/input.asc`), filter messages by sender, and replay them on a SocketCAN interface (default `vcan0`).

Build

make

Run

./can_replay

Capture CAN output (create new terminal and run)

candump vcan0 -L > replay_output/replay_can.log


Notes
- Defaults: `dbc/vehicle.dbc`, `log_input/input.asc`, interface `vcan0`.
- The program currently excludes sender `EngineECU` by default. Edit `src/main.cpp` to change filters.
