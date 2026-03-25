#include <iostream>
#include "app.hpp"

void App::reader_task() {
    std::vector<std::string> batch;
    batch.reserve(5000);

    std::string line;
    int games_read = 0;
    auto start_time = std::chrono::steady_clock::now();
    // zstd -> std::cin as long as there is new data
    while (std::getline(std::cin, line)) {
        batch.push_back(line);
        games_read++;

        if (batch.size() >= 5000) {
            // send batches of 5000 lines to task_queue
            task_queue.push(std::move(batch));
            batch = std::vector<std::string>();
            batch.reserve(5000);

            if (games_read % 100000 == 0) {
                auto push_time = std::chrono::steady_clock::now();
                auto duration = push_time - start_time;
                auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count(); 
                std::cout << "-> Reader: Pushed " << games_read << " lines to memory in " << seconds << " seconds." <<std::endl;
            }
        }
    }
    // If data ran out but there are games in the buffer send them for workers
    if (!batch.empty()) {
        task_queue.push(std::move(batch));
    }
    task_queue.set_done();
}

void App::worker_task() {
    PgnParser local_parser;

    std::vector<std::string> games_batch;

    // get a batch from task_queue, sleep if queue is empty.
    // returns false when task_queue.set_done() has been called
    while (task_queue.pop(games_batch)) {
        std::unordered_map<std::string, int> fen_counts;

        for (const std::string& game_line : games_batch) {
            std::vector<std::string>& fens = local_parser.parse_line_to_fens(game_line);

            for (const auto& fen : fens) {
                fen_counts[fen]++;
            }
        }
        // Make the unordered map a vector, caused problems previously
        // Multithreading weirdness, memory didn't free properly on worker threads
        // because the data was moved to db_task and it freed the memory.
        std::vector<std::pair<std::string, int>> flat_counts(fen_counts.begin(), fen_counts.end());

        db_queue.push(std::move(flat_counts));
    }
}

void App::db_task() {
    std::vector<std::pair<std::string, int>> fen_batch;
    uint64_t fens_saved = 0;
    uint64_t current_buffer = 0;
    uint64_t milestone = 1000000;

    db.begin_transaction();

    while (db_queue.pop(fen_batch)) {
        db.insert_fens(fen_batch);
        fens_saved += fen_batch.size();
        current_buffer += fen_batch.size();
        if (current_buffer > 50000) {
            db.commit_transaction();
            db.begin_transaction();
            current_buffer = 0;
        }
        if (fens_saved >= milestone) {
            milestone += 1000000;
            std::cout << "Saved " << fens_saved << " fens to the database" << std::endl;
        }
    }
    db.commit_transaction();
}

void App::run() {
    std::cout << "Starting with " << number_of_threads << "threads" << std::endl;

    std::thread db_thread(&App::db_task, this);

    std::vector<std::thread> workers;
    for (int i = 0; i < number_of_threads; ++i) {
        workers.emplace_back(&App::worker_task, this);
    }

    reader_task();

    for (auto& worker : workers) {
        worker.join();
    }

    db_queue.set_done();

    db_thread.join();
}