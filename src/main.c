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
    int tileMap[WIDTH_TILE_COUNT*HEIGHT_TILE_COUNT] = {
        1,1,1,1,1,1,1,1,1,1,
        1,0,0,1,1,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,1,0,0,0,0,0,1,
        1,0,0,0,0,0,0,1,1,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,1,1,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,2,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,1,1
    };
    

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
                Vector2 oldVelo = velo;
                velo = Vector2Subtract(GetMousePosition(), startLinePos);
                velo = Vector2Scale(velo, 5.0);
                // velo = Vector2Add(oldVelo, velo);
                //speed = Vector2Normalize(speed);
                printf("realsed! (%f, %f) (%f, %f)\n", velo.x, velo.y, oldVelo.x, oldVelo.y);
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

        // boxPosition = GetMousePosition();

        // Vector2 deltaSpeed = {speed.x * GetFrameTime(), speed.y * GetFrameTime()};

        // boxPosition = Vector2Add(boxPosition, deltaSpeed);
        // boxPosition.y += (gravity/5.0)*GetFrameTime();
        // if (boxPosition.y > screenHeight - 100) boxPosition.y = screenHeight - 100;

        // float speedLength = Vector2Length(speed);
        // if (!FloatEquals(speedLength, 0.0)) {
        //     float newSpeedLength = fmax(speedLength - gravity*GetFrameTime(), 0);
        //     speed = Vector2Scale(speed, newSpeedLength / speedLength);
        // }
        
        // speed.x -= velo.x * friction;

        Vector2 oldVelo = velo;
        velo = Vector2Add(velo, accel);
        boxPosition = Vector2Add(boxPosition, Vector2Scale(Vector2Add(velo, Vector2Scale(accel, 0.5)), GetFrameTime())); 
        // printf("curr (%f, %f) last (%f, %f)\n\n", boxPosition.x, boxPosition.y, lastBoxPosition.x, lastBoxPosition.y);
        // Vec2Int lastLeftTopCorner = {lastBoxPosition.x/tileSize, lastBoxPosition.y/tileSize};
        // Vec2Int lastRightTopCorner = {(lastBoxPosition.x+tileSize)/tileSize, lastBoxPosition.y/tileSize};
        // Vec2Int lastLeftBottomCorner = {lastBoxPosition.x/tileSize, (lastBoxPosition.y+tileSize)/tileSize};
        // Vec2Int lastRightBottomCorner = {(lastBoxPosition.x+tileSize)/tileSize, (lastBoxPosition.y+tileSize)/tileSize};

        // Vec2Int leftTopCorner = {boxPosition.x/tileSize, boxPosition.y/tileSize};
        // Vec2Int rightTopCorner = {(boxPosition.x+tileSize)/tileSize, boxPosition.y/tileSize};
        // Vec2Int leftBottomCorner = {boxPosition.x/tileSize, (boxPosition.y+tileSize)/tileSize};
        // Vec2Int rightBottomCorner = {(boxPosition.x+tileSize)/tileSize, (boxPosition.y+tileSize)/tileSize};

        // 1. Define a tiny value to prevent the box from bleeding into adjacent tiles
        float epsilon = 0.001f;

        // 2. Apply epsilon ONLY to the right (x + tileSize) and bottom (y + tileSize) calculations
        Vec2Int lastLeftTopCorner = {lastBoxPosition.x/tileSize, lastBoxPosition.y/tileSize};
        Vec2Int lastRightTopCorner = {(lastBoxPosition.x+tileSize-epsilon)/tileSize, lastBoxPosition.y/tileSize};
        Vec2Int lastLeftBottomCorner = {lastBoxPosition.x/tileSize, (lastBoxPosition.y+tileSize-epsilon)/tileSize};
        Vec2Int lastRightBottomCorner = {(lastBoxPosition.x+tileSize-epsilon)/tileSize, (lastBoxPosition.y+tileSize-epsilon)/tileSize};

        Vec2Int leftTopCorner = {boxPosition.x/tileSize, boxPosition.y/tileSize};
        Vec2Int rightTopCorner = {(boxPosition.x+tileSize-epsilon)/tileSize, boxPosition.y/tileSize};
        Vec2Int leftBottomCorner = {boxPosition.x/tileSize, (boxPosition.y+tileSize-epsilon)/tileSize};
        Vec2Int rightBottomCorner = {(boxPosition.x+tileSize-epsilon)/tileSize, (boxPosition.y+tileSize-epsilon)/tileSize};

        // printf("curr (%f, %f)(%f, %f) \n", boxPosition.x, boxPosition.y, lastBoxPosition.x, lastBoxPosition.y);
        // printf("curr (%i, %i)(%i, %i) \n", leftTopCorner.x, leftTopCorner.y, lastLeftTopCorner.x, lastLeftTopCorner.y);

        int leftTopCollision = checkTileCollision(leftTopCorner, 1, tileMap);
        int leftTopEqualsLast = lastLeftTopCorner.y == leftTopCorner.y;
        int bottomSideLeftCollision = leftTopCollision && !leftTopEqualsLast; 
        int leftSideTopCollision = leftTopCollision && leftTopEqualsLast;

        int leftBottomCollision = checkTileCollision(leftBottomCorner, 1, tileMap);
        int leftBottomEqualsLast = lastLeftBottomCorner.y == leftBottomCorner.y;
        int topSideLeftCollision = leftBottomCollision && !leftBottomEqualsLast;
        int leftSideBottomCollision = leftBottomCollision && leftBottomEqualsLast;

        int rightTopCollision = checkTileCollision(rightTopCorner, 1, tileMap);
        int rightTopEqualsLast = lastRightTopCorner.y == rightTopCorner.y;
        int bottomSideRightCollision = rightTopCollision && !rightTopEqualsLast;
        int rightSideTopCollision = rightTopCollision && rightTopEqualsLast;

        int rightBottomCollision = checkTileCollision(rightBottomCorner, 1, tileMap);
        int rightBottomEqualsLast = lastRightBottomCorner.y == rightBottomCorner.y;
        int topSideRightCollision = rightBottomCollision && !rightBottomEqualsLast; 
        int rightSideBottomCollision = rightBottomCollision && rightBottomEqualsLast; 

        if (leftTopCollision || leftBottomCollision || rightBottomCollision || rightTopCollision) {
            Vector2 dir = Vector2Normalize((Vector2){boxPosition.x-lastBoxPosition.x, boxPosition.y - lastBoxPosition.y});
            Vector2 lastStep = lastBoxPosition;
            Vector2 newPos = lastStep;
            while(checkRectCollision(newPos, 1, tileSize, tileMap) == 0) {
                lastStep = newPos;
                newPos = Vector2Add(lastStep, Vector2Scale(dir, 0.1));
                // printf("step\n");
            }

            boxPosition = lastStep;
            printf("(%f, %f) (%f, %f)\n", lastStep.x, lastStep.y, lastBoxPosition.x, lastBoxPosition.y);
            velo = (Vector2) {0,0};
        }
        // if (leftSideBottomCollision || leftSideTopCollision) {
        //      printf("LEFT SIDE!\n");
        //     boxPosition.x = rightTopCorner.x*tileSize+1;
        // }

        // if (rightSideBottomCollision || rightSideTopCollision) {
        //     // printf("RIGHT SIDE!\n");
        //     boxPosition.x = leftTopCorner.x*tileSize-0.001;
        //     velo = (Vector2) {-0.5*velo.x, velo.y};
        //     if (abs(0 - velo.y) < 50.0) velo.y = 0;
        // }


        // if (bottomSideLeftCollision || bottomSideRightCollision) {
        //     boxPosition.y = leftBottomCorner.y*tileSize+0.001;
        //     // printf("BOTTOM SIDE!\n");
        //     velo = (Vector2) {velo.x*friction, 0};
        //     if (abs(0 - velo.x) < 50.0) velo.x = 0;
        // }

        // if (topSideLeftCollision || topSideRightCollision) {
        //     boxPosition.y = leftTopCorner.y*tileSize-0.001;
        //     // printf("TOP SIDE!\n");
        //     velo = (Vector2) {velo.x*friction, 0};
        //     if (abs(0 - velo.x) < 50.0) velo.x = 0;
        // }


        // if (checkTileCollision(leftBottomCorner, 1, tileMap) || checkTileCollision(rightBottomCorner, 1, tileMap)){ 
        //     boxPosition.y = leftTopCorner.y*tileSize;
        //     velo = (Vector2) {velo.x*friction, 0};
        //     if (abs(0 - velo.x) < 50.0) velo.x = 0;
        // }

        // if (checkTileCollision(leftTopCorner, 1, tileMap) || checkTileCollision(rightTopCorner, 1, tileMap)){ 
        //     boxPosition.y = leftBottomCorner.y*tileSize;
        //     velo = (Vector2) {velo.x*friction, 0};
        //     if (abs(0 - velo.x) < 50.0) velo.x = 0;
        // }
        
        // if (checkTileCollision(leftBottomCorner, 1, tileMap) || checkTileCollision(leftTopCorner, 1, tileMap)){ 
        //     boxPosition.x = rightTopCorner.x*tileSize;
        //     velo = (Vector2) {velo.x*friction, 0};
        //     if (abs(0 - velo.x) < 50.0) velo.x = 0;
        // }

        // if (checkTileCollision(rightTopCorner, 1, tileMap) || checkTileCollision(rightBottomCorner, 1, tileMap)){ 
        //     boxPosition.x = leftBottomCorner.x*tileSize;
        //     velo = (Vector2) {velo.x*friction, 0};
        //     if (abs(0 - velo.x) < 50.0) velo.x = 0;
        // }
        
        
        // int offset = 30;
        // if (boxPosition.y > screenHeight - offset) {
        //     boxPosition.y = screenHeight - offset;
        //     velo = (Vector2) {velo.x*friction, -1*(velo.y*0.8)};
        //     if (abs(0 - velo.x) < 50.0) velo.x = 0;
        // }

        //printf("%i, %i\n", leftCorner.x, leftCorner.y);


        lastBoxPosition = boxPosition;
        // printf("velo: (%f, %f) (%f, %f)\n", velo.x, velo.y, oldVelo.x, oldVelo.y);
        // printf("mouse: (%f, %f)\n", GetMousePosition().x, GetMousePosition().y);

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

                    DrawRectangle(xPos, yPos, tileSize, tileSize, tileColor);
                }
            }

            DrawRectangle(boxPosition.x, boxPosition.y, tileSize, tileSize, MAROON);
            if (isLeftMouseButtonPressed) {
                DrawLineEx(startLinePos, GetMousePosition(), 3*(lineColor+1), colorMap[lineColor]);
            }


            DrawText("Use mouse wheel to move the cube up and down!", 10, 10, 20, GRAY);
            DrawText(TextFormat("friction: %f", friction), 10, 40, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}