#include <iostream>
#include <vector>
#include <conio.h>
#define NOMINMAX
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
using namespace std;

#define V 25
#define H 79

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD position;
void coordinates(int x, int y) {
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition(console, position);
}

void gameborder() {
    system("cls");
    for (int i = 2; i <= H; i++) {
        coordinates(i, 1);  cout << '-';
        coordinates(i, V);  cout << '-';
    }
    for (int i = 1; i <= V; i++) {
        coordinates(2, i);  cout << '|';
        coordinates(H, i);  cout << '|';
    }
    for (int i = 3; i <= H - 1; i++) {
        coordinates(i, 4);  cout << '-';
    }
    coordinates(4, 3);   cout << "ATTEMPTS: 0";
    coordinates(24, 3);  cout << "ROUND: 0";
    coordinates(44, 3);  cout << "HIGHSCORE: 0";
}

class Node {
public:
    int value;
    bool flipped;
    Node* next;
    Node(int val) : value(val), flipped(false), next(nullptr) {}
};

class Score {
public:
    int roundsCount;
    int *stack, *roundIndex;
    int top, r;

    Score(int rounds)
      : roundsCount(rounds),
        stack(new int[rounds]),
        roundIndex(new int[rounds]),
        top(-1),
        r(1)
    {}

    ~Score() {
        delete[] stack;
        delete[] roundIndex;
    }

    void Push(int data) {
        stack[++top]     = data;
        roundIndex[top] = r++;
    }

    int hscore() const {
        int best = 0;
        for (int i = 0; i <= top; ++i)
            if (stack[i] > best) best = stack[i];
        return best;
    }

    void swap(int &a, int &b) {
        int tmp = a; a = b; b = tmp;
    }

    int partition(int low, int high) {
        int pivot = stack[high], i = low - 1;
        for (int j = low; j < high; ++j) {
            if (stack[j] >= pivot) {
                swap(stack[++i], stack[j]);
                swap(roundIndex[i], roundIndex[j]);
            }
        }
        swap(stack[i+1], stack[high]);
        swap(roundIndex[i+1], roundIndex[high]);
        return i + 1;
    }

    void quicksort(int low, int high) {
        if (low < high) {
            int p = partition(low, high);
            quicksort(low, p - 1);
            quicksort(p + 1, high);
        }
    }

    void sortScores() {
        quicksort(0, top);
    }
};

class Game {
private:
    int size;
    Node* head;
    int attempts;
    static int cround;
    Score s;

    Node* findNode(Node* cur, int pos) {
        if (!cur || pos == 0) return cur;
        return findNode(cur->next, pos - 1);
    }

    bool isGameOver() {
        for (Node* cur = head; cur; cur = cur->next)
            if (!cur->flipped) return false;
        return true;
    }

    void revealCard(Node* cur, int pos) {
        if (pos == 0) {
            cur->flipped = true;
            return;
        }
        revealCard(cur->next, pos - 1);
    }

public:
    Game(int rounds, int boardSize)
      : size(boardSize),
        head(nullptr),
        attempts(0),
        s(rounds)
    {}

    ~Game() {
        while (head) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    void initialize() {
        int total = size * size;
        // use vector instead of raw new[]
        vector<int> cards(total);
        // fill pairs
        for (int i = 0; i < total; i += 2) {
            cards[i]   = i/2 + 1;
            cards[i+1] = i/2 + 1;
        }
        // shuffle
        srand((unsigned)time(0));
        for (int i = total - 1; i > 0; --i) {
            int j = rand() % (i + 1);
            std::swap(cards[i], cards[j]);
        }
        // build linked list
        head = nullptr;
        for (int i = 0; i < total; ++i) {
            Node* node = new Node(cards[i]);
            node->next = head;
            head = node;
        }
        attempts = 0;
    }

    void displayBoardValues() {
        Node* cur = head;
        SetConsoleTextAttribute(console, 12);
        for (int i = 0; i < size; ++i) {
            coordinates(6, 7 + i);
            for (int j = 0; j < size; ++j) {
                cout << setw(4) << cur->value;
                cur = cur->next;
            }
        }
    }

    void displayBoard() {
        Node* cur = head;
        for (int i = 0; i < size; ++i) {
            coordinates(6, 7 + i);
            for (int j = 0; j < size; ++j) {
                if (cur->flipped) {
                    SetConsoleTextAttribute(console, 11);
                    cout << setw(4) << cur->value;
                } else {
                    SetConsoleTextAttribute(console, 12);
                    cout << setw(4) << (i * size + j);
                }
                cur = cur->next;
            }
        }
    }

    void playMemoryGame() {
        while (!isGameOver()) {
            system("cls");
            gameborder();
            coordinates(4, 3);  cout << "ATTEMPTS: "  << attempts;
            coordinates(24, 3); cout << "ROUND: "     << cround;
            coordinates(44, 3); cout << "HIGHSCORE: " << s.hscore();
            displayBoard();

            int p1, p2;
            coordinates(6, 12); cout << "First pos: ";  cin >> p1;
            coordinates(6, 13); cout << "Second pos: "; cin >> p2;

            Node* n1 = findNode(head, p1);
            Node* n2 = findNode(head, p2);
            if (!n1 || !n2 || n1->flipped || n2->flipped || p1 == p2) {
                coordinates(6, 15);
                cout << "Invalid. Try again.";
                Sleep(1000);
                continue;
            }

            if (n1->value == n2->value) {
                coordinates(6, 16); cout << "Match!";
                revealCard(head, p1);
                revealCard(head, p2);
            } else {
                coordinates(6, 16); cout << "No match.";
            }

            attempts++;
            Sleep(1000);
        }

        // push one score per round
        int roundScore = 100 - attempts;
        s.Push(roundScore);

        cround++;
        coordinates(6, 17);
        cout << "Completed in " << attempts << " attempts.";
        Sleep(2000);
    }

    void Performance(int totalRounds) {
        system("cls");
        gameborder();
        s.sortScores();
        coordinates(24, 3);  cout << "GAME SUMMARY";
        for (int i = 0; i < totalRounds; ++i) {
            coordinates(6, 6 + 2*i);
            cout << "Round#" << s.roundIndex[i]
                 << "  Score:" << s.stack[i]
                 << (s.stack[i] == 0 ? " (failed)" : "");
        }
        coordinates(6, 6 + 2*totalRounds + 1);
        cout << "High Score: " << s.hscore();
        coordinates(6, 6 + 2*totalRounds + 3);
        cout << "THANK YOU FOR PLAYING :)";
    }
};

int Game::cround = 1;

int main() {
    coordinates(34, 4);  cout << "Memory Matching Game";
    coordinates(34,10);  cout << "Press any key to start...";
    _kbhit();
    system("cls");

    int rounds, boardSize;
    gameborder();

    coordinates(6,6);
    cout << "Enter rounds (<10): ";  cin >> rounds;
    while (rounds <= 0 || rounds > 10) {
        coordinates(6,7);
        cout << "Invalid. Enter again: ";
        cin >> rounds;
    }

    coordinates(6,8);
    cout << "Enter board size (even): ";  cin >> boardSize;
    while (boardSize <= 0 || boardSize % 2 != 0) {
        coordinates(6,9);
        cout << "Invalid. Enter again: ";
        cin >> boardSize;
    }

    Game game(rounds, boardSize);
    for (int i = 0; i < rounds; ++i) {
        game.initialize();
        gameborder();
        coordinates(6,6);  cout << "Memorize!";
        game.displayBoardValues();
        Sleep(3000);
        game.playMemoryGame();
    }

    game.Performance(rounds);
    return 0;
}
