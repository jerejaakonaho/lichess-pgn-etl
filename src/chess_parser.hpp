#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../chess/chess.hpp"
#include <gtest/gtest_prod.h>

class PgnParser {
public:
    PgnParser();
    std::vector<std::string>& parse_line_to_fens(const std::string& line);

private:
    FRIEND_TEST(ChessParserTest, TestIsValidMoveLine);
    FRIEND_TEST(ChessParserTest, TestCleanMoveLine);

    std::string cleaned_buffer;
    std::vector<std::string_view> moves_view_buffer;
    std::vector<std::string> fen_buffer;

    chess::Board board;
    /**
     * Checks, if the given line contains chess moves
     * @param line A single line from a PGN file.
     * @return true
     */
    bool is_valid_move_line(const std::string& line);

    /**
     * Cleans the move line from unnecessary data and pushes the result
     * 
     * @param source a clean string of moves provided by is_valid_move_line
     * @return void
     */
    void clean_move_line(const std::string& source);

    /**
     * Extracts the moves
     * 
     * 
     */
    void extract_moves();

    std::string get_stripped_fen(const std::string& full_fen);
};