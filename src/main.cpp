/// Author : GhostOne
/// Created Date : 5th May, 2024
/// <summary>
/// Entry point of the game
/// </summary>

#define RAYGUI_IMPLEMENTATION

#include "game-config.h"

#include <raygui.h>
#include <raylib.h>

#include <queue>
#include <chrono>

using namespace std;

void SetupWorld();

class Block
{
public:
    Vector2 index;
    Block(int x, int y)
    {
        this->index = (Vector2){static_cast<float>(x), static_cast<float>(y)};
    }

    Block()
    {
        *this = Block(0, 0);
    }

    void DrawBlock()
    {
        DrawRectangleRec((Rectangle){GAME_WORLD_START_X + CELL_SIZE_X * index.x, GAME_WORLD_START_Y + CELL_SIZE_Y * index.y, CELL_SIZE_X, CELL_SIZE_Y}, WHITE);
        DrawRectangleLines(GAME_WORLD_START_X + CELL_SIZE_X * index.x, GAME_WORLD_START_Y + CELL_SIZE_Y * index.y, CELL_SIZE_X, CELL_SIZE_Y, BLACK);
    }
};

class Food : public Block
{
public:
    Food()
    {
        this->index = (Vector2){static_cast<float>(rand() % 48 + 1.0), static_cast<float>(rand() % 48 + 1.0)};
    }

    void DrawFood()
    {
        DrawRectangleRec((Rectangle){GAME_WORLD_START_X + CELL_SIZE_X * index.x, GAME_WORLD_START_Y + CELL_SIZE_Y * index.y, CELL_SIZE_X, CELL_SIZE_Y}, GREEN);
        DrawRectangleLines(GAME_WORLD_START_X + CELL_SIZE_X * index.x, GAME_WORLD_START_Y + CELL_SIZE_Y * index.y, CELL_SIZE_X, CELL_SIZE_Y, WHITE);
    }
};

class Snake
{
    class SnakeBlock
    {
    public:
        Vector2 index;
        SnakeBlock(int x, int y)
        {
            this->index = (Vector2){static_cast<float>(x), static_cast<float>(y)};
        }

        SnakeBlock()
        {
            *this = SnakeBlock(0, 0);
        }

        void DrawBlock()
        {
            DrawRectangleRounded((Rectangle){GAME_WORLD_START_X + CELL_SIZE_X * index.x, GAME_WORLD_START_Y + CELL_SIZE_Y * index.y, CELL_SIZE_X, CELL_SIZE_Y}, 1, 2, RED);
        }
    };

    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    queue<SnakeBlock> snakeBlocks;
    Direction currentDirection;

public:
    Snake()
    {
        SnakeBlock block1 = SnakeBlock(25, 25);
        SnakeBlock block2 = SnakeBlock(25, 26);
        SnakeBlock block3 = SnakeBlock(25, 27);
        snakeBlocks.push(block3);
        snakeBlocks.push(block2);
        snakeBlocks.push(block1);

        currentDirection = UP;
    }
    void drawSnake()
    {
        for (int i = 0; i < snakeBlocks.size(); i++)
        {
            snakeBlocks.front().DrawBlock();
            snakeBlocks.push(snakeBlocks.front());
            snakeBlocks.pop();
        }
    }

    void moveSnake()
    {
        SnakeBlock head = snakeBlocks.back();
        SnakeBlock newBlock;
        switch (currentDirection)
        {
        case UP:
            newBlock = SnakeBlock(head.index.x, head.index.y - 1);
            break;
        case DOWN:
            newBlock = SnakeBlock(head.index.x, head.index.y + 1);
            break;
        case LEFT:
            newBlock = SnakeBlock(head.index.x - 1, head.index.y);
            break;
        case RIGHT:
            newBlock = SnakeBlock(head.index.x + 1, head.index.y);
            break;
        }
        snakeBlocks.push(newBlock);
        snakeBlocks.pop();
    }

    void controlSnake()
    {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W) && currentDirection != DOWN)
        {
            currentDirection = UP;
        }
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S) && currentDirection != UP)
        {
            currentDirection = DOWN;
        }
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A) && currentDirection != RIGHT)
        {
            currentDirection = LEFT;
        }
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D) && currentDirection != LEFT)
        {
            currentDirection = RIGHT;
        }
    }

    void eatFood(Food &food, int &score)
    {
        SnakeBlock snakeHead = snakeBlocks.back();
        if (snakeHead.index.x == food.index.x && snakeHead.index.y == food.index.y)
        {
            score++;
            SnakeBlock head = snakeBlocks.back();
            SnakeBlock newBlock;
            switch (currentDirection)
            {
            case UP:
                newBlock = SnakeBlock(head.index.x, head.index.y - 1);
                break;
            case DOWN:
                newBlock = SnakeBlock(head.index.x, head.index.y + 1);
                break;
            case LEFT:
                newBlock = SnakeBlock(head.index.x - 1, head.index.y);
                break;
            case RIGHT:
                newBlock = SnakeBlock(head.index.x + 1, head.index.y);
                break;
            }
            snakeBlocks.push(newBlock);
            food = Food();
        }
    }

    void checkCollision()
    {
        SnakeBlock snakeHead = snakeBlocks.back();
        for (int i = 0; i < snakeBlocks.size() - 1; i++)
        {
            if (snakeHead.index.x == snakeBlocks.front().index.x && snakeHead.index.y == snakeBlocks.front().index.y)
            {
                SetupWorld();
                break;
            }
            snakeBlocks.push(snakeBlocks.front());
            snakeBlocks.pop();
        }

        snakeBlocks.push(snakeBlocks.front());
        snakeBlocks.pop();

        if (snakeHead.index.x <= 0 || snakeHead.index.x >= 49 || snakeHead.index.y <= 0 || snakeHead.index.y >= 49)
        {
            SetupWorld();
        }
    }
};

Camera2D camera;
int score = 0;
Block worldWalls[196];
Food food = Food();
Snake snake = Snake();
std::chrono::steady_clock::time_point lastMoveTime = std::chrono::steady_clock::now();
Music backgroundMusic = LoadMusicStream("resources/audio/bg_music.mp3");

void SetupCamera()
{
    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void SetupWorld()
{
    for (int i = 0; i < 50; i++)
    {
        worldWalls[i] = Block(i, 0);
    }
    for (int i = 50; i < 99; i++)
    {
        worldWalls[i] = Block(49, (i + 1) - 50);
    }
    for (int i = 99; i < 148; i++)
    {
        worldWalls[i] = Block(0, (i + 1) - 99);
    }

    for (int i = 148; i < 196; i++)
    {
        worldWalls[i] = Block((i + 1) - 148, 49);
    }

    snake = Snake();
    score = 0;
    lastMoveTime = std::chrono::steady_clock::now();
    Food newFood = Food();
}

void RestartGameOnPressed()
{
    if (IsKeyPressed(KEY_R))
    {
        SetupWorld();
    }
}

void DrawSnakeAndDoSnakeLogic()
{
    BeginMode2D(camera);
    // Get the current time
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    // Calculate the time difference since the last move
    std::chrono::milliseconds timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMoveTime);

    if (timeDiff.count() >= MOVEMENT_REFRESH_RATE)
    {
        snake.checkCollision();
        snake.moveSnake();
        snake.eatFood(food, score);
        lastMoveTime = now;
    }
    snake.drawSnake();
    snake.controlSnake();
    EndMode2D();
}

/// @brief draw the environment of the game
/// @return
void DrawWorld()
{

    BeginMode2D(camera);
    for (int i = 0; i < 196; i++)
    {
        worldWalls[i].DrawBlock();
    }
    // DrawRectangleRec((Rectangle){GAME_WORLD_START_X, GAME_WORLD_START_Y, SCREEN_WIDTH, SCREEN_HEIGHT}, GRAY);
    EndMode2D();
}

/// @brief the 25px header at the top that displays score information
void DrawScoreBoard()
{

    char scoreStr[50];
    sprintf(scoreStr, "Score: %d", score);

    GuiDrawText(scoreStr, (Rectangle){0, 0, 100, 25}, 0, WHITE);
    GuiDrawText("Click R to Restart Game", (Rectangle){150, 0, 200, 25}, 1, WHITE);
    GuiDrawText("GhostOne", (Rectangle){SCREEN_WIDTH - 200, 0, 200, 25}, 2, WHITE);
}

int main()
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");
    InitAudioDevice();
    SetTargetFPS(60);
    SetupCamera();
    SetupWorld();
    PlayMusicStream(backgroundMusic);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawWorld();
        DrawSnakeAndDoSnakeLogic();
        food.DrawFood();
        DrawScoreBoard();
        RestartGameOnPressed();

        DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadMusicStream(backgroundMusic);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}