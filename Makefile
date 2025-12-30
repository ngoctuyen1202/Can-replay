CPP = g++
CXXFLAGS = -std=c++17 -I./src
LDFLAGS = -ldbcppp

SRCS = src/dbc_parser.cpp src/asc_parser.cpp src/can_socket.cpp src/main.cpp

all: can_replay

can_replay: $(SRCS)
	$(CPP) $(CXXFLAGS) $(SRCS) -o can_replay $(LDFLAGS)

clean:
	rm -f can_replay $(OBJS)
