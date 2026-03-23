#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "app.hpp"
#include "chess_parser.hpp"

TEST(ChessParserTest, TestIsValidMoveLine) {
    std::string test_move_line = "1. d4 d5 2. Nc3 Nf6 3. Bf4 g6 { D01 Rapport-Jobava System } 4. e3 Bg7 5. h4 c6 6. h5 Qb6 ";
    PgnParser p;
    
    bool boolean = (p.is_valid_move_line(test_move_line));
    EXPECT_TRUE(boolean);
}

TEST(ChessParserTest, TestCleanMoveLine) {
    std::string test_move_line = "1. d4 d5 2. Nc3 Nf6 3. Bf4 g6 { D01 Rapport-Jobava System } 4. e3 Bg7 5. h4 c6 6. h5 Qb6 ";
    
    PgnParser p;
    p.clean_move_line(test_move_line);
}

TEST(AppTest, TestWorkerTask) {

    App app("test_db.sqlite", 1); 


    std::vector<std::string> dummy_batch = {
        "1. e4 e5 2. Ke2",
        "1. e4 Nf6 2. e5 d5",
        "1. f3 e5 2. g4 Qh4#"
    };
    
    app.task_queue.push(dummy_batch);

    app.task_queue.set_done();

    app.worker_task(); 

    std::unordered_map<std::string, int> result_batch;
    
    bool got_data = app.db_queue.pop(result_batch);
    
    EXPECT_TRUE(got_data);
    EXPECT_FALSE(result_batch.empty());
    
    EXPECT_EQ(result_batch["rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPPKPPP/RNBQ1BNR b kq -"], 1);

    // 2. Aito En Passant -lopputulos
    // Huomaa "w KQkq d6" lopussa. Mustan d-sotilas ohitti d6-ruudun, joten se on nyt merkitty.
    EXPECT_EQ(result_batch["rnbqkb1r/ppp1pppp/5n2/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6"], 1);

    // 3. Narrinmatin lopputulos (Kuningatar h4:ssä, matriisi täysin siivottu #-merkistä)
    EXPECT_EQ(result_batch["rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq -"], 1);
}
