#include <iostream>
#include <chrono>
#include "app.hpp"

int main() {
    try {
        int number_of_threads = 4;

        App app("chess_data.db", number_of_threads);

        auto start_time = std::chrono::high_resolution_clock::now();
        app.run();

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_time - start_time;
        std::cout << "Time: " << duration.count() << " seconds" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Program executed succesfully. Closing..." << std::endl;
    return 0;
}