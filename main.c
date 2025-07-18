#include "./lib/cJSON/cJSON.h"
#include "raylib.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRAVITY 0.4f
#define JUMP_VELOCITY -8.0f
#define GROUND_Y 350 // or whatever your ground level is
#define MAX_FRAMES 256
#define MAX_TAGS 16
const float scale = 2.5f;

typedef enum PlayerState { IDLE, WALK, DEFEND, TOTAL_COUNT, DASH } PlayerState;

typedef struct Entity {
  int16_t x, y, dx, dy, width, height;
  PlayerState state;
  bool facingRight;
  Color color;
} Entity;

typedef struct Frame {
  Rectangle rect;
  int duration;
} Frame;

typedef struct FrameTag {
  char name[32];
  int from, to;
} FrameTag;

Frame frames[MAX_FRAMES];
FrameTag tags[MAX_TAGS];
int frameCount = 0, tagCount = 0;
int currentFrame = 0;
float frameTimer = 0;

char *loadFile(const char *filename) {
  FILE *file = fopen(filename, "rb");
  assert(file);
  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *data = malloc(length + 1);
  assert(data);
  fread(data, 1, length, file);
  data[length] = '\0';
  fclose(file);
  return data;
}

void parse_spritesheet_json(const char *json_data) {
  cJSON *root = cJSON_Parse(json_data);
  assert(root);

  cJSON *frames_array = cJSON_GetObjectItem(root, "frames");
  assert(frames_array);

  frameCount = 0;
  cJSON *frame_entry = NULL;
  cJSON_ArrayForEach(frame_entry, frames_array) {
    if (frameCount >= MAX_FRAMES)
      break;
    cJSON *frame_obj = cJSON_GetObjectItem(frame_entry, "frame");
    cJSON *duration_obj = cJSON_GetObjectItem(frame_entry, "duration");
    if (!frame_obj)
      continue;

    frames[frameCount].rect.x =
        (float)cJSON_GetObjectItem(frame_obj, "x")->valueint;
    frames[frameCount].rect.y =
        (float)cJSON_GetObjectItem(frame_obj, "y")->valueint;
    frames[frameCount].rect.width =
        (float)cJSON_GetObjectItem(frame_obj, "w")->valueint;
    frames[frameCount].rect.height =
        (float)cJSON_GetObjectItem(frame_obj, "h")->valueint;
    frames[frameCount].duration = duration_obj ? duration_obj->valueint : 100;
    frameCount++;
  }

  cJSON *meta = cJSON_GetObjectItem(root, "meta");
  cJSON *frameTags = cJSON_GetObjectItem(meta, "frameTags");
  tagCount = 0;
  cJSON *tag_entry = NULL;
  cJSON_ArrayForEach(tag_entry, frameTags) {
    if (tagCount >= MAX_TAGS)
      break;
    strncpy(tags[tagCount].name,
            cJSON_GetObjectItem(tag_entry, "name")->valuestring, 31);
    tags[tagCount].from = cJSON_GetObjectItem(tag_entry, "from")->valueint;
    tags[tagCount].to = cJSON_GetObjectItem(tag_entry, "to")->valueint;
    tagCount++;
  }

  cJSON_Delete(root);
}

int find_tag_index(const char *name) {
  for (int i = 0; i < tagCount; i++) {
    if (strcmp(tags[i].name, name) == 0)
      return i;
  }
  return -1;
}

int main(void) {
  InitWindow(800, 450, "Old Samurai Animation Test");
  SetTargetFPS(60);

  Entity player = {.x = 100,
                   .y = 300,
                   .dx = 3,
                   .dy = 3,
                   .width = 32,
                   .height = 32,
                   .facingRight = true,
                   .state = IDLE,
                   .color = WHITE};
  Texture2D playerTexture = LoadTexture("./assets/old_samurai/atlas.png");

  char *jsonText = loadFile("./assets/old_samurai/atlas.json");
  parse_spritesheet_json(jsonText);
  free(jsonText);

  Texture2D background1 = LoadTexture("./assets/background/Hills Layer 01.png");
  Texture2D background2 = LoadTexture("./assets/background/Hills Layer 02.png");
  Texture2D background3 = LoadTexture("./assets/background/Hills Layer 03.png");
  Texture2D background4 = LoadTexture("./assets/background/Hills Layer 04.png");
  Texture2D background5 = LoadTexture("./assets/background/Hills Layer 05.png");
  Texture2D background6 = LoadTexture("./assets/background/Hills Layer 06.png");

  while (!WindowShouldClose()) {
    // Input
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

    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
      player.state = DEFEND;
    }

    if (IsKeyDown(KEY_SPACE)) {
      player.state = DASH;
    }

    // Animation tag
    int tagIndex;

    switch (player.state) {
    case IDLE: {
      tagIndex = find_tag_index("IDLE");
      break;
    }
    case WALK: {
      tagIndex = find_tag_index("WALK");
      break;
    }
    case DEFEND: {
      tagIndex = find_tag_index("DEFEND");
      break;
    }
    case DASH: {
      tagIndex = find_tag_index("DASH");
      break;
    }
    }

    if (tagIndex < 0)
      tagIndex = 0;
    int start = tags[tagIndex].from;
    int end = tags[tagIndex].to;

    frameTimer += GetFrameTime() * 1000.0f;
    if (frameTimer >= frames[currentFrame].duration) {
      frameTimer = 0;
      currentFrame++;
      if (currentFrame > end || currentFrame < start)
        currentFrame = start;
    }

    // Drawing
    BeginDrawing();
    ClearBackground(BLACK);
    // DrawTextureEx(background1, (Vector2){0, 0}, 0, 2.0f, WHITE);
    DrawTexturePro(
        background1, (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
        (Rectangle){0, 0, 850 + 400, 450 + 341}, (Vector2){0, 0}, 0, WHITE);
    DrawTexturePro(
        background2, (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
        (Rectangle){0, 0, 850 + 400, 450 + 341}, (Vector2){0, 0}, 0, WHITE);
    DrawTexturePro(
        background3, (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
        (Rectangle){0, 0, 850 + 400, 450 + 341}, (Vector2){0, 0}, 0, WHITE);
    DrawTexturePro(
        background4, (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
        (Rectangle){0, 0, 850 + 400, 450 + 341}, (Vector2){0, 0}, 0, WHITE);
    DrawTexturePro(
        background5, (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
        (Rectangle){0, 0, 850 + 400, 450 + 341}, (Vector2){0, 0}, 0, WHITE);
    //
    DrawTexturePro(
        background6, (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
        (Rectangle){0, 0, 850 + 400, 450 + 341}, (Vector2){0, 0}, 0, WHITE);

    Frame frame = frames[currentFrame];
    Rectangle src = frame.rect;
    Rectangle dest = {.x = player.x,
                      .y = player.y,
                      .width = frame.rect.width * scale *
                               (player.facingRight ? 1 : -1),
                      .height = frame.rect.height * scale};

    DrawTexturePro(playerTexture, src, dest, (Vector2){0, 0}, 0, WHITE);
    DrawText("Use A/D or Arrow Keys to Walk", 10, 10, 20, RAYWHITE);

    EndDrawing();
  }

  UnloadTexture(playerTexture);
  CloseWindow();
  return 0;
}
