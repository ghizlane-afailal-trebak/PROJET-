#include "raylib.h"
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <fstream>
// Constantes globales
#define GRAY CLITERAL(Color){ 64, 69, 77, 255 }
const int screenWidth = 800;
const int screenHeight = 630;
int cellSize = 40;
// Variables globales  
int cols, rows;
bool gameStarted = false;
double startTime = 0.0;
double endTime = 0.0;
int playerWidth, playerHeight;
enum GameState { MENU, GAME, VICTORY };
GameState gameState = MENU;
class Cell;

std::vector<Cell> grid;
std::stack<Cell*> stack;

class Cell {
  private :
    int x, y; // coordonnees
    bool visited; 
    bool walls[4]; // 4 Murs
  public :
    Cell(int x, int y) : x(x), y(y), visited(false) { // constactor
        walls[0] = walls[1] = walls[2] = walls[3] = true; 
    }
    void Draw() { // fonction bach t3ml table Kaml mrba3
        if (walls[0]) DrawLine(x * cellSize, y * cellSize, (x + 1) * cellSize, y * cellSize, BLACK); // trsm lhayt lwl  
        if (walls[1]) DrawLine((x + 1) * cellSize, y * cellSize, (x + 1) * cellSize, (y + 1) * cellSize, BLACK);
        if (walls[2]) DrawLine((x + 1) * cellSize, (y + 1) * cellSize, x * cellSize, (y + 1) * cellSize, BLACK);
        if (walls[3]) DrawLine(x * cellSize, (y + 1) * cellSize, x * cellSize, y * cellSize, BLACK);
    }
    Cell* GetCell(int x, int y) { // 
        if (x < 0 || y < 0 || x >= cols || y >= rows) return nullptr; // SI LES COORD NEGATIVE ET KHARIJ LES MURS RETURN NULLPTR
        return &grid[y * cols + x]; // RETURN VECTOR QUI DONNE CELLULE 1D
    }
    void RemoveWalls(Cell* next) { // pour supprimer les mures 
        int dx = x - next->x;
        int dy = y - next->y;
        if (dx == 1) {
            walls[3] = false;
            next->walls[1] = false;
        } else if (dx == -1) {
            walls[1] = false;
            next->walls[3] = false;
        }
        if (dy == 1) {
            walls[0] = false;
            next->walls[2] = false;
        } else if (dy == -1) {
            walls[2] = false;
            next->walls[0] = false;
        }
    }
    friend class Maze ;
    friend class Player ;
};

class Maze {
public:
    void GenerateMaze() {
        Cell* current = &grid[0]; // CURRENT la première cellule du tableau grid (généralement en haut à gauche du labyrinthe).
        current->visited = true;  
        stack.push(current);
        while (!stack.empty()) { // La boucle continue tant qu'il y a des cellules dans la pile.  
            current = stack.top();
            std::vector<Cell*> neighbors;
            Cell* top = current->GetCell(current->x, current->y - 1); // les cellules voisins
            Cell* right = current->GetCell(current->x + 1, current->y);
            Cell* bottom = current->GetCell(current->x, current->y + 1);
            Cell* left = current->GetCell(current->x - 1, current->y);
            if (top && !top->visited) neighbors.push_back(top);
            if (right && !right->visited) neighbors.push_back(right); 
            if (bottom && !bottom->visited) neighbors.push_back(bottom);
            if (left && !left->visited) neighbors.push_back(left);  
            if (!neighbors.empty()) {
                Cell* next = neighbors[rand() % neighbors.size()];
                next->visited = true;
                current->RemoveWalls(next); 
                stack.push(next);
            } else {
                stack.pop();
            }
        }
    }
};


Texture2D startScreenImage ; 
Texture2D playerTexture;
Texture2D player2Texture; 

class Player {
 private:
    int x, y;
   public: 
    Player(int startX, int startY) : x(startX), y(startY) {}
      void Draw() {
    Vector2 position = {(float)(x * cellSize + 1), (float)(y * cellSize + 1)};
    DrawTextureEx(playerTexture, position, 0.0f, (float)playerWidth / playerTexture.width, WHITE);
}
 void Draw2() {
    Vector2 position = {(float)(x * cellSize + 1), (float)(y * cellSize + 1)};
    DrawTextureEx(player2Texture, position, 0.0f, (float)playerHeight / playerTexture.height, WHITE);
}
    void Move(int dx, int dy) {
        x += dx;
        y += dy;
    }
    
void GameStart (Player& goal, Player& player){
         if (IsKeyPressed(KEY_RIGHT) && !grid[player.y * cols + player.x].walls[1]) player.Move(1, 0);
            if (IsKeyPressed(KEY_LEFT) && !grid[player.y * cols + player.x].walls[3]) player.Move(-1, 0);
            if (IsKeyPressed(KEY_DOWN) && !grid[player.y * cols + player.x].walls[2]) player.Move(0, 1);
            if (IsKeyPressed(KEY_UP) && !grid[player.y * cols + player.x].walls[0]) player.Move(0, -1);
            if (player.x == goal.x && player.y == goal.y) {
                endTime = GetTime();
                //SaveScore(endTime - startTime);
                gameState = VICTORY;        }
    }
};

void DrawTimer(double startTime, double endTime, bool hasWon) {
    double elapsed = hasWon ? endTime - startTime : GetTime() - startTime;
    int minutes = static_cast<int>(elapsed) / 60;
    int seconds = static_cast<int>(elapsed) % 60;
    char timerText[20];
    sprintf(timerText, "Time: %02d:%02d", minutes, seconds);
    //DrawText(timerText, 10, 10, 20, RED);
     // Afficher le timer en bas à gauche, juste au-dessus du bouton restart
    int timerY = screenHeight - 40;  // Placer le timer légèrement au-dessus du bouton restart
    DrawText(timerText, 10, timerY, 20, RED);
}
void SaveScore(double elapsedTime) {
    std::ofstream scoreFile("scores.txt", std::ios::app);
    if (scoreFile.is_open()) {
        scoreFile << "Time: " << elapsedTime << " seconds\n";
        scoreFile.close();
    }
} 
int SelectDifficultyLevel() {
    int level = 1;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
         DrawTexture(startScreenImage, 0, 0, WHITE);
        DrawText("   Select Difficulty Level:", 150, 230, 40, WHITE);
        DrawText("       1 - Easy", 210, 300, 27, YELLOW);
        DrawText("       2 - Medium",210, 340, 27, ORANGE);
        DrawText("       3 - Hard", 210, 385, 29, RED);
        if (IsKeyPressed(KEY_ONE)) {
            level = 1;
            break;
        }
        if (IsKeyPressed(KEY_TWO)) {
            level = 2;
            break;
        }
        if (IsKeyPressed(KEY_THREE)) {
            level = 3;
            break;
        }
        EndDrawing();
    }
    return level;
}
void StartGame(int level) {
    gameState = GAME;
    startTime = GetTime();
    Maze m ;
    switch (level) {
        case 1: 
            cols = 11; 
            rows = 8; 
            playerHeight = 50;
            playerWidth = 52 ; // Adjust cell size for easy level
            break;
        case 2: 
            cols = 19; 
            rows = 14; 
            playerHeight =40;
            playerWidth = 40 ;  // Adjust cell size for medium level
            break;
        case 3: 
            cols = 40; 
            rows = 30; 
            playerHeight = 16;
            playerWidth = 17 ;  // Adjust cell size for hard level
            break;
    }
    cellSize = screenWidth / cols;
    // Adjust player size proportionally to cellSize
    srand(time(0)); 
    grid.clear(); 
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            grid.push_back(Cell(x, y));
        }
    }
    m.GenerateMaze();
}
void DrawStarticon(Rectangle restartButton) { 
      DrawRectangleRec(restartButton, RED);
    DrawText("RESTART", restartButton.x + 23, restartButton.y + 10, 15, WHITE);
}
void DrawStartScreen(Rectangle startButton) {
    ClearBackground(BLACK);
     DrawTexture(startScreenImage, 0, 0, WHITE);
    DrawText("Welcome to the Maze Game!", screenWidth / 2 - 200, screenHeight / 2 - 60, 30, WHITE);
    DrawRectangleRec(startButton, GRAY);
    DrawText("START", startButton.x + 37, startButton.y + 10, 26, WHITE);
}
void DrawVictoryScreen(Rectangle restartButton, double elapsedTime) {
    ClearBackground(BLACK);
    DrawTexture(startScreenImage, 0, 0, WHITE);
    DrawText("Congratulations! You reached the goal!", screenWidth / 2 - 180, screenHeight / 2 - 60, 20, YELLOW);
    char scoreText[50];
    sprintf(scoreText, "Final Time: %.2f seconds", elapsedTime);
    DrawText(scoreText, screenWidth / 2 - 100, screenHeight / 2, 20, GREEN);
    DrawRectangleRec(restartButton, WHITE);
    DrawText("RESTART", restartButton.x + 18, restartButton.y + 10, 25, BLACK);
}

//FONCTION PRINCIPALE

int main() {
    InitWindow(screenWidth  , screenHeight  , "Maze Game with Victory Screen");
    SetTargetFPS(60);
     playerTexture = LoadTexture("player/tommy.png");
     player2Texture = LoadTexture("player/jerry.png");
     startScreenImage = LoadTexture("player/maze intro.png");
    Rectangle startButton = {screenWidth / 2 - 75, screenHeight / 2 - 25, 150, 50};
    Rectangle restartButton = {screenWidth / 2 - 75, screenHeight / 2 + 50, 150, 50};
    Rectangle restartButton2 = { 600 ,585, 110, 30};
    int difficultyLevel = 1;
    Player player(0, 0);
    

    Player goal(cols - 1, rows - 1);

    while (!WindowShouldClose()) {
        if (gameState == MENU) {
            BeginDrawing();
            DrawStartScreen(startButton);

            if (CheckCollisionPointRec(GetMousePosition(), startButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                difficultyLevel = SelectDifficultyLevel();
                StartGame(difficultyLevel);
                player = Player( 0 ,  0);
                goal = Player (cols - 1, rows -1 );
            }
            EndDrawing();

        } else if (gameState == GAME) {
            BeginDrawing();
            ClearBackground(WHITE);
            for (auto& cell : grid) cell.Draw();
              player.Draw();
              goal.Draw2() ;
              player.GameStart(goal, player);
            DrawStarticon(restartButton2);
            if (CheckCollisionPointRec(GetMousePosition(), restartButton2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                gameState = MENU;
            }
            DrawTimer(startTime, endTime, gameState == VICTORY);
            EndDrawing();

        } else if (gameState == VICTORY) {
            BeginDrawing();
            DrawVictoryScreen(restartButton, endTime - startTime);

            if (CheckCollisionPointRec(GetMousePosition(), restartButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                gameState = MENU;
            }

            EndDrawing();
        }
    }
    UnloadTexture(playerTexture);
    CloseWindow();
  return 0 ;
}