#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "chess/chess.hpp"

struct ChessGame {
    std::string white_player;
    std::string black_player;
    std::string result;
    std::string moves;

    void clear() {
        white_player = "";
        black_player = "";
        result = "";
        moves = "";
    }
};

int main() {
    std::string file_path = "lichess_db_standard_rated_2025-10.pgn.zst";

    std::ifstream pgn_file(file_path);

    if (!pgn_file.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return 1;
    }


};