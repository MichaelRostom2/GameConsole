#pragma once

#include "../GameConsole.h"

typedef enum
{
    Dodge_Start_Game = 1,
    Dodge_Move_Step = 2,
    Dodge_GAME_OVER = 3,
} DodgeState;

// Structures
struct Bullet
{
    float x, y;
    float dx, dy;
    bool active;
};

struct Player
{
    float x, y;
    bool alive;
};

extern DodgeState DODGE_CURRENT_STATE;

bool checkCollision(Bullet bullet, Player player);
void drawPlayer();
void updatePlayer(struct Joystick_input Joystick_input);
void spawnBullet();
void updateBullets();
void eraseBullet(Bullet bullet);
void drawBullet(Bullet bullet);
void checkBulletCollisions();
DodgeState DodgeUpdateFSM(DodgeState curState, struct Joystick_input Joystick_input);
void playDodge(struct Joystick_input Joystick_input);
void displayDodgeGameOver();
void incrementDodgeScore();