#include "raylib.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

int main(void) {
  InitWindow(800, 450, "Parry");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    // Input
    // Drawing
    BeginDrawing();
    ClearBackground(BLACK);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
