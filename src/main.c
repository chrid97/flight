#include "raylib.h"
#include "types.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  float x;
  float y;
  float dx;
  float dy;
  float width;
  float height;
  Color color;
  bool onGround;
} Entity;

Entity player = {
    .x = 0,
    .y = 0,
    .dx = 3,
    .dy = 3,
    .width = 32,
    .height = 32,
    .onGround = false,
    .color = WHITE,
};

int main(void) {
  InitWindow(800, 450, "Parry");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    // Input
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
      player.x -= player.dx;
    }

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
      player.x += player.dx;
    }

    // Update
    if (!player.onGround) {
      player.y += 4.5f;
    }

    if (player.y > GetScreenHeight() - player.y) {
      player.onGround = true;
    }

    // Drawing
    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangle(player.x, player.y, player.width, player.height,
                  player.color);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
