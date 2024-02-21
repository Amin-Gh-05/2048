#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <fstream>
#include <Windows.h>
using namespace std;

struct Score {
	int lb;
	string user;
};

void showMenu();
void startGame(int n, string user);
void randNum(int** table, int n, int points);
void moveUp(int** table, int n, int& points, bool& moved);
void moveRight(int** table, int n, int& points, bool& moved);
void moveDown(int** table, int n, int& points, bool& moved);
void moveLeft(int** table, int n, int& points, bool& moved);
void showNums(int** table, int n);
void fillLeaderBoard(int points, string user);
void pullLeaderBoard(int lc);
int measureSize();
void sortPoints(Score*& points, int n);
void ShowConsoleCursor(bool showFlag);

int main()
{
	showMenu();
}

void showMenu() {
	bool is_valid = true;

	while (true) {
		system("cls");
		int choice;
		// checks if choice of player in menu is valid
		if (!is_valid) {
			cout << "Please Enter a Valid Choice!" << endl;
		}

		cout << "1: New Game" << endl;
		cout << "2: Leaderboard" << endl;
		cout << "3: Exit" << endl;
		cout << "Enter Your Choice: ";

		cin >> choice;
		// start a new game
		if (choice == 1) {
			int n;
			string un;
			cout << "In What Size Do You Wish to Play? ";
			cin >> n;
			cout << "Enter a UserName: ";
			cin >> un;
			startGame(n, un);
		}
		// pull the leaderboard from file
		else if (choice == 2) {
			pullLeaderBoard(measureSize());
		}
		// exit app
		else if (choice == 3) {
			exit(0);
		}
		else {
			is_valid = false;
			continue;
		}
	}
}

void startGame(int n, string user) {
	ShowConsoleCursor(false);
	int points = 0;
	system("cls");

	// using a two-dimensional matrix to save the numbers
	int** table = new int* [n];
	for (int i = 0; i < n; i++) {
		table[i] = new int[n];
		for (int j = 0; j < n; j++) {
			table[i][j] = 0;
		}
	}
	// add two random nums to start the game with
	randNum(table, n, points);
	randNum(table, n, points);

	while (true) {
		// win check
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (table[i][j] == 2048) {
					cout << "You Won!" << endl;
					fillLeaderBoard(points, user);
					cin.get();
					return;
				}
			}
		}

		system("cls");
		cout << "Points: " << points << endl;
		showNums(table, n);

		// using getch function to get movement
		char move = _getch();
		bool is_moved = false;

		if (move == 'w') {
			moveUp(table, n, points, is_moved);
		}
		else if (move == 'd') {
			moveRight(table, n, points, is_moved);
		}
		else if (move == 's') {
			moveDown(table, n, points, is_moved);
		}
		else if (move == 'a') {
			moveLeft(table, n, points, is_moved);
		}
		else if (move == 'b') {
			fillLeaderBoard(points, user);
			break;
		}

		int zero_count = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (table[i][j] == 0)
					zero_count++;
			}
		}
		// lose check
		if (zero_count == 0) {
			cout << "Game Over!" << endl;
			fillLeaderBoard(points, user);
			cin.get();
			cin.get();
			return;
		}
		// insert another random number if any movement is done
		if (is_moved) {
			randNum(table, n, points);
		}
	}

	for (int i = 0; i < n; i++) {
		delete[] table[i];
	}
	delete[] table;
	ShowConsoleCursor(true);
}

void randNum(int** table, int n, int points) {
	srand(static_cast<unsigned int>(time(nullptr))); // setting the seed of rand function
	int randnum = rand() % 4;
	int result;
	if (randnum == 0) {
		result = 4; // 1 / 4 chance
	}
	else {
		result = 2; // 3 / 4 chance
	}

	int count = 0;
	// checks the count of zeros in two-dimensional array
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (table[i][j] == 0) {
				count++;
			}
		}
	}

	// make two parallel arrays to store free indexes
	int* x = new int[count];
	int* y = new int[count];
	int k = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (table[i][j] == 0) {
				x[k] = i;
				y[k] = j;
				k++;
			}
		}
	}
	// checks if there is any free tile
	if (count > 0) {
		int index = rand() % count;
		table[x[index]][y[index]] = result;
	}

	delete[] x;
	delete[] y;
}

void moveUp(int** table, int n, int& points, bool& moved) {
	for (int j = 0; j < n; j++) {
		int last = -1; // checks if the dest we're going to is set before or not
		for (int i = 1; i < n; i++) {
			if (table[i][j] != 0) {
				int dest = i;
				// find the destinaion among tiles in column
				for (int k = i - 1; k >= 0; k--) {
					if (table[k][j] == 0) {
						dest = k;
					}
					else if (table[k][j] == table[i][j] && last != k) {
						dest = k;
						break;
					}
					else {
						break;
					}
				}

				if (table[dest][j] == 0) {
					table[dest][j] = table[i][j];
					table[i][j] = 0;
					moved = true;
				}
				else if (table[dest][j] == table[i][j] && dest != i) {
					table[dest][j] *= 2;
					table[i][j] = 0;
					points += table[dest][j];
					last = dest;
					moved = true;
				}
			}
		}
	}
}

void moveRight(int** table, int n, int& points, bool& moved) {
	for (int i = 0; i < n; i++) {
		int last = -1; // checks if the dest we're going to is set before or not
		for (int j = n - 2; j >= 0; j--) {
			if (table[i][j] != 0) {
				int dest = j;
				// find the destinaion among tiles in row
				for (int k = j + 1; k < n; k++) {
					if (table[i][k] == 0) {
						dest = k;
					}
					else if (table[i][k] == table[i][j] && last != k) {
						dest = k;
						break;
					}
					else {
						break;
					}
				}

				if (table[i][dest] == 0) {
					table[i][dest] = table[i][j];
					table[i][j] = 0;
					moved = true;
				}
				else if (table[i][dest] == table[i][j] && dest != j) {
					table[i][dest] *= 2;
					table[i][j] = 0;
					points += table[i][dest];
					last = dest;
					moved = true;
				}
			}
		}
	}
}

void moveDown(int** table, int n, int& points, bool& moved) {
	for (int j = 0; j < n; j++) {
		int last = -1; // checks if the dest we're going to is set before or not
		for (int i = n - 2; i >= 0; i--) {
			if (table[i][j] != 0) {
				int dest = i;
				// find the destinaion among tiles in column
				for (int k = i + 1; k < n; k++) {
					if (table[k][j] == 0) {
						dest = k;
					}
					else if (table[k][j] == table[i][j] && last != k) {
						dest = k;
						break;
					}
					else {
						break;
					}
				}

				if (table[dest][j] == 0) {
					table[dest][j] = table[i][j];
					table[i][j] = 0;
					moved = true;
				}
				else if (table[dest][j] == table[i][j] && dest != i) {
					table[dest][j] *= 2;
					table[i][j] = 0;
					points += table[dest][j];
					last = dest;
					moved = true;
				}
			}
		}
	}
}

void moveLeft(int** table, int n, int& points, bool& moved) {
	for (int i = 0; i < n; i++) {
		int last = -1; // checks if the dest we're going to is set before or not
		for (int j = 1; j < n; j++) {
			if (table[i][j] != 0) {
				int dest = j;
				// find the destinaion among tiles in row
				for (int k = j - 1; k >= 0; k--) {
					if (table[i][k] == 0) {
						dest = k;
					}
					else if (table[i][k] == table[i][j] && last != k) {
						dest = k;
						break;
					}
					else {
						break;
					}
				}

				if (table[i][dest] == 0) {
					table[i][dest] = table[i][j];
					table[i][j] = 0;
					moved = true;
				}
				else if (table[i][dest] == table[i][j] && dest != j) {
					table[i][dest] *= 2;
					table[i][j] = 0;
					points += table[i][dest];
					last = dest;
					moved = true;
				}
			}
		}
	}
}

void showNums(int** table, int n) {
	// draws a table with unicode characters
	for (int i = 0; i < 4 * n + 1; i++) {
		for (int j = 0; j < 7 * n + 1; j++) {
			if (i == 0 && j == 0)
				cout << "\u250F";
			else if (i == 0 && j == 7 * n)
				cout << "\u2513";
			else if (i == 4 * n && j == 0)
				cout << "\u2517";
			else if (i == 4 * n && j == 7 * n)
				cout << "\u251B";
			else if (i == 0 && j % 7 == 0)
				cout << "\u2533";
			else if (i % 4 == 0 && j == 7 * n)
				cout << "\u252B";
			else if (i == 4 * n && j % 7 == 0)
				cout << "\u253B";
			else if (i % 4 == 0 && j == 0)
				cout << "\u2523";
			else if (i % 4 == 0 && j % 7 == 0)
				cout << "\u254B";
			else if (i % 4 == 0 && j % 7 != 0)
				cout << "\u2501";
			else if (i % 4 != 0 && j % 7 == 0)
				cout << "\u2503";
			else if (i % 4 == 2 && j % 7 == 2 && table[i / 4][j / 7]) {
				cout << setw(4) << table[i / 4][j / 7];
				j += 3;
			}
			else
				cout << " ";
		}
		
		cout << endl;
	}

	cout << "Press B to Go Back to Menu" << endl;
}

// fills the file every time game is stopped
void fillLeaderBoard(int points, string user) {
	ofstream myFile;
	myFile.open("leaderboard.txt", ios::app);
	// check if file is successfully opened
	if (!myFile) {
		cout << "Unable to Open the File!" << endl;
	}
	else {
		myFile << points << ' ' << user << endl;
		myFile.close();
	}
}

// counting the records of file
int measureSize() {
	int lc = 0;
	string temp; // it has almost no use
	ifstream myFile;
	myFile.open("leaderboard.txt", ios::in);
	while (getline(myFile, temp)) {
		lc++;
	}
	myFile.close();

	return lc;
}

void pullLeaderBoard(int lc) {
	ifstream myFile;
	myFile.open("leaderboard.txt", ios::in);

	Score* scs = new Score[lc];
	
	for (int i = 0; i < lc; i++) {
		myFile >> scs[i].lb >> scs[i].user;
	}
	myFile.close();

	sortPoints(scs, lc);
	
	system("cls");
	for (int i = lc - 1; i >= 0; i--) {
		cout << lc - i << " " << scs[i].user << " " << scs[i].lb << endl;
	}
	
	cin.get();
	cin.get();
}

void sortPoints(Score*& points, int n) {
	int i, j;
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (points[j].lb > points[j + 1].lb) {
				swap(points[j], points[j + 1]);
			}
		}
	}
}

void ShowConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}
