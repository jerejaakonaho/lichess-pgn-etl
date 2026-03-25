#pragma once
#include <string>
#include <vector>
#include <sqlite3.h>
#include <cstdint>
#include <unordered_map>

class Database {
public:
    Database(const std::string& db_name);
    ~Database();
    void begin_transaction();
    void insert_fens(const std::vector<std::pair<std::string, int>>& fen_batch);
    void commit_transaction();
    void flush();
private:
    sqlite3* db;
    sqlite3_stmt* insert_statement;
    std::vector<std::string> buffer;

    void create_table();
};