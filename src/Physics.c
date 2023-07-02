#include <math.h>
#include <stdlib.h>
#include "../include/raylib.h"

#include "Game.h"
#include "Controls.h"

void limit_x_velocity(player_t *player, int max_vel)
{
  // This code is a C rewrite of https://github.com/ChristianD37/YoutubeTutorials/blob/master/Physics/player.py
  player->velocity.x = max(-max_vel, min(player->velocity.x, max_vel));
  if (fabsf(player->velocity.x) < 1) player->velocity.x = 0;
}

void UpdatHorizontalPhysics(player_t *player)
{
  player->acceleration.x = 0;

  if (INPUT_LEFT_DOWN) player->acceleration.x -= 2000;
  if (INPUT_RIGHT_DOWN) player->acceleration.x += 2000;

  player->acceleration.x += player->velocity.x * player->friction;

  player->velocity.x += player->acceleration.x * GetFrameTime();

  limit_x_velocity(player, 700);

  // This code is also a C rewrite of https://github.com/ChristianD37/YoutubeTutorials/blob/master/Physics/player.py
  player->pos.x += player->velocity.x * GetFrameTime() + (player->acceleration.x * .5) * (GetFrameTime() * GetFrameTime());
}

void UpdateVerticalPhysics(player_t *player, Rectangle ground)
{
  if (CheckCollisionCircleRec(player->pos, player->radius, ground)) { player->canJump = true; player->is_jumping = false; }
  else player->canJump = false;

  if (player->canJump) {
    if (INPUT_JUMP_PRESSED) { player->is_jumping = true; player->velocity.y += 430.0f; }
  }

  if (!CheckCollisionCircleRec(player->pos, player->radius, ground)) player->velocity.y -= 1000.0f * GetFrameTime();
  else if (player->canJump && !player->is_jumping) { player->velocity.y = 0; player->pos.y = ground.y-20; }
}
