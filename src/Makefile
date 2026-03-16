CXX = g++

CXXFLAGS = -O3 -march=native -Wall -std=c++17

LDFLAGS = -lsqlite3

TARGET = parser

SRCS = main.cpp chess_parser.cpp database.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET) : $(OBJS)
		$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.cpp
		$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
		rm -f $(OBJS) $(TARGET)