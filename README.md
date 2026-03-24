# ♟️ Lichess Data Pipeline

## Project Overview
This project is a high-performance, C++ based program designed to process massive datasets from the [Lichess database](https://database.lichess.org/). The engine streams raw `.pgn.zst` files and converts game moves into **FEN (Forsyth–Edwards Notation)** strings, storing the resulting board states in a structured SQLite database.

---

## Tech Stack
* **Modern C++:** Written in **C++17**, leveraging `std::string_view` for high-performance text parsing without unnecessary allocations.
* **SQLite:** Used as the primary storage engine for efficient data retrieval and indexing.
* **Zstandard (zstd):** Utilized for real-time decompression to handle gigabytes of data with minimal disk footprint.

---

## Architecture and Data Flow
The core of the application is built as a multi-stage processing pipeline to ensure maximum throughput:

1.  **Extraction:** Filters and extracts move lines from raw PGN data using optimized `is_valid_move_line` and `clean_move_line` logic.
2.  **Transformation:** Tokenizes text-based moves into an `std::vector<std::string_view>` to maintain a zero-copy architecture.
3.  **FEN Generation:** Simulates the board state for every move in the game and generates a standardized **FEN string** representing the exact position.
4.  **Loading:** Performs high-speed batch insertions of the FEN strings into the SQLite database.

---

## Performance
The engine is engineered for extreme efficiency, capable of processing hundreds of gigabytes of data entirely in RAM without temporary disk files.

| Feature | Description |
| :--- | :--- |
| **Throughput** | ~14,000 games per second |
| **I/O Management** | Zstandard-streaming directly to RAM |
| **Memory Management** | Zero-copy architecture using string views |
| **Database Speed** | Millions of FEN strings written per second |

---

## Dependencies & Prerequisites
To run the pipeline, you need:
* **zstd**: Required for decompressing Lichess database files.
    * *Install on Ubuntu/WSL:* `sudo apt install zstd`
* **chess-library**: [Disservin/chess-library](https://github.com/Disservin/chess-library) for board logic.
* **SQLite3**: The standard database engine.

---

## Usage

### 1. Compiling
The program is built using the provided Makefile:

```bash
make
