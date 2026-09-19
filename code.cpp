#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;

const int m = 20, n = 40;

int row = 15, col = 19;

const int maxBullets = 18;
int bulletRow[maxBullets], bulletCol[maxBullets];
bool bulletFired[maxBullets] = { false };

const int maxChicken = 19;
int chickens = 0;
bool chickenSpawned[maxChicken] = { false };
int chickenRow[maxChicken], chickenCol[maxChicken];
bool movingRight[maxChicken] = { true };

int score = 0, lives = 3;

const int maxEggs = 20;
bool eggDropped[maxEggs] = { false };
int eggRow[maxEggs], eggCol[maxEggs];

int level = 1;
int nextLevel = 190;

int HScore[10] = { 0,0,0,0,0,0,0,0,0 };
string name[10];
string naam;


void welcome();
void menu();
void Game();
void instructions();
void showGrid(char grid[m][n], int row, int col);
void updateGrid(char direction, int& row, int& col);
void map_print(char grid[m][n]);
void initialize_map(char grid[m][n]);
void fireBullet();
void processInput(char& direction);
void updateBullets(char grid[m][n]);
void initializeChickens();
void updateChickens(char grid[m][n]);
void updateEggs(char grid[m][n]);
void dropEgg();
bool gameOver();
void stats();
void fileHandling();
void gotoxy(int x, int y);
void levelUp();
void showFile();

void Game() {
    char grid[m][n];
    char direction = '\0';
    cout << setw(20) << right << "Level 1" << endl;
    Sleep(1000);
    system("CLS");
    while (gameOver()) {
        initialize_map(grid);  // Reset the grid before updating
        processInput(direction);  // Process player input (movement and firing)
        updateBullets(grid);  // Update bullet movement and logic
        initializeChickens(); //spawns Chicken
        updateChickens(grid); //moves chickens
        dropEgg(); //spawns egg
        updateEggs(grid); //move eggs
        showGrid(grid, row, col);  // Show the player and bullets
        map_print(grid);  // Display the grid with updated positions

        if (level == 1)//game speed
            Sleep(1);
        else if (level == 2)
            Sleep(16);
        else if (level == 3)
            Sleep(10);
        else if (level == 4)
        {
            system("CLS");
            cout << "Game Completed!\n";
            system("pause");
            break;
        }
        stats();
        gotoxy(0, 0);
    }
    if (lives <= 0)
    {
        system("CLS");
        cout << "Game Over!" << endl;
        system("pause");
    }
    fileHandling();//maintains files for high score
    system("cls");
}

void welcome() {
    cout << "\n\n\n\n\n\t\t\t Welcome to Chicken Invaders" << endl;
    cout << endl << "\t\t\t\tGet ready to fight waves of EVIL chicken!" << endl;
    cout << "\n\t\t\t\t\t\tLoading";
    for (int i = 0; i < 5; i++)
    {
        cout << ".";
        Sleep(200);
    }
    cout << "\b\b\b\b\b";
    for (int i = 0; i < 5; i++)
    {
        cout << " ";
        Sleep(200);
    }
    cout << "\b\b\b\b\b";
    for (int i = 0; i < 5; i++)
    {
        cout << ".";
        Sleep(200);
    }
    cout << "Completed";
    Sleep(500);
    system("cls");
}

void menu() {
    system("cls");
    int input;
    do {
        cout << "\n\n1. Start Game";
        cout << "\n2. Tutorial";
        cout << "\n3. Leader Board";
        cout << "\n4. Exit\n\n";
        cout << "Enter your Choice:";
        cin >> input;
        switch (input) {
        case 1:
            system("cls");
            Game();
            break;
        case 2:
            instructions();
            break;
        case 3:
            showFile();
            menu();
        case 4:
            system("cls");
            cout << "\n\n\n\n\n\t\t\tBye Bye";
            Sleep(1000);
            system("cls");
            exit(0);
        default:
            system("cls");
            cout << "\nInvalid Choice!\nPress Again";
            Sleep(2000);
            system("cls");
        }
    } while (input != 1);
    system("cls");
}

void instructions() {
    system("cls");
    cout << "Press w and you will move up.\n" << endl;
    cout << "Press s and you will move down.\n" << endl;
    cout << "Press a and you will move left\n" << endl;
    cout << "Press d and you will move right.\n" << endl;
    cout << "Killing a Chicken gives you 10 points.\n" << endl;
    cout << "You lose a life when a chicken hits you or an egg hits you.\n" << endl;
    cout << "Game will end if any chicken reaches the bottom.\n" << endl;
    cout << "Press any key to continue.";
    _getch();
    menu();
}

// Function to initialize/reset the game grid
void initialize_map(char grid[m][n]) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            grid[i][j] = ' ';
        }
    }
}

// Function to initialize the player and grid
void showGrid(char grid[m][n], int row, int col) {
    // Set boundary walls
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == 0 || i == m - 1 || j == 0 || j == n - 1) {
                grid[i][j] = '#';  // Boundary walls
            }
        }
    }
    // Set player position
    grid[row][col] = '$';
}

// Function to display the game grid
void map_print(char grid[m][n]) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << grid[i][j];
        }
        cout << endl;
    }
}

// Function to handle player movement and firing
void processInput(char& direction) {
    if (_kbhit()) {
        direction = _getch();  // Get input key

        if (direction == ' ') {  // Spacebar to fire (only if no bullet is currently fired)
            fireBullet();    //Calls function to fire bullet
        }
        updateGrid(direction, row, col);  // Update player position based on direction
    }
}

// Function to update the player's position based on input
void updateGrid(char direction, int& row, int& col) {
    if (direction == 'W' || direction == 'w')
        row--;
    else if (direction == 'S' || direction == 's')
        row++;
    else if (direction == 'A' || direction == 'a')
        col--;
    else if (direction == 'D' || direction == 'd')
        col++;

    // Boundary checks to prevent going out of bounds
    if (row <= 0)
        row = 1;
    if (col <= 0)
        col = 1;
    if (row >= m - 1)
        row = m - 2;
    if (col >= n - 1)
        col = n - 2;
}

// Function to fire a bullet
void fireBullet() {
    for (int i = 0; i < maxBullets; i++) {
        if (!bulletFired[i]) {
            bulletRow[i] = row - 1;  // Place the bullet just above the player
            bulletCol[i] = col;
            bulletFired[i] = true;   // Mark the bullet as fired
            break;
        }
    }
}

// Function to move the bullets' positions
void updateBullets(char grid[m][n]) {
    for (int i = 0; i < maxBullets; i++) {
        if (bulletFired[i]) {
            grid[bulletRow[i]][bulletCol[i]] = ' ';  // Clear the previous bullet position


            if (bulletRow[i] > 0) {
                bulletRow[i]--;  // Move the bullet up
            }
            else {
                bulletFired[i] = false;  // Stop the bullet when it hits the top
            }

            // Place the bullet at its new position
            if (bulletRow[i] >= 0) {
                grid[bulletRow[i]][bulletCol[i]] = '*';
            }

        }
    }
}


//function to spawn chicken
void initializeChickens() {
    for (int i = 0; i < maxChicken; i = i + level) {
        if (!chickenSpawned[i] && chickens < maxChicken) {  // Only spawn new chickens if not already spawned and total chickens in level less than 19
            chickenRow[i] = 1;
            chickenCol[i] = (i * 2) % (n - 2) + 1; // Chickens Spread across columns
            chickenSpawned[i] = true;
            chickens++;
        }
    }
    levelUp();
}

void levelUp() {
    if (score >= nextLevel) {
        level++;
        chickens = 0;  // Reset chicken count for the new level
        system("CLS");
        if (level != 4)
            cout << setw(20) << right << "Level: " << level;//Display level before start
        Sleep(1000);
        system("CLS");
        nextLevel += 190; //increase the score to reach next level by 190
    }
}

void updateChickens(char grid[m][n]) {
    for (int i = 0; i < maxChicken; ++i) {
        if (chickenSpawned[i]) {
            grid[chickenRow[i]][chickenCol[i]] = ' '; // Clear previous position
            // Move chicken
            if (movingRight[i]) {
                if (chickenCol[i] < n - 1) {
                    chickenCol[i]++;
                }
                else {
                    movingRight[i] = false; // Change direction
                    chickenRow[i]++;
                }
            }
            else {
                if (chickenCol[i] > 0) {
                    chickenCol[i]--;
                }
                else {
                    movingRight[i] = true; // Change direction
                    chickenRow[i]++;
                }
            }

            // Place chicken at new position
            if (chickenRow[i] < m) {
                grid[chickenRow[i]][chickenCol[i]] = '&';
            }
        }
    }
}
bool gameOver() {
    for (int i = 0; i < maxBullets; ++i) {
        if (bulletFired[i]) {  // Only check collisions for active bullets
            for (int j = 0; j < maxChicken; ++j) {
                if (chickenSpawned[j] && (bulletRow[i] == chickenRow[j]) && (bulletCol[i] == chickenCol[j])) {
                    score += 10;  // Increase score
                    bulletFired[i] = false;  // Remove the bullet
                    chickenSpawned[j] = false;  // Remove the chicken
                }
            }
        }
    }

    for (int j = 0; j < maxChicken; ++j) {
        if (chickenSpawned[j]) {
            // Check collision with player
            if (chickenRow[j] == row && chickenCol[j] == col) {
                lives--;  // Decrease lives
                nextLevel -= 10;
                if (row != m - 1)
                    row++;
                chickenSpawned[j] = false;  // Remove the chicken
            }

            // Check if chicken reaches the bottom
            if (chickenRow[j] >= m - 2) {
                lives -= 3;  // Lose 3 lives
                chickenSpawned[j] = false;  // Remove the chicken
            }
        }
    }
    for (int i = 0; i < maxEggs; ++i)
        // Check for collision with the player
        if (eggRow[i] == row && eggCol[i] == col) {
            lives--;  // Player loses a life
            eggDropped[i] = false;  // Remove the egg
        }

    if (lives > 0)
        return true;
    return false;
}

void dropEgg() {
    srand(time(0));
    for (int i = 0; i < maxChicken; ++i) {
        if (chickenSpawned[i] && rand() % (12 / level) == 0) {  // Random chance to drop an egg
            for (int j = 0; j < 3; ++j) {
                if (!eggDropped[j]) {  // Find an empty slot for the new egg
                    eggRow[j] = chickenRow[i] + 1;  // Start just below the chicken
                    eggCol[j] = chickenCol[i];
                    eggDropped[j] = true;
                    break;
                }
            }
        }
    }
}

void updateEggs(char grid[m][n]) {
    for (int i = 0; i < maxEggs; ++i) {

        if (eggDropped[i]) {
            grid[eggRow[i]][eggCol[i]] = ' ';  // Clear the previous position

            // Move egg downwards
            if (eggRow[i] < m - 1) {
                eggRow[i]++;
            }
            else {
                eggDropped[i] = false;  // Remove egg if it hits the bottom
            }

            // Place the egg at its new position
            if (eggDropped[i]) {
                grid[eggRow[i]][eggCol[i]] = 'O';
            }
        }
    }
}

void stats() {
    cout << endl << "Level:" << level << endl;
    cout << "Score:" << score << endl;
    cout << "Lives:" << lives << endl;
}

void fileHandling() {
    int j = 0;
    int tempH[10];
    string tempN[10];
    int value;
    string names;
    ifstream read;
    read.open("HighScore.txt");
    while (j < 10) {//reads the previous heigh scores from the file zahid mursleen
        read >> names >> value;
        HScore[j] = value;
        name[j] = names;
        j++;
    }

    read.close();
    int min = HScore[0], minIndex = 0;
    for (int i = 0; i < 10; ++i) {//finds out the minimum score in the file
        if (min > HScore[i])
        {
            min = HScore[i];
            minIndex = i;
        }

    }
    if (min < score) {//if minimum score is less then current score then its value is replaced by current score
        { 
            HScore[minIndex] = score; 
            name[minIndex] = naam;
        }
    }
    for (int i = 0; i < 10; ++i) {//sorts high score in acending order using bubble sort
        for (int j = 0; j < 9 - i; ++j) {
            if (HScore[j] < HScore[j + 1])
            {
                int temph = HScore[j];
                HScore[j] = HScore[j + 1];
                HScore[j + 1] = temph;

                string tempn = name[j];
                name[j] = name[j + 1];
                name[j + 1] = tempn;
            }
        }
    }
    for (int i = 0; i < 10; ++i) {//reverses the list in another array so now it is in descending order
        tempH[i] = HScore[9 - i];
        tempN[i] = name[9 - i];
    }
    for (int i = 0; i < 10; ++i) {//gives back the descending order array to original high score array
        HScore[i] = tempH[i];
        name[i] = tempN[i];
    }
    ofstream write;
    write.open("HighScore.txt");
    for (int i = 0; i < 10; ++i)//writes the updated high scores in file
    {
        write << name[i]<<'\t'<<HScore[i]<<endl;
    }
    write.close();
    showFile();
}
int main() {

    welcome();
    cout << "Your first name:";
    cin >> naam;
    menu();

    return 0;
}

void showFile() {
    int count = 0;
    int value;
    string names;
    system("cls");
    cout << "High Scores" << endl;
    ifstream show;
    show.open("HighScore.txt");
    while (count < 10) {
        show >> names;
        show >> value;
        cout << names << '\t' << value << endl;
        count++;
    }
    show.close();
    cout << "\n Press any key to continue...";
    _getch();
    system("cls");
}
void gotoxy(int x, int y)
{
    COORD c = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}