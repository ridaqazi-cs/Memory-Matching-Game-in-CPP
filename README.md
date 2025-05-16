# Memory Matching Game

A console-based Memory Matching Game written in C++ for Windows.

Players flip pairs of cards on a grid to find matching values. The fewer attempts you take per round, the higher your score. After a set number of rounds, the game summarizes your performance.

---

## Features

- Configurable number of rounds (up to 10)
- Configurable even-sized square board (e.g. 2×2, 4×4, 6×6)
- Attempts and high-score tracking per round
- Final game summary with sorted scores and highest score
- ANSI console cursor control and color output via WinAPI

---

## Repository Structure

```
memory-matching-game/
├── MemoryGame.cpp    # Game source code
├── README.md         # Project overview and instructions
```

---

## Prerequisites

- Windows 10 or later  
- Visual Studio Community 2022 with **Desktop development with C++** workload (used for development)  
- (Alternatively) MinGW-w64 (GCC) installed and added to your PATH  

---

## Build & Run

### Using Visual Studio Developer Prompt

1. Open **x64 Native Tools Command Prompt for VS**.  
2. Navigate to the project root:
   ```bat
   cd \path\to\memory-matching-game\src
   ```
3. Compile:
   ```bat
   cl /EHsc MemoryGame.cpp
   ```
4. Run:
   ```bat
   MemoryGame.exe
   ```

### Using MinGW-w64

1. Open a new Command Prompt (after adding MinGW's `bin` to PATH).  
2. Navigate to the project root:
   ```bat
   cd C:\path\to\memory-matching-game\src
   ```
3. Compile:
   ```bat
   g++ -std=c++17 MemoryGame.cpp -o MemoryGame.exe
   ```
4. Run:
   ```bat
   MemoryGame.exe
   ```

---

## Usage

1. **Rounds:** Enter the number of rounds (1–10).  
2. **Board Size:** Enter an even integer for the board dimension (e.g. 4 for a 4×4 grid).  
3. **Memorize:** The board’s card values are revealed briefly.  
4. **Play:** Enter two positions per turn to flip cards. Matching pair remains face-up.  
5. **Complete Round:** Once all pairs are found, your score is recorded.  
6. **Game Summary:** After all rounds, view sorted scores and highest score.

---
