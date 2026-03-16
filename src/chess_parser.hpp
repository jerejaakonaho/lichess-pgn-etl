#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "chess/chess.hpp"

class PgnParser {
public:
    PgnParser();
    std::vector<uint64_t>& parse_line_to_hashes(const std::string& line);

private:
    std::string cleaned_buffer;
    std::vector<std::string_view> moves_view_buffer;
    std::vector<uint64_t> hashes_buffer;

    chess::Board board;

    bool is_valid_move_line(const std::string& line);
    void clean_move_line(const std::string& source);
    void extract_moves();
};