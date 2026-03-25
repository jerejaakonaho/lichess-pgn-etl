#pragma once
#include <vector>
#include <string>
#include <thread>
#include <unordered_map>
#include "safe_queue.hpp"
#include "database.hpp"
#include "chess_parser.hpp"
#include <chrono>

class App {
FRIEND_TEST(AppTest, TestWorkerTask);
private:
    SafeQueue<std::vector<std::string>> task_queue{50};
    SafeQueue<std::vector<std::pair<std::string, int>>> sort_queue{50};
    SafeQueue<std::vector<std::pair<std::string, int>>> db_queue{10};

    Database db;
    // How many threads to allocate to the program.
    int number_of_threads = 5;

    // Does the data streaming to worker_task.
    void reader_task();

    // Sanitizes the data for db_task.
    void worker_task();

    // Transfers the data to a SQL database SQLite for now.
    void db_task();

public:
    App(const std::string& db_name, int threads = 4)
        :
        db(db_name), number_of_threads(threads) {}

    void run();
};