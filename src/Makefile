# Compiler Settings
CXX = g++
CXXFLAGS = -O3 -march=native -Wall -Werror -std=c++17 -I.. -pthread
LDFLAGS = -lsqlite3 -pthread

# GTest linker flags
GTEST_LDFLAGS = -lgtest -lgtest_main -pthread

# Targets
TARGET = parser
TEST_TARGET = run_tests

# Core application files (everything except main.cpp)
APP_SRCS = chess_parser.cpp database.cpp app.cpp
APP_OBJS = $(APP_SRCS:.cpp=.o)

# Main entry point for the real app
MAIN_SRC = main.cpp
MAIN_OBJ = $(MAIN_SRC:.cpp=.o)

# Test files
TEST_SRC = test.cpp
TEST_OBJ = $(TEST_SRC:.cpp=.o)

# Rules
all: $(TARGET)

# Main
$(TARGET) : $(APP_OBJS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(APP_OBJS) $(MAIN_OBJ) $(LDFLAGS)

# Build and run tests
test: $(TEST_TARGET)
		./$(TEST_TARGET)

$(TEST_TARGET): $(APP_OBJS) $(TEST_OBJ)
		$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(APP_OBJS) $(TEST_OBJ) $(LDFLAGS) $(GTEST_LDFLAGS)

# Compile cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(APP_OBJS) $(MAIN_OBJ) $(TEST_OBJ) $(TARGET) $(TEST_TARGET)