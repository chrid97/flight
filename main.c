#include "raylib.h"
#include <stdint.h>
#include <stdio.h>

typedef struct {
  int16_t x;
  int16_t y;
  int16_t dx;
  int16_t dy;
  int16_t width;
  int16_t height;
} Entity;

int main(int argc, char *argv[]) {
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Window");
  SetTargetFPS(60);

  Entity player = {.x = screenWidth / 2,
                   .y = screenHeight - 100,
                   .dx = 3,
                   .dy = 3,
                   .width = 10,
                   .height = 10};

  // use the same buffer for this. Probably some debug buffer for all of the
  // debug text.
  char buffer[100];
  char xbuffer[100];
  Entity entities[100];

  while (!WindowShouldClose()) {
    snprintf(xbuffer, 100, "X: %i", player.x);
    snprintf(buffer, 100, "Y: %i", player.y);
    DrawText(buffer, screenWidth - 80, 20, 20, RED);
    DrawText(xbuffer, screenWidth - 80, 40, 20, RED);

    // Update
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
      if (player.y > (screenHeight / 2) + player.height) {
        player.y -= player.dy;
      }
    }

    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
      if (player.y < (screenHeight - player.height)) {
        player.y += player.dy;
      }
    }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
      if (player.x > 0) {
        player.x -= player.dx;
      }
    }

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
      if (player.x < (screenWidth - player.width)) {
        player.x += player.dx;
      }
    }

    // (TODO) shooter
    if (IsKeyDown(KEY_SPACE)) {
      Entity player = {.x = player.x,
                       .y = screenHeight - 100,
                       .dx = 3,
                       .dy = 3,
                       .width = 10,
                       .height = 10};
    }

    // Draw
    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangle(player.x, player.y, player.width, player.height, ORANGE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}

// add barrel roll / warp forward dash
// you can only play on the bottom half of the screen
// maybe some of the game is spent flying through obstacles instead of shooting
// enemies different levels
