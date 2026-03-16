#include <iostream>
#include <chrono>
#include "chess_parser.hpp"
#include "database.hpp"

int main() {
    std::string line;
    int games_found = 0;

    PgnParser parser;
    Database db("chess_data.db");

    std::cout << "Waiting for data" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    while (std::getline(std::cin, line)) {
        // parse line to hashes
        std::vector<uint64_t>& hashes = parser.parse_line_to_hashes(line);
        if (hashes.empty()) continue;
        
        // send hashes to db
        db.insert_hashes(hashes);

        games_found++;

        if (games_found % 50000 == 0) {
            std::cout << "Käsitelty " << games_found << " peliä..." << std::endl;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    std::cout << "Processed " << games_found << " games in " << time << " ms" << std::endl;
    return 0;
}