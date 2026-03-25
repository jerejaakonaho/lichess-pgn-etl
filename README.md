# ♟️ Lichess Data Engine
## Project overview
This project is a high-performance, C++ based program that uses .pgn.zst files from Lichess database (https://database.lichess.org/). The program streams the raw PGN data and converts it into board positions in FEN format and stores them in a SQLite database.

## Tech stack
* **Modern C++** Written in C++17.
* **SQLite** Used SQLite for the database as the goal wasn't to make a product out of this.

## Architecture and data flow

The core of the application is a multi-stage processing pipeline, visualized below:

![Data processing](../assets/image.png)

1. **Extraction:** Extract only the moves from the raw PGN data utilizing methods `is_valid_move_line` and `clean_move_line`.
2. **Transformation:** Converts text-based moves into an `std::vector<std::string_view>` format, avoiding unnecessary string allocations.
3. **FEN Generation:** Generates a **FEN string** (Forsyth-Edwards Notation) for each board position based on the exact state of the chessboard.
4. **Loading:** Saves the FEN strings into an SQLite database.

## Performance
This engine is optimized to handle hundreds of gigabytes of data without creating any temporary files on the disk.

| Feature | Description |
| :--- | :--- |
| **Speed** | ~14 000 games/second. |
| **I/O-management** | Zstandard-streaming directly to RAM |
| **Memory management** | Zero-copy architecture |
| **Database** | Millions of rows written per second |

## Dependencies & prerequisites
To run the pipeline, you need:
* **zstd**: used to decompress Lichess database files. 
  * Install on Ubuntu/WSL: `sudo apt install zstd`

This project uses the following third-party libraries:
* [chess-library](https://github.com/Disservin/chess-library) by Disservin
* [SQLite3](https://www.sqlite.org/) - Database engine (standard on most Linux systems).

## Usage
### 1. Compiling
The program is built using the Makefile:

```bash
make
```

### 2. Running
Stream data directly from a compressed `.zst` file without extracting it to your hard drive:

```bash
zstd -dc lichess_db.zst | ./parser
```
