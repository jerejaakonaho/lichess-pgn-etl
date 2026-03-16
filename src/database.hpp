#pragma once
#include <string>
#include <vector>
#include <sqlite3.h>
#include <cstdint>

class Database {
public:
    Database(const std::string& db_name);
    ~Database();

    // The hashes from the parser -> Database
    void insert_hashes(const std::vector<uint64_t>& hashes);

    void flush();
private:
    sqlite3* db;
    sqlite3_stmt* insert_statement;
    std::vector<uint64_t> buffer;

    void create_table();
};