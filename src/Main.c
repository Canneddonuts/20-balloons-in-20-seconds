#include "../include/raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#include "Game.h"
#include "Controls.h"

#define MAX_BALLOONS 5

static int screen = 0;
static player_t player = { 0 };
static entity_t balloons[MAX_BALLOONS] = { 0 };
static entity_t enemy = { 0 };
static bool gameover = false, Debug = false;
static int collected = 0;
static Rectangle ground = {
    0,
    400,
    0,
    50
};
static gtimer_t gameTimer = { 0 };

static void Init(void);
static void Update(void);
static void Draw(void);
static void UpdateDrawFrame(void);


int main(void)
{
    SetConfigFlags(FLAG_VSYNC_HINT);

    InitWindow(500, 450, "20sec");

    Init();

    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
    #else
        SetTargetFPS(60);

        while (!WindowShouldClose()) UpdateDrawFrame();
    #endif

    CloseWindow();

    return 0;
}

void Init(void)
{
  collected = 0;
  gameover = false;

  player.pos = (Vector2){ (float)GetScreenWidth()/2, (float)GetScreenHeight()/2 };
  player.velocity = (Vector2){ 0, 0 };
  player.radius = 20;
  player.friction = -3.5;

  for (int i = 0; i < MAX_BALLOONS; ++i) {
      balloons[i].pos = (Vector2) {
        (float)GetRandomValue(0, GetScreenWidth()-50),
        (float)GetRandomValue(-100, 0)
      };
      balloons[i].velocity = (Vector2){ 0, 100.0f };
      balloons[i].radius = 15;
      balloons[i].active = true;
  }

  enemy.pos = (Vector2){ (float)GetScreenWidth() - 22, 380 };
  enemy.velocity = (Vector2){ 150.0f, 0 };
  enemy.radius = 20;

  ground.width = GetScreenWidth() + 20;

  StartTimer(&gameTimer, 20);
}

void Update(void)
{
  if (IsKeyPressed(KEY_D)) Debug = !Debug;

  switch (screen) {
    case 0: if (INPUT_OPTION_PRESSED) screen = 1; break;
    case 1: {
      UpdateTimer(&gameTimer);

      if (TimerDone(&gameTimer)) gameover = true;

      if (!gameover) {

        // player physics
        UpdatHorizontalPhysics(&player);
        UpdateVerticalPhysics(&player, ground);

        enemy.pos.x -= enemy.velocity.x * GetFrameTime();

        player.pos.y -= player.velocity.y * GetFrameTime();

        if ((enemy.pos.x <= enemy.radius) || (enemy.pos.x >= (GetScreenWidth() - enemy.radius))) enemy.velocity.x *= -1;

        if (CheckCollisionCircles(enemy.pos, enemy.radius, player.pos, player.radius)) gameover = true;

        for (int i = 0; i < MAX_BALLOONS; ++i) {
            if (balloons[i].active) balloons[i].pos.y += balloons[i].velocity.y * GetFrameTime();
            else {
                balloons[i].pos.y = -15;
                balloons[i].pos.x = (float)GetRandomValue(0, GetScreenWidth()-50);
                balloons[i].active = true;
            }

            // balloon collision
            if (CheckCollisionCircleRec(balloons[i].pos, balloons[i].radius, ground)) balloons[i].active = false;
            else if (CheckCollisionCircles(balloons[i].pos, balloons[i].radius, player.pos, player.radius)) { balloons[i].active = false; collected++; }
        }

        // player wall collision
        if ((player.pos.x <= player.radius) || (player.pos.x >= (GetScreenWidth() - player.radius))) { player.velocity.x *= -2; }
      } else {
        if (INPUT_OPTION_PRESSED) Init();
      }
    } break;
    default: break;
  }

}

void Draw(void)
{
  BeginDrawing();

      ClearBackground(RAYWHITE);

     if (Debug) DrawFPS(GetScreenWidth()-100, 0);

      switch (screen) {
        case 0: {
          DrawText("20 balloons in 20 seconds", 30, 20, 30, BLACK);

          DrawText("PRESS ENTER OR START", GetScreenWidth()/4, GetScreenHeight() - 170, 20, RED);

          DrawText("Canneddonuts 2022", 5, GetScreenHeight() - 20, 20, GRAY);

          DrawText("v1.3", GetScreenWidth() - 35, GetScreenHeight() - 20, 20, GREEN);
        } break;
        case 1: {
          if (!gameover) {
            DrawText(TextFormat("%d", (int)gameTimer.Lifetime), GetScreenWidth()/2 - 90, 100, 150, LIGHTGRAY);

            if (Debug) {
                DrawText(TextFormat("X VELOCITY: %f", player.velocity.x), GetScreenWidth()/2 - 150, 100, 20, GREEN);
                DrawText(TextFormat("X POS: %f", player.pos.x), GetScreenWidth()/2 - 150, 120, 20, GREEN);
                DrawText(TextFormat("X ACCELERATION: %f", player.acceleration.x), GetScreenWidth()/2 - 150, 140, 20, GREEN);
                DrawText(TextFormat("Y VELOCITY: %f", player.velocity.y), GetScreenWidth()/2 - 150, 160, 20, GREEN);
                DrawText(TextFormat("Y POS: %f", player.pos.y), GetScreenWidth()/2 - 150, 180, 20, GREEN);
                DrawText(TextFormat("Y ACCELERATION: %f", player.acceleration.y), GetScreenWidth()/2 - 150, 200, 20, GREEN);
            }

            DrawCircleV(player.pos, player.radius, BLUE);

            DrawCircleV(enemy.pos, enemy.radius, GREEN);

            for (int i = 0; i < MAX_BALLOONS; ++i) {
                if (balloons[i].active) DrawCircleV(balloons[i].pos, balloons[i].radius, RED);
            }

            DrawRectangle(ground.x, ground.y, ground.width, ground.height, GRAY);

            DrawText(TextFormat("SCORE: %d", collected), 10, 5, 30, RED);
          } else if (collected > 19) {
              DrawText("YOU WIN", GetScreenWidth()/2 - 150, 100, 70, GREEN);

              DrawText(TextFormat("Score was %d balloons", collected), GetScreenWidth()/2 - 120, 200, 20, BLACK);

              DrawText("PRESS ENTER OR START", GetScreenWidth()/2 - 130, 240, 20, RED);
          } else {
              DrawText("GAME OVER", GetScreenWidth()/2 - 150, 100, 50, RED);

              DrawText(TextFormat("Score was %d balloons", collected), GetScreenWidth()/2 - 120, 200, 20, BLACK);

              DrawText("PRESS ENTER OR START", GetScreenWidth()/2 - 130, 240, 20, RED);
          }
        } break;
        default: break;
      }

  EndDrawing();
}

void UpdateDrawFrame(void)
{
  Update();
  Draw();
}
