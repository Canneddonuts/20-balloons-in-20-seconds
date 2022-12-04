#ifndef GAME_HEADER
#define GAME_HEADER

typedef struct
{
    Vector2 pos;
    Vector2 velocity;
    Vector2 acceleration;
    float radius;
    float friction;
    bool active;
    bool canJump;
    bool is_jumping;
} player_t;

typedef struct
{
    Vector2 pos;
    Vector2 velocity;
    float radius;
    bool active;
} entity_t;

typedef struct
{
  float Lifetime;
} gtimer_t;

#define max(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

#define min(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})

void limit_x_velocity(player_t *player, int max_vel);
void UpdatHorizontalPhysics(player_t *player);
void UpdateVerticalPhysics(player_t *player, Rectangle ground);
void StartTimer(gtimer_t *timer, float lifetime);
void UpdateTimer(gtimer_t *timer);
bool TimerDone(gtimer_t *timer);

#endif
