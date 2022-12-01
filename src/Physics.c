#include <math.h>
#include <stdlib.h>
#include "../include/raylib.h"

#include "Game.h"

void limit_x_velocity(actor_t *actor, int max_vel)
{
  // This code is a C rewrite of https://github.com/ChristianD37/YoutubeTutorials/blob/master/Physics/player.py
  actor->velocity.x = max(-max_vel, min(actor->velocity.x, max_vel));
  if (fabsf(actor->velocity.x) < 1) actor->velocity.x = 0;
}

void UpdatHorizontalPhysics(actor_t *actor)
{
  actor->acceleration.x = 0;

  if (IsKeyDown(KEY_LEFT)) actor->acceleration.x -= 2000;
  if (IsKeyDown(KEY_RIGHT)) actor->acceleration.x += 2000;

  actor->acceleration.x += actor->velocity.x * actor->friction;

  actor->velocity.x += actor->acceleration.x * GetFrameTime();

  limit_x_velocity(actor, 700);

  // This code is also a C rewrite of https://github.com/ChristianD37/YoutubeTutorials/blob/master/Physics/player.py
  actor->pos.x += actor->velocity.x * GetFrameTime() + (actor->acceleration.x * .5) * (GetFrameTime() * GetFrameTime());
}

void UpdateVerticalPhysics(actor_t *actor, Rectangle ground)
{
  if (CheckCollisionCircleRec(actor->pos, actor->radius, ground)) { actor->canJump = true; actor->is_jumping = false; }
  else actor->canJump = false;

  if (actor->canJump) {
    if ((IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_UP))) { actor->is_jumping = true; actor->velocity.y += 400.0f; }
  }

  if (!CheckCollisionCircleRec(actor->pos, actor->radius, ground)) actor->velocity.y -= 1000.0f * GetFrameTime();
  else if (actor->canJump && !actor->is_jumping) { actor->velocity.y = 0; actor->pos.y = ground.y-20; }
}
