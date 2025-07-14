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
  // (TODO) I have to despawn destroyed entities, whether its an enemy that
  // died/left the screen or a bullet that hit an enemy or went off screen
  Entity entities[10000];
  int entitiesLength = 0;
  int weaponCooldown = 0;

  char buff[100];
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

    if (IsKeyDown(KEY_SPACE) && weaponCooldown == 0) {
      weaponCooldown = 60;
      Entity player_projectile = {.x = player.x + (player.width / 4),
                                  .y = player.y - 10,
                                  .dx = 5,
                                  .dy = 5,
                                  .width = 5,
                                  .height = 5};

      // proper error message when im out of bounds
      entities[entitiesLength++] = player_projectile;
    }

    DrawText(buff, screenWidth - 150, 90, 20, RED);
    for (int i = 0; i < entitiesLength; i++) {
      Entity *entity = &entities[i];
      entity->y -= entity->dy;
      snprintf(buff, 100, "Entity y: %i", entity->y);
    }

    if (weaponCooldown > 0) {
      weaponCooldown--;
    }

    // Draw
    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangle(player.x, player.y, player.width, player.height, ORANGE);

    for (int i = 0; i < entitiesLength; i++) {
      Entity entity = entities[i];
      DrawRectangle(entity.x, entity.y, entity.width, entity.height, BLUE);
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}

// add barrel roll / warp forward dash
// you can only play on the bottom half of the screen
// maybe some of the game is spent flying through obstacles instead of shooting
// enemies different levels
// map should change so it's harder to navigate sometimes, like the walls will
// close in but to compensate you can move up the map further
// boosters?
// dash or parry mechanic?
// anti matter gun - can instatly vaporize an enemy or player. Denoted by green
// bullet Levels
// - Outerspace
// - On a planet
// - then theres terrain
