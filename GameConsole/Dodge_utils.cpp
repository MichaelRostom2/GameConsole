#include "GameConsole.h"
#include "utils.h"
#include "Dodge_utils.h"

const int bulletSize = 12;
const int playerSize = 16;
float bulletSpeed = 150.0; // px/s
float playerSpeed = 50.0;  // px/s
const uint16_t playerColor = 0xFFE0;
const uint16_t bulletColor = 0x8410;
const uint16_t bgColor = 0x0000;

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

// Global objects
Player player = {screenWidth / 2 - playerSize / 2, screenHeight / 2 - playerSize / 2, true};
const int maxBullets = 5;
Bullet bullets[maxBullets];

// Function prototypes
void updateFSM();
bool checkCollision(Bullet bullet, Player player);
void erasePlayer();
void drawPlayer();
void updatePlayer(Joystick_input Joystick_input);
void spawnBullet();
void updateBullets();
void eraseBullet(Bullet bullet);
void drawBullet(Bullet bullet);
void checkBulletCollisions();

////////////// Player functions //////////////

void erasePlayer()
{
    gfx->fillRect(player.x, player.y, playerSize, playerSize, bgColor);
}

void drawPlayer()
{
    gfx->fillRect(player.x, player.y, playerSize, playerSize, playerColor);
}

void updatePlayer(Joystick_input Joystick_input)
{
    player.x += Joystick_input.x / 133 * playerSpeed * 0.01;
    if (player.x < 0)
    {
        player.x = 0;
    }
    if (player.x > screenWidth - playerSize)
    {
        player.x = screenWidth - playerSize;
    }
    player.y += Joystick_input.y / 133 * playerSpeed * 0.01;
    if (player.y < 0)
    {
        player.y = 0;
    }
    if (player.y > screenHeight - playerSize)
    {
        player.y = screenHeight - playerSize;
    }
}

////////////// Bullet functions //////////////

void spawnBullet()
{
    for (int i = 0; i < maxBullets; i++)
    {
        if (!bullets[i].active)
        {
            int edge = random(0, 4);
            if (edge == 0)
            { // Top edge
                bullets[i] = {random(0, screenWidth - bulletSize) * 1.0f, -bulletSize, 0, bulletSpeed, true};
            }
            else if (edge == 1)
            { // Bottom edge
                bullets[i] = {random(0, screenWidth - bulletSize) * 1.0f, screenHeight * 1.0f, 0, -bulletSpeed, true};
            }
            else if (edge == 2)
            { // Left edge
                bullets[i] = {-bulletSize, random(0, screenHeight - bulletSize) * 1.0f, bulletSpeed, 0, true};
            }
            else if (edge == 3)
            { // Right edge
                bullets[i] = {screenWidth * 1.0f, random(0, screenHeight - bulletSize) * 1.0f, -bulletSpeed, 0, true};
            }
            break;
        }
    }
}

void updateBullets()
{
    for (int i = 0; i < maxBullets; i++)
    {
        if (bullets[i].active)
        {
            eraseBullet(bullets[i]);
            bullets[i].x += bullets[i].dx * 0.01;
            bullets[i].y += bullets[i].dy * 0.01;
            drawBullet(bullets[i]);
        }
    }
}

bool checkCollision(Bullet bullet, Player player)
{
    if (!bullet.active)
    {
        return false;
    }
    int bulletRight = bullet.x + bulletSize;
    int bulletBottom = bullet.y + bulletSize;
    int playerRight = player.x + playerSize;
    int playerBottom = player.y + playerSize;

    return !(bulletRight <= player.x || bullet.x >= playerRight || bulletBottom <= player.y || bullet.y >= playerBottom);
}

void checkBulletCollisions()
{
    for (int i = 0; i < maxBullets; i++)
    {
        if (bullets[i].active)
        {
            if (checkCollision(bullets[i], player))
            {
                player.alive = false;
                break;
            }
            if (bullets[i].x < -bulletSize || bullets[i].x > screenWidth || bullets[i].y < -bulletSize || bullets[i].y > screenHeight)
            {
                bullets[i].active = false;
            }
        }
    }
}

void eraseBullet(Bullet bullet)
{
    if (bullet.active)
    {
        // Erase the current bullet by drawing over it with a black rectangle
        gfx->fillRect(bullet.x, bullet.y, bulletSize, bulletSize, bgColor);
    }
}

void drawBullet(Bullet bullet)
{
    if (bullet.active)
    {
        // Erase the current bullet by drawing over it with a black rectangle
        gfx->fillRect(bullet.x, bullet.y, bulletSize, bulletSize, bgColor);

        gfx->fillRect(bullet.x, bullet.y, bulletSize, bulletSize, bulletColor);
    }
}
/*!
  @brief  Updates Dodge according to FSM states and guards
  @param  curState Current state of Game
  @param  Joystick_input Latest input from Joystick
  @return New dodge State
*/
DodgeState DodgeUpdateFSM(DodgeState curState, Joystick_input Joystick_input)
{
    DodgeState nextState;
    Serial.println(player.alive);
    switch (curState)
    {
    case Dodge_Start_Game:
        gfx->fillScreen(BLACK);
        drawPlayer();
        nextState = Dodge_Move_Step;
        break;

    case Dodge_Move_Step:
        if (random(0, 3) < 1)
        {
            spawnBullet();
        }
        erasePlayer();
        updatePlayer(Joystick_input);
        drawPlayer();
        updateBullets();
        checkBulletCollisions();
        if (player.alive)
        {
            nextState = Dodge_GAME_OVER;
        }
        break;
    case Dodge_GAME_OVER:
        // displayDodgeGameOver();

        break;
    }
    return nextState;
}
DodgeState DODGE_CURRENT_STATE = Dodge_Start_Game;

/*!
  @brief  Plays the Dodge Game by calling updateFSM of Dodge
  @param  joystickInput Latest input from Joystick
*/
void playDodge(Joystick_input joystickInput)
{
    DODGE_CURRENT_STATE = DodgeUpdateFSM(DODGE_CURRENT_STATE, joystickInput);
}
/*!
  @brief  Displays Game Over Screen
*/
void displayDodgeGameOver()
{
    // gfx->fillScreen(BLACK);
    gfx->setTextColor(WHITE);
    gfx->setTextSize(4);
    gfx->setCursor(screenWidth / 15, screenHeight / 3);
    gfx->println("Game Over");
}