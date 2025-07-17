#include "raylib.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum PlayerState { IDLE, WALK } PlayerState;

typedef struct Entity {
  int16_t x;
  int16_t y;
  int16_t dx;
  int16_t dy;
  int16_t width;
  int16_t height;
  int16_t attackCooldown;
  int16_t parryCooldown;
  PlayerState state;
  bool facingRight;
  Color color;
} Entity;

bool checkCollision(Entity a, Entity b) {
  return (a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height &&
          a.y + a.height > b.y);
}

int main(int argc, char *argv[]) {
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Window");
  SetTargetFPS(60);

  Entity player = {.x = 0,
                   .y = 0,
                   .dx = 3,
                   .dy = 3,
                   .width = 32,
                   .height = 32,
                   .attackCooldown = 0,
                   .parryCooldown = 0,
                   .facingRight = true,
                   .state = IDLE,
                   .color = ORANGE};

  // IDLE animation
  Vector2 origin = {0, 0};
  Texture2D playerIdle = LoadTexture("./assets/player/IDLE.png");
  Texture2D playerWalk = LoadTexture("./assets/player/WALK.png");
  // pixel widht is supposed to be 32 px but this seems like a better size,
  // maybe other animations use more the screen
  // i feel like i should combine the sprite sheets so i only have to load 1
  // file, at least for each char
  // i wonder if it makes sense to change the size of my hitbox depending on
  // what im doing int frameWidth = 21;

  int frameWidth = 32;
  int frameHeight = 32;
  int maxFrames = 10;
  float scale = 3.0f;
  Rectangle sourceRec = {900.0f, 1200.0f, frameWidth,
                         frameHeight}; // first frame
  Rectangle destRec = {player.x, player.y, frameWidth * scale,
                       frameHeight * scale};
  int currentFrame = 0;
  int frameCounter = 0;
  int framesSpeed = 10; // how many game frames per animation frame

  while (!WindowShouldClose()) {
    // Input //
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
      player.x -= player.dx;
      player.state = WALK;
      player.facingRight = false;
    } else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
      player.x += player.dx;
      player.state = WALK;
      player.facingRight = true;
    } else {
      player.state = IDLE;
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
    destRec.x = player.x;
    destRec.y = player.y;

    sourceRec.width = player.facingRight ? frameWidth : -frameWidth,

    frameCounter++;
    if (frameCounter >= (60 / framesSpeed)) {
      frameCounter = 0;
      sourceRec.x += 96;
    }

    // sourceRec.x

    // Draw //
    BeginDrawing();
    ClearBackground(BLACK);
    switch (player.state) {
    case IDLE:
      DrawTexturePro(playerIdle, sourceRec, destRec, origin, 0.0f, WHITE);
      break;
    case WALK:
      DrawTexturePro(playerWalk, sourceRec, destRec, origin, 0.0f, WHITE);
      break;
    }
    DrawRectangleRec(destRec, (Color){100, 100, 100, 100}); // RGBA

    EndDrawing();
  }

  UnloadTexture(playerIdle);
  UnloadTexture(playerWalk);

  CloseWindow();
  return 0;
}

// maybe lock on option so you can walkaway backawrds from an emey without
// turning around

// every floor is just 1 room
// your goal is to get to the top floor
// bosses every 5 floors
// only 10 floors
// isometric game? like zelda?
//
// You can charge your parry meter and when its full you can maybe dash or
// something. Maybe ranged energy attack that destroys projectiles and pierces
