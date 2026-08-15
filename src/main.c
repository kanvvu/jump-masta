/*******************************************************************************************
*
*   raylib [core] example - input mouse wheel
*
*   Example complexity rating: [★☆☆☆] 1/4
*
*   Example originally created with raylib 1.1, last time updated with raylib 1.3
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2025 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include "stdio.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
#define WIDTH_TILE_COUNT 10
#define HEIGHT_TILE_COUNT 15

struct _Vec2Int {
    int x;
    int y;
} typedef Vec2Int;




int checkTileCollision(Vec2Int pos, int tileType, int tileMap[] ) {
    if (pos.x >= WIDTH_TILE_COUNT || pos.x < 0 || pos.y >= HEIGHT_TILE_COUNT || pos.y < 0) {
        printf("COLLSION POSITION OUT OF BOUNDS!\n");
        return 0;
    }

    if (tileMap[pos.y*WIDTH_TILE_COUNT + pos.x] == tileType) return 1;
    
    
    return 0;
}

int checkRectCollision(Vector2 pos, int tileType, int tileSize, int tileMap[]) {
    Vec2Int leftTopCorner = {pos.x/tileSize, pos.y/tileSize};
    Vec2Int rightTopCorner = {(pos.x+tileSize)/tileSize, pos.y/tileSize};
    Vec2Int leftBottomCorner = {pos.x/tileSize, (pos.y+tileSize)/tileSize};
    Vec2Int rightBottomCorner = {(pos.x+tileSize)/tileSize, (pos.y+tileSize)/tileSize};

    if (checkTileCollision(leftTopCorner, tileType, tileMap)) return 1;
    if (checkTileCollision(rightTopCorner, tileType, tileMap)) return 1;
    if (checkTileCollision(rightBottomCorner, tileType, tileMap)) return 1;
    if (checkTileCollision(leftBottomCorner, tileType, tileMap)) return 1;

    return 0;
}

int checkPointCollision(Vector2 pos1, Vector2 pos2, int size2) {
    if (pos1.x >= pos2.x && pos1.x <= pos2.x + size2 && pos1.y >= pos2.y && pos1.y <= pos2.y + size2) return 1;
    return 0;
}

int checkCollision(Vector2 pos1, int size1, Vector2 pos2, int size2) {
    if (pos1.y + size1 > pos2.y && pos1.x + size1 > pos2.x && pos1.x < pos2.x + size2 && pos1.y < pos2.y + size2) {
        return 1;
    }
    return 0;
}

int checkCollision2(Vector2 pos1, int size1, Vector2 pos2, int size2) {
    /*      2
          _ _ _
         |     |
     1   |     |  3
         |_ _ _|
          
            4
    */
    Vector2 leftTopCorner = {pos1.x, pos1.y};
    Vector2 rightTopCorner = {pos1.x + size1, pos1.y};
    Vector2 leftBottomCorner = {pos1.x, pos1.y + size1};
    Vector2 rightBottomCorner = {pos1.x + size1, pos1.y + size1};

    float xL = pos2.x + size2 - pos1.x;
    float xR = pos1.x + size1 - pos2.x;
    float yT = pos2.y + size2 - pos1.y;
    float yB = pos1.y + size1 - pos2.y;

    if (checkPointCollision(leftTopCorner, pos2, size2)) {
        if (xL > yT) return 2;
        return 1;
    }
    if (checkPointCollision(rightTopCorner, pos2, size2)) {
        if (xR > yT) return 2;
        return 3;
    }
    if (checkPointCollision(rightBottomCorner, pos2, size2)) {
        if (xR > yB) return 4;
        return 3;
    }
    if (checkPointCollision(leftBottomCorner, pos2, size2)) {
        if (xL > yB) return 4;
        return 1;
    }

    return 0;
}

int main(void)
{
    const int scale = 2;
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800 * scale;
    const int screenHeight = 450 * scale;
    Color colorMap[3] = {BLACK, ORANGE, RED};
    int lineColor = 0;
    int tileSize = 64;
    // int tileMap[WIDTH_TILE_COUNT*HEIGHT_TILE_COUNT] = {
    //     1,1,1,1,1,1,1,1,1,1,
    //     1,0,0,1,1,0,0,0,0,1,
    //     1,0,0,0,0,0,0,0,0,1,
    //     1,0,0,0,0,0,0,0,0,1,
    //     1,0,0,1,0,0,0,0,0,1,
    //     1,0,0,0,0,0,0,1,1,1,
    //     1,0,0,0,0,0,0,0,0,1,
    //     1,0,0,0,0,0,0,0,0,1,
    //     1,0,0,0,0,0,0,0,0,1,
    //     1,1,1,0,0,0,0,0,0,1,
    //     1,0,0,0,0,0,0,0,0,1,
    //     1,0,0,0,0,0,0,0,0,1,
    //     1,0,0,0,2,0,0,0,0,1,
    //     1,0,0,0,0,0,0,0,0,1,
    //     1,1,1,1,1,1,1,1,1,1
    // };
    int tileMap1[WIDTH_TILE_COUNT*HEIGHT_TILE_COUNT] = {
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,1,1,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,1,0,0,0,0,0,1,
        1,0,0,0,0,0,0,1,1,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,1,1,1,0,0,1,1,1,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,2,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,1,1
    };
    int tileMap2[WIDTH_TILE_COUNT*HEIGHT_TILE_COUNT] = {
        1,1,1,1,1,1,1,1,1,1,
        1,0,0,1,1,0,0,0,0,0,
        1,0,0,0,0,0,0,0,0,0,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,1,0,0,0,0,0,1,
        1,0,0,0,0,0,0,1,1,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,1,1,1,1,
        1,0,0,0,0,0,0,0,0,1
    };

    int *tileMap = tileMap1;
    

    // InitWindow(screenWidth, screenHeight, "raylib [core] example - input mouse wheel");
    InitWindow(WIDTH_TILE_COUNT*tileSize, HEIGHT_TILE_COUNT*tileSize, "raylib [core] example - input mouse wheel");

    int isLeftMouseButtonPressed = 0;
    Vector2 startLinePos = {0};

    float gravity = 40;
    Vector2 accel = {0,gravity};
    float friction = 0.9;
    Vector2 velo = {0,0};
    Vector2 boxPosition = {screenWidth/2 - 40, screenHeight - 500};
    Vector2 lastBoxPosition = boxPosition;
    for (int i=0; i<WIDTH_TILE_COUNT*HEIGHT_TILE_COUNT; i++) {
        if (tileMap[i] == 2) {
            int yPos = i/WIDTH_TILE_COUNT;
            int xPos = i%WIDTH_TILE_COUNT;
            boxPosition = (Vector2) {xPos*tileSize, yPos*tileSize};
        }
    }
    int scrollSpeed = 4;            // Scrolling speed in pixels
    int onGround = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        friction -= (GetMouseWheelMove()*0.01);
        Vector2 mousePos = GetMousePosition();
        float currFriction = -1*friction;
        if (velo.x < 0) currFriction = friction;

        // accel = (Vector2) {currFriction, gravity};
        // if (IsKeyDown(KEY_A)) speed.x = -100;
        // if (IsKeyDown(KEY_D)) speed.x = 100;

        if (IsMouseButtonDown(0) && !isLeftMouseButtonPressed)
        {
            isLeftMouseButtonPressed = true;
            startLinePos = GetMousePosition();
        }

        if (!IsMouseButtonDown(0)) {
            if (isLeftMouseButtonPressed) {
                if (onGround) {
                    velo = Vector2Subtract(GetMousePosition(), startLinePos);
                    velo = Vector2Scale(velo, 5.0);
                    printf("realsed! (%f, %f)\n", velo.x, velo.y);
                }
            }
            isLeftMouseButtonPressed = false;
        }

        if (isLeftMouseButtonPressed){
            float lineLength = Vector2Length(Vector2Subtract(GetMousePosition(), startLinePos)); 
            //printf("%f\n", lineLength);

            if (lineLength < 300.0) lineColor = 2;
            if (lineLength < 200.0) lineColor = 1;
            if (lineLength < 100.0) lineColor = 0;
        }

        if (boxPosition.y + tileSize < 0) {
            tileMap = tileMap2;
            // boxPosition.y -= HEIGHT_TILE_COUNT*tileSize - tileSize;
            boxPosition.y = (HEIGHT_TILE_COUNT-1)*tileSize;
        }

        if (boxPosition.y > HEIGHT_TILE_COUNT*tileSize) {
            tileMap = tileMap1;
            boxPosition.y = 0;
        }
        
        
        // Vector2 newBoxPosition = GetMousePosition();

        // Vector2 newBoxPosition =  boxPosition;

        // if (IsKeyDown(KEY_W)) newBoxPosition.y -= 10;
        // if (IsKeyDown(KEY_A)) newBoxPosition.x -= 10;
        // if (IsKeyDown(KEY_S)) newBoxPosition.y += 10;
        // if (IsKeyDown(KEY_D)) newBoxPosition.x += 10;
        
        if (onGround) velo.x = 0.8*velo.x;
        if (abs(velo.x) < 0.1) velo.x = 0;


        velo = Vector2Add(velo, accel);
        Vector2 newBoxPosition = Vector2Add(boxPosition, Vector2Scale(Vector2Add(velo, Vector2Scale(accel, 0.5)), GetFrameTime())); 

        Vector2 bP = {boxPosition.x/tileSize, boxPosition.y/tileSize};
        Vector2 nBP = {newBoxPosition.x/tileSize, newBoxPosition.y/tileSize};
        
        // velo = Vector2Subtract(nBP, bP);

        if (velo.x <= 0) { //left
            if (checkTileCollision((Vec2Int){nBP.x, bP.y}, 1, tileMap) ||
                checkTileCollision((Vec2Int){nBP.x, bP.y + 0.99}, 1, tileMap)) {
               nBP.x = (int)nBP.x + 1;
               newBoxPosition.x = nBP.x * tileSize; 
            //    velo.x = 0;
               velo.x = -0.5*velo.x;
            }
        }
        else {  //right
            if (checkTileCollision((Vec2Int){nBP.x + 1.0, bP.y}, 1, tileMap) ||
                checkTileCollision((Vec2Int){nBP.x + 1.0, bP.y + 0.99}, 1, tileMap)) {
               nBP.x = (int)nBP.x;
               newBoxPosition.x = nBP.x * tileSize; 
            //    velo.x = 0;
               velo.x = -0.7*velo.x;
            }
        }

        onGround = 0;
        if (velo.y <= 0) { //up
            if (checkTileCollision((Vec2Int){nBP.x, nBP.y}, 1, tileMap) ||
                checkTileCollision((Vec2Int){nBP.x+0.99, nBP.y}, 1, tileMap)) {
               nBP.y = (int)nBP.y + 1;
               newBoxPosition.y = nBP.y * tileSize; 
               velo.y = -0.5*velo.y;
            }
        }
        else { //down
            if (checkTileCollision((Vec2Int){nBP.x, nBP.y + 1.0}, 1, tileMap) ||
                checkTileCollision((Vec2Int){nBP.x + 0.99, nBP.y + 1.0}, 1, tileMap)) {
               nBP.y = (int)nBP.y;
               newBoxPosition.y = nBP.y * tileSize; 
               velo.y = 0;
            //    velo.x = 0;
               onGround = 1;
            }
        }


        boxPosition = newBoxPosition;

        lastBoxPosition = boxPosition;

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            //tiles
            for(int i=0; i<HEIGHT_TILE_COUNT; i++) {
                for(int j=0; j<WIDTH_TILE_COUNT; j++) {
                    int tileIndex = i*WIDTH_TILE_COUNT + j;
                    int xPos = j*tileSize;
                    int yPos = i*tileSize;
                    Color tileColor = WHITE;

                    if (tileMap[tileIndex] == 1) tileColor = BLUE;
                    if (tileMap[tileIndex] == 3) tileColor= ORANGE;

                    DrawRectangle(xPos, yPos, tileSize, tileSize, tileColor);
                    char msg1[64];
                    snprintf(msg1, sizeof(msg1), "%i, %i", j, i);
                    char msg2[64];
                    snprintf(msg2, sizeof(msg2), "%i, %i", xPos, yPos);
                    // DrawLineEx((Vector2) {xPos, yPos}, (Vector2) {xPos + tileSize, yPos}, 1, GRAY);
                    // DrawLineEx((Vector2) {xPos, yPos}, (Vector2) {xPos, yPos + tileSize}, 1, GRAY);
                    // DrawLineEx((Vector2) {xPos+tileSize, yPos}, (Vector2) {xPos + tileSize, yPos + tileSize}, 1, GRAY);
                    // DrawLineEx((Vector2) {xPos, yPos+tileSize}, (Vector2) {xPos + tileSize, yPos+ tileSize}, 1, GRAY);
                    // DrawText(msg1, xPos, yPos, 5, BLACK);
                    // DrawText(msg2, xPos, yPos+10, 5, BLACK);
                }
            }

            char msg1[64];
            snprintf(msg1, sizeof(msg1), "%i, %i", (int) boxPosition.x/tileSize, (int) boxPosition.y/tileSize);
            char msg2[64];
            snprintf(msg2, sizeof(msg2), "%.2f\n%.2f", boxPosition.x, boxPosition.y);
            char msg3[64];
            snprintf(msg3, sizeof(msg3), "%.2f, %.2f", velo.x, velo.y);
            DrawRectangle(boxPosition.x, boxPosition.y, tileSize, tileSize, MAROON);
            // if (colType == 1) DrawLineEx((Vector2) {boxPosition.x, boxPosition.y}, (Vector2) {boxPosition.x, boxPosition.y + tileSize}, 6, ORANGE);
            // if (colType == 2) DrawLineEx((Vector2) {boxPosition.x, boxPosition.y}, (Vector2) {boxPosition.x+tileSize, boxPosition.y}, 6, ORANGE);
            // if (colType == 3) DrawLineEx((Vector2) {boxPosition.x+tileSize, boxPosition.y}, (Vector2) {boxPosition.x+tileSize, boxPosition.y+tileSize}, 6, ORANGE);
            // if (colType == 4) DrawLineEx((Vector2) {boxPosition.x, boxPosition.y+tileSize}, (Vector2) {boxPosition.x+tileSize, boxPosition.y+tileSize}, 6, ORANGE);
            // printf("HUUUJ %i\n", colType);
            // DrawText(msg1, boxPosition.x, boxPosition.y, 18, BLACK);
            // DrawText(msg2, boxPosition.x, boxPosition.y+20, 18, BLACK);
            DrawText(msg3, boxPosition.x, boxPosition.y-20, 18, BLACK);
            if (isLeftMouseButtonPressed) {
                DrawLineEx(startLinePos, GetMousePosition(), 3*(lineColor+1), colorMap[lineColor]);
            }


            DrawText("Use mouse wheel to move the cube up and down!", 10, 10, 20, GRAY);
            DrawText(TextFormat("friction: %f.2f", friction), 10, 40, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}