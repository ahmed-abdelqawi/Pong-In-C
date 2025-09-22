// Pong in C, C20x.

#include "raylib.h"
#include <math.h>

int main(){
    // Window.
    const int screenWidth = 800;
    const int screenHight = 600;
    InitWindow(screenWidth, screenHight, "Pong in raylib.");

    // Player 1 (Human).
    Vector2 p1Size = {10.0f, 75.0f};
    Vector2 p1Position = {750, (screenHight / 2) - (p1Size.y / 2)};
    float p1Speed = 5.5f;
    int p1Score = 0;
    bool winner1 = false;

    // Player 2 (Dumb AI).
    Vector2 p2Size = {10.0f, 75.0f};
    Vector2 p2Position = {50, (screenHight / 2) - (p2Size.y / 2)};
    float p2Speed = 5.5f;
    int p2Score = 0;
    bool winner2 = false;

    // Ball.
    Vector2 ballSize = {15.0f, 15.0f};
    Vector2 ballPosition = {(screenWidth / 2) - (ballSize.x / 2), (screenHight / 2)};
    Vector2 ballSpeed = {8.0f, 8.0f};

    // Line.
    Vector2 lineStartP = {(screenWidth / 2), 0};
    Vector2 lineEndP = {(screenWidth / 2), screenHight};

    // Game Loop.
    SetTargetFPS(60);
    bool gameOver = false;
    while (!WindowShouldClose()){
        //---- Game Logic ----

        if (!gameOver){
            // Player1 Input (Human).
            if (IsKeyDown(KEY_UP)) p1Position.y -= p1Speed;
            if (IsKeyDown(KEY_DOWN)) p1Position.y += p1Speed;

            // Bounds Checking Player1 (Human).
            if (p1Position.y < 0) p1Position.y = 0;
            if (p1Position.y + p1Size.y > screenHight) p1Position.y = screenHight - p1Size.y;

            // Player2 (Dumb AI)
            if (GetRandomValue(0, 100) < 90) {
                // 90% chance AI reacts, 10% chance does nothing --> it was hard af to beat (lol).

                float p2Center = p2Position.y + p2Size.y / 2;
                float ballC = ballPosition.y + ballSize.y / 2;

                // Dead zone: don't move if already close enough
                float threshold = 15.0f;

                if (ballSpeed.x < 0) {
                    int error = GetRandomValue(-15, 15); // adds imperfection
                    ballC += error;

                    if (ballC < p2Center - threshold) p2Position.y -= p2Speed;
                    else if (ballC > p2Center + threshold) p2Position.y += p2Speed;
                }

                // Bounds Checking Player2 (Dumb AI).
                if (p2Position.y < 0) p2Position.y = 0;
                if (p2Position.y + p2Size.y > screenHight) p2Position.y = screenHight - p2Size.y;

            }
            
            // Ball.
            ballPosition.x += ballSpeed.x;
            ballPosition.y += ballSpeed.y;

            //if (ballPosition.y <= 0 || ballPosition.y + ballSize.y >= screenHight) ballSpeed.y *= -1;

            // Top wall
            if (ballPosition.y <= 0) {
                ballPosition.y = 0;   // snap inside
                ballSpeed.y *= -1;
            }

            // Bottom wall
            if (ballPosition.y + ballSize.y >= screenHight) {
            ballPosition.y = screenHight - ballSize.y; // snap inside
                ballSpeed.y *= -1;
            }

            if (ballPosition.x < p2Position.x - 50){
                p1Score ++;
                ballPosition = (Vector2){(screenWidth / 2) - (ballSize.x / 2), (screenHight / 2)};
                ballSpeed = (Vector2){8.0f, 8.0f};
            }
            if (ballPosition.x > p1Position.x + 50){
                p2Score ++;
                ballPosition = (Vector2){(screenWidth / 2) - (ballSize.x / 2), (screenHight / 2)};
                ballSpeed = (Vector2){-8.0f, 8.0f}; // Launching the ball to the other player.
            }

            // Collisions.
            Rectangle player1 = {p1Position.x, p1Position.y, p1Size.x, p1Size.y};
            Rectangle player2 = {p2Position.x, p2Position.y, p2Size.x, p2Size.y};
            Rectangle ball = {ballPosition.x, ballPosition.y, ballSize.x, ballSize.y};

            if (CheckCollisionRecs(ball, player1)) {
                // Where did it hit the paddle?
                float paddleCenter = p1Position.y + p1Size.y / 2;
                float ballCenter = ballPosition.y + ballSize.y / 2;
                float hitPos = (ballCenter - paddleCenter) / (p1Size.y / 2);

                // Change direction and adjust angle
                ballSpeed.x = -fabs(ballSpeed.x);  // always go left after hitting right paddle
                ballSpeed.y = hitPos * 5;          // adjust vertical speed

                // Push ball outside paddle (so it doesn't get stuck)
                ballPosition.x = p1Position.x - ballSize.x;
            }

            if (CheckCollisionRecs(ball, player2)) {
                float paddleCenter = p2Position.y + p2Size.y/2;
                float ballCenter = ballPosition.y + ballSize.y/2;
                float hitPos = (ballCenter - paddleCenter) / (p2Size.y/2);

                ballSpeed.x = fabs(ballSpeed.x);   // always go right after hitting left paddle
                ballSpeed.y = hitPos * 5;

                // Push ball outside paddle
                ballPosition.x = p2Position.x + p2Size.x;
            }

            // Game Over State.
            if (p1Score == 10){
                winner1 = true;
                gameOver = true;
            }
            if (p2Score == 10){
                winner2 = true;
                gameOver = true;
            }
        }

        // Resetting the Game.
        if (gameOver && IsKeyDown(KEY_ENTER)){
            p1Score = 0;
            p2Score = 0;

            winner1 = false;
            winner2 = false;
            gameOver = false;

            p1Position = (Vector2){750, (screenHight / 2) - (p1Size.y / 2)};
            p2Position = (Vector2){50, (screenHight / 2) - (p2Size.y / 2)};
            ballPosition = (Vector2){(screenWidth / 2) - (ballSize.x / 2), (screenHight / 2)};
            ballSpeed = (Vector2){5.0f, 5.0f};
        }

        //---- Drawing ----
        BeginDrawing();

        ClearBackground(BLACK);
        DrawLineV(lineStartP, lineEndP, RAYWHITE);

       // Players.
        DrawRectangleV(p1Position, p1Size, RAYWHITE);
        DrawText(TextFormat("%i", p2Score), screenWidth / 4, 20, 40, RAYWHITE);
        if (winner1){
            DrawText("Player 1 Wins! Press ENTER", screenWidth / 2 - 180, screenHight / 2 - 100, 30, RAYWHITE);
        }

        DrawRectangleV(p2Position, p2Size, RAYWHITE);
        DrawText(TextFormat("%i", p1Score), screenWidth * 3/4, 20, 40, RAYWHITE);
        if (winner2){
            DrawText("Player 2 Wins! Press ENTER", screenWidth / 2 - 180, screenHight / 2 - 100, 30, RAYWHITE);
        }

        // Ball.
        DrawRectangleV(ballPosition, ballSize, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}