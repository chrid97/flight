#include "raylib.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  int16_t x;
  int16_t y;
  int16_t dx;
  int16_t dy;
  int16_t width;
  int16_t height;
  int16_t attackCooldown;
  int16_t parryCooldown;
  Color color;
} Entity;

bool CheckCollision(Entity a, Entity b) {
  return (a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height &&
          a.y + a.height > b.y);
}

int main(int argc, char *argv[]) {
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Window");
  SetTargetFPS(60);

  Entity player = {.x = 10,
                   .y = screenHeight - 90,
                   .dx = 2.5,
                   .dy = 2.5,
                   .width = 50,
                   .height = 100,
                   .attackCooldown = 0,
                   .parryCooldown = 0,
                   .color = ORANGE};
  Entity enemy = {
      .x = screenWidth - 100,
      .y = screenHeight - 100,
      .dx = 0,
      .dy = 0,
      .width = 50,
      .height = 100,
      .attackCooldown = 0,
  };
  int enemyAttacking = 0;
  int resting = 120;
  Entity enemyWeapon = {
      .x = enemy.x - 100,
      .y = enemy.y + 50,
      .dx = 1,
      .dy = 1,
      .width = 0,
      .height = 0,
      .attackCooldown = 0,
  };

  // use the same buffer for this. Probably some debug buffer for all of the
  // debug text.
  char buffer[100];
  char xbuffer[100];
  // (TODO) I have to despawn destroyed entities, whether its an enemy that
  // died/left the screen or a bullet that hit an enemy or went off screen
  Entity entities[10000];
  int entitiesLength = 0;

  char buff[100];
  while (!WindowShouldClose()) {

    // Input //

    // if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
    //     player.y -= player.dy;
    // }

    // if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
    //     player.y += player.dy;
    // }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
      player.x -= player.dx;
    }

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
      player.x += player.dx;
    }

    if (IsKeyDown(KEY_SPACE) && player.parryCooldown == 0) {
      player.parryCooldown = 15;
      player.color = RED;
    }

    if (player.parryCooldown > 0) {
      player.parryCooldown--;
      if (player.parryCooldown == 0) {
        player.color = GOLD;
      }
    }

    // Update
    DrawText(buff, screenWidth - 150, 90, 20, RED);
    for (int i = 0; i < entitiesLength; i++) {
      Entity *entity = &entities[i];
      entity->y -= entity->dy;
      snprintf(buff, 100, "Entity y: %i", entity->y);
    }

    // Draw //
    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangle(player.x, player.y, player.width, player.height,
                  player.color);

    // for (int i = 0; i < entitiesLength; i++) {
    //   Entity entity = entities[i];
    //   DrawRectangle(entity.x, entity.y, entity.width, entity.height,
    //   entity.color);
    // }

    if (resting > 0) {
      resting--;
      enemyWeapon.width = 0;
      enemyWeapon.height = 0;
    }

    if (resting == 0) {
      enemyAttacking = 120;
    }

    if (enemyAttacking > 0) {
      enemyAttacking--;
      enemyWeapon.width = 100;
      enemyWeapon.height = 10;
    } else {
      resting = 120;
    }

    DrawRectangle(enemy.x, enemy.y, enemy.width, enemy.height, GREEN);
    DrawRectangle(enemyWeapon.x, enemyWeapon.y, enemyWeapon.width,
                  enemyWeapon.height, BROWN);

    // if (CheckCollision(player, enemyWeapon) || CheckCollision(player, enemy))
    // {
    //   player.color = RED;
    // } else {
    //   if (player.color.r == RED.r && player.color.b == RED.b &&
    //       player.color.g == RED.g) {
    //     player.color = ORANGE;
    //   }
    // }

    // Entity ground = {
    //     .y = screenHeight - 50,
    //     .dx = 0,
    //     .x = 0,
    //     .dy = 0,
    //     .width = screenWidth,
    //     .height = 50,
    //     .attackCooldown = 0,
    // };
    // DrawRectangle(ground.x, ground.y, ground.width, ground.height, BROWN);

    // DEBUG //
    snprintf(xbuffer, 100, "X: %i", player.x);
    snprintf(buffer, 100, "Y: %i", player.y);
    DrawText(buffer, screenWidth - 80, 20, 20, RED);
    DrawText(xbuffer, screenWidth - 80, 40, 20, RED);

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
//
//
// im pivoting
// youre going to have a sword and you can parry and maybe dash
// every floor is just 1 room
// your goal is to get to the top floor
// bosses every 5 floors
// only 10 floors
// isometric game? like zelda?
// 2d side scroller?
// beat em up?
//
//
// 2d side view one room - each room is a levels
// core mechanic is parrying
//
//
// You can charge your parry meter and when its full you can maybe dash or
// something. Maybe ranged energy attack that destroys projectiles and pierces
