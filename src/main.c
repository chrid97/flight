#include "raylib.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Utility function to rotate a point around an origin
Vector2 RotatePoint(Vector2 point, Vector2 origin, float angle) {
  float radians = angle * DEG2RAD;
  float s = sinf(radians);
  float c = cosf(radians);

  // Translate point back to origin
  point.x -= origin.x;
  point.y -= origin.y;

  // Rotate
  float xnew = point.x * c - point.y * s;
  float ynew = point.x * s + point.y * c;

  // Translate back
  point.x = xnew + origin.x;
  point.y = ynew + origin.y;

  return point;
}

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
  int health;
  bool is_attacking;
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
    .health = 10,
    .is_attacking = true,
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
      .health = 10,
      .is_attacking = true,
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

  // delta - how much time has passed since the last frame
  InitWindow(800, 450, "Parry");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    float delta = GetFrameTime();

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
      if (false) {
        if (player.x > enemy.x) {
          enemy.x += enemy.dx;
        } else {
          enemy.x -= enemy.dx;
        }
        enemy_weapon.x = enemy.x - enemy.width;
        enemy_weapon.y = enemy.y + (enemy.height / 2);
      }
      break;
    }
    case ATTACK: {
      break;
    }
    case ATTACK_RECOVERY: {
      break;
    }
    default:
      enemy.color = PURPLE;
    }

    if (enemy.is_attacking) {
      enemy.is_attacking = false;
      enemy.state = ATTACK_RECOVERY;
    } else {
      rotation -= 500 * delta;
    }

    // Weapon base and tip position BEFORE rotation
    Vector2 weapon_origin = (Vector2){enemy_weapon.x, enemy_weapon.y};
    Vector2 weapon_tip =
        (Vector2){enemy_weapon.x + enemy_weapon.width, enemy_weapon.y};

    // Rotate tip around the origin
    Vector2 rotated_tip = RotatePoint(weapon_tip, weapon_origin, rotation);

    // Visualize tip hitbox
    DrawCircleV(rotated_tip, 5, RED);

    // Hit detection (example: circular hitbox)
    float hit_radius = 5.0f;
    Vector2 player_center =
        (Vector2){player.x + player.width / 2, player.y + player.height / 2};
    bool hit = CheckCollisionCircles(rotated_tip, hit_radius, player_center,
                                     player.width / 2);

    if (hit) {
      DrawText("HIT!", 10, 10, 20, RED);
    }

    if (enemy.state == ATTACK_RECOVERY) {
      rotation = 0;
      enemy.state = IDLE;
    }

    if (enemy.state == IDLE) {
      // printf("%i", enemy.state);
      enemy.state = ATTACK;
    }
    printf("%f\n", enemy_weapon.y);

    // if (enemy.state == ATTACK_RECOVERY) {
    //   rotation += 3.5;
    // }

    // printf("%i\n", enemy_weapon.y);

    // Drawing
    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangle(player.x, player.y, player.width, player.height,
                  player.color);

    DrawRectangle(enemy.x, enemy.y, enemy.width, enemy.height, enemy.color);
    // DrawRectanglePro(
    //     (Rectangle){enemy_weapon.x + 25, enemy_weapon.y - 100, 50, 100},
    //     (Vector2){0, 0}, rotation, enemy_weapon.color);
    // DrawRectanglePro(
    //     (Rectangle){enemy_weapon.x + 90, enemy_weapon.y - 75, 45, 15},
    //     (Vector2){0, 0}, rotation, enemy_weapon.color);
    DrawRectanglePro((Rectangle){enemy_weapon.x, enemy_weapon.y,
                                 enemy_weapon.width, enemy_weapon.height},
                     (Vector2){0, 0}, rotation, enemy_weapon.color);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
