#include "chess_parser.hpp"

PgnParser::PgnParser() {
    cleaned_buffer.reserve(4096);
    moves_view_buffer.reserve(256);
    fen_buffer.reserve(256);
}

bool PgnParser::is_valid_move_line(const std::string& line) {
    if (line.empty() || line[0] == '\r') {
        return false;
    }

    if (line[0] == '[') {
        return false;
    }
    return true;
}

void PgnParser::clean_move_line(const std::string& source) {
    cleaned_buffer.clear();
    bool inside_brackets = false;

    for (char c : source) {
        if (c == '{') { inside_brackets = true; continue; }
        if (c == '}') { inside_brackets = false; continue; }
        if (!inside_brackets) { cleaned_buffer += c; }
    }
}

void PgnParser::extract_moves() {
    moves_view_buffer.clear();
    std::string_view source = cleaned_buffer;
    size_t start = 0;
    size_t end = source.find(' ');

    while (end != std::string_view::npos) {
        std::string_view word = source.substr(start, end - start);

        if (word.empty()) {
            start = end + 1;
            end = source.find(' ', start);
            continue;
        }

        if (word.find('.') == std::string_view::npos &&
            word != "1-0" &&
            word != "0-1" && 
            word != "1/2-1/2" &&
            word != "*") {
                moves_view_buffer.push_back(word);
            }
        start = end + 1;
        end = source.find(' ', start);
    }

    std::string_view last_word = source.substr(start);
    if (!last_word.empty() && 
        last_word.find('.') == std::string_view::npos &&
        last_word != "1-0" && 
        last_word != "0-1" && 
        last_word != "1/2-1/2" && 
        last_word != "*") {
        moves_view_buffer.push_back(last_word);
    }
}

void PgnParser::get_stripped_fen(std::string& full_fen) {
    int spaces = 0;
    for (size_t i = 0; i < full_fen.length(); ++i) {
        if (full_fen[i] == ' ') {
            spaces++;
            if (spaces == 4) {
                full_fen.resize(i);
                return;
            }
        }
    }
    return;
}

std::vector<std::string>& PgnParser::parse_line_to_fens(const std::string& line) {
    fen_buffer.clear();
    if (!is_valid_move_line(line)) return fen_buffer;
    
    clean_move_line(line);
    extract_moves();

    if (moves_view_buffer.empty()) return fen_buffer;

    board.setFen(chess::constants::STARTPOS);

    for (auto move_view : moves_view_buffer) {
        try {
            auto move = chess::uci::parseSan(board, move_view);
            board.makeMove(move);

            std::string full_fen = board.getFen();

            get_stripped_fen(full_fen);
            
            fen_buffer.push_back(std::move(full_fen));

        } catch (...) {
            break;
        }
    }
    return fen_buffer;
}