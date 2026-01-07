CPP = g++
CXXFLAGS := -std=c++17 \
            -I./inc \
            -I./third_party/json/include
LDFLAGS = -ldbcppp

SRCS := \
    src/main.cpp \
    src/can_socket/can_socket.cpp \
    src/parser/asc_parser.cpp \
    src/parser/dbc_parser.cpp \
    src/config/replay_config.cpp

all: can_replay

can_replay: $(SRCS)
	$(CPP) $(CXXFLAGS) $(SRCS) -o can_replay $(LDFLAGS)

clean:
	rm -f can_replay $(OBJS)