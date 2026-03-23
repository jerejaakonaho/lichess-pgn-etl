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

    void insert_fens(const std::unordered_map<std::string, int>& fen_counts);

    void flush();
private:
    sqlite3* db;
    sqlite3_stmt* insert_statement;
    std::vector<std::string> buffer;

    void create_table();
};