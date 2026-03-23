#include <iostream>
#include "database.hpp"

Database::Database(const std::string& db_name) {
    // Open db or create if there isn't one
    if (sqlite3_open(db_name.c_str(), &db) != SQLITE_OK) {
        std::cerr << "DB failed to open: " << sqlite3_errmsg(db) << std::endl;
    }

    create_table();

    // SQL command
    const char* sql = "INSERT INTO positions (fen, count) VALUES (?, ?) "
                  "ON CONFLICT(fen) DO UPDATE SET count = count + excluded.count;";
    sqlite3_prepare_v2(db, sql, -1, &insert_statement, nullptr);

    sqlite3_exec(db, "PRAGMA synchronous = NORMAL;", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "PRAGMA journal_mode = WAL;", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "PRAGMA cache_size = -2000000;", nullptr, nullptr, nullptr); // 2 Gt välimuistia
    sqlite3_exec(db, "PRAGMA mmap_size = 0;", nullptr, nullptr, nullptr); // Memory mapping
    buffer.reserve(900000);
}

Database::~Database() {
    flush(); // Force flush even if the buffer isnt full
    sqlite3_finalize(insert_statement);
    sqlite3_close(db);
}

void Database::create_table() {
    const char* sql = "CREATE TABLE IF NOT EXISTS positions (fen TEXT PRIMARY KEY, count INT DEFAULT 1);";
    sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
}

void Database::begin_transaction() {
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
}

void Database::insert_fens(const std::unordered_map<std::string, int>& fen_batch) {
    for (const auto& pair : fen_batch) {
        sqlite3_bind_text(insert_statement, 1, pair.first.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(insert_statement, 2, pair.second);
        
        sqlite3_step(insert_statement);
        sqlite3_reset(insert_statement);
    }
}

void Database::commit_transaction() {
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
}

void Database::flush() {
    if (buffer.empty()) return;

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    for (const std::string& fen : buffer) {
        sqlite3_bind_text(insert_statement, 1, fen.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(insert_statement); // execute
        sqlite3_reset(insert_statement); // reset for next hash
    }

    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);

    buffer.clear();
}