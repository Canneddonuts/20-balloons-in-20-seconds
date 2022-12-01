#include <stddef.h>
#include "../include/raylib.h"

#include "Game.h"

void StartTimer(gtimer_t *timer, float lifetime)
{
  if (timer != NULL) timer->Lifetime = lifetime;
}

void UpdateTimer(gtimer_t *timer)
{
  if (timer != NULL && timer->Lifetime > 0) timer->Lifetime -= GetFrameTime();
}

bool TimerDone(gtimer_t *timer)
{
  if (timer != NULL) return timer->Lifetime <= 0;

  return false;
}