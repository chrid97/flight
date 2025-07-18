#include "raylib.h"
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

const float scale = 2.5f;
typedef enum PlayerState { IDLE, WALK, TOTAL_COUNT } PlayerState;

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

  typedef struct {
    int frameCount;
    int frameWidth;
    int frameHeight;
    int startX; // x offset in texture for first frame
    int startY;
    int frameSpacing; // pixel spacing between frames (often 0)
    float fps;
  } AnimationInfo;

  typedef struct {
    Texture2D texture;
    AnimationInfo info;
    float frameTimer;
    int currentFrame;
  } Animation;

  typedef struct {
    Animation animations[TOTAL_COUNT];
    PlayerState current;
    bool facingRight;
  } SpriteAnimator;

  SpriteAnimator playerAnimator = {.facingRight = true};
  playerAnimator.animations[IDLE] =
      (Animation){.texture = LoadTexture("./assets/player/IDLE.png"),
                  .info = {.frameCount = 4,
                           .frameWidth = 32,
                           .frameHeight = 32,
                           .startX = 0,
                           .startY = 0,
                           .fps = 10}};

  playerAnimator.animations[WALK] = (Animation){
      .texture = LoadTexture("./assets/player/WALK.png"),
      // i think framcount is wrong and fps should be framecount instead
      .info = {.frameCount = 6,
               .frameWidth = 32,
               .frameHeight = 32,
               .startX = 0,
               .startY = 0,
               .fps = 12}};

  // Rectangle sourceRec = {900.0f, 1200.0f, frameWidth,
  //                        frameHeight}; // first frame
  // Rectangle sourceRec2 = {900.0f, 1200.0f, frameWidth,
  //                         frameHeight}; // first frame
  // Rectangle destRec = {player.x, player.y, frameWidth * scale,
  // frameHeight * scale};
  int frameCounter = 0;

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

    // if (IsKeyDown(KEY_SPACE) && player.parryCooldown == 0) {
    //   player.parryCooldown = 15;
    //   player.color = RED;
    // }
    //
    // if (player.parryCooldown > 0) {
    //   player.parryCooldown--;
    //   if (player.parryCooldown == 0) {
    //     player.color = GOLD;
    //   }
    // }

    // Update
    // destRec.x = player.x;
    // destRec.y = player.y;

    // sourceRec.width = player.facingRight ? frameWidth : -frameWidth,

    // frameCounter++;
    // 10 is totalFrames
    // if (frameCounter >= (60 / 10)) {
    //   frameCounter = 0;
    //   sourceRec.x += 96;
    // }
    //
    // if (frameCounter >= (60 / 12)) {
    //   sourceRec2.x += 96;
    // }

    // sourceRec.x

    // Draw //
    BeginDrawing();
    ClearBackground(BLACK);
    switch (player.state) {
    case IDLE: {
      AnimationInfo info = playerAnimator.animations[IDLE].info;
      Rectangle sourceRec = {900.0f, 1200.0f, info.frameWidth,
                             info.frameHeight}; // first frame
      Rectangle destRec = {player.x, player.y, info.frameWidth * scale,
                           info.frameHeight * scale};
      DrawTexturePro(playerIdle, sourceRec, destRec, origin, 0.0f, WHITE);
      break;
    }
    case WALK:
      // DrawTexturePro(playerWalk, sourceRec, destRec, origin, 0.0f, WHITE);
      break;
    }
    // DrawRectangleRec(destRec, (Color){100, 100, 100, 100}); // RGBA

    EndDrawing();
  }

  for (int i = 0; i < TOTAL_COUNT; i++) {
    UnloadTexture(playerAnimator.animations[i].texture);
  }

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
