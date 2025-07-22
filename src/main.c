#include "raylib.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef enum {
  IDLE,
  WALK,
  JUMP,
  ATTACK,
  ATTACK_RECOVERY,
  RUN,
  EVADE,
  FALLING,
  DEATH,
  MOVE_RIGHT,
  MOVE_LEFT,
  PARRY
} State;

typedef struct {
  float x;
  float y;
  float dx;
  float dy;
  float width;
  float height;
  Color color;
  bool onGround;
  State state;
  int attack_cooldown;
  int attack_timer;
  bool hit_registered;
} Entity;

Entity player = {
    .x = 30,
    .y = 450 - 32,
    .dx = 3.5,
    .dy = 3,
    .width = 22,
    .height = 32,
    .onGround = false,
    .color = WHITE,
    .state = IDLE,
};
int parry_timer = 0;

int main(void) {
  Entity enemy = {
      .x = 800 - 62 - 48,
      .y = 450 - 48,
      .dx = 3,
      .dy = 3,
      .width = 48,
      .height = 48,
      .onGround = false,
      .color = PURPLE,
      .state = IDLE,
      .attack_cooldown = 0,
  };

  Entity enemy_weapon = {
      .x = enemy.x - enemy.width + 35,
      .y = enemy.y + (enemy.height / 2) + 10,
      .dx = 3,
      .dy = 3,
      .width = 120,
      .height = 5,
      .onGround = false,
      .color = MAGENTA,
      .state = IDLE,
      .attack_cooldown = 0,
  };
  float rotation = 330;

  InitWindow(800, 450, "Parry");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    if (parry_timer > 0) {
      parry_timer--;
    }

    // Input
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
      if (player.x > 0) {
        player.x -= player.dx;
      }
    }

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
      if (player.x < GetScreenWidth() - player.width) {
        player.x += player.dx;
      }
    }

    if (IsKeyPressed(KEY_SPACE) && parry_timer == 0) {
      parry_timer = 10;
      player.state = PARRY;
    } else if (parry_timer == 0) {
      player.state = IDLE;
    }

    if (player.state == PARRY) {
      player.color = BLUE;
    } else {
      player.color = WHITE;
    }

    // Update
    if (!player.onGround) {
      player.y += 4.5f;
    }

    if (player.y > 450 - player.height) {
      player.onGround = true;
    }

    // int distance = fabsf(enemy.x - player.x);
    // if (enemy.state != ATTACK && enemy.state != ATTACK_RECOVERY) {
    //   if (distance >= 0 && distance <= 100) {
    //     enemy.state = ATTACK;
    //   } else if (distance > 100) {
    //     enemy.state = WALK;
    //   } else {
    //     enemy.state = IDLE;
    //   }
    // }

    switch (enemy.state) {
    case WALK: {
      // if (player.x > enemy.x) {
      //   enemy.x += enemy.dx;
      // } else {
      //   enemy.x -= enemy.dx;
      // }
      // enemy_weapon.x = enemy.x - enemy.width;
      // enemy_weapon.y = enemy.y + (enemy.height / 2);
      break;
    }
    case ATTACK: {
      // if (fabsf(rotation) != 180) {
      //   rotation -= 4;
      // }
      // if (fabsf(rotation) == 180) {
      //   enemy.state = ATTACK_RECOVERY;
      // }
      break;
    }
    case ATTACK_RECOVERY: {
      // rotation += 3.5;
      // if (fabsf(rotation) == 180) {
      //   enemy.state = IDLE;
      // }
      break;
    }
    default:
      enemy.color = PURPLE;
    }

    if (rotation < 175) {
      enemy.state = ATTACK_RECOVERY;
    } else {
      rotation -= 3.5;
    }

    if (enemy.state == ATTACK_RECOVERY) {
      rotation += 3.5;
    }

    // Drawing
    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangle(player.x, player.y, player.width, player.height,
                  player.color);

    DrawRectangle(enemy.x, enemy.y, enemy.width, enemy.height, enemy.color);
    // DrawRectanglePro(
    //     (Rectangle){enemy_weapon.x + 25, enemy_weapon.y - 100, 50, 100},
    //     (Vector2){0, 0}, rotation, enemy_weapon.color);
    DrawRectanglePro(
        (Rectangle){enemy_weapon.x + 90, enemy_weapon.y - 75, 45, 15},
        (Vector2){0, 0}, rotation, enemy_weapon.color);
    DrawRectanglePro((Rectangle){enemy_weapon.x, enemy_weapon.y,
                                 enemy_weapon.width, enemy_weapon.height},
                     (Vector2){0, 0}, rotation, enemy_weapon.color);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
