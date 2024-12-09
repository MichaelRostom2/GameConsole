#include "Dodge_utils.h"

const int bulletSize = 12;
const int playerSize = 16;
float bulletSpeed = 150.0; // px/s
const float MAX_BULLET_SPEED = 300.0;
float playerSpeed = 50.0; // px/s
uint16_t playerColor = 0xFFE0;
const uint16_t bulletColor = 0x8410;
const uint16_t bgColor = 0x0000;
float bulletSpawnRate = 0.25;
float bulletSpawnTimer = 0.0;
const float bulletSpawnRamp = -0.00005;
const float bulletSpeedRamp = 0.02;

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
const int maxBullets = 30;
Bullet bullets[maxBullets];
int dodgePlayerScore = 0;

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
    float oldX = player.x;
    float oldY = player.y;

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

    Erase(oldX, oldY, player.x, player.y, playerSize);
}

////////////// Bullet functions //////////////

void spawnBullet()
{
    if (bulletSpawnTimer < bulletSpawnRate)
    {
        bulletSpawnTimer += 0.01;
        return;
    }
    else
    {
        bulletSpawnTimer = 0.0;
    }
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
    // Ramp speed and spawn rate
    bulletSpawnRate += bulletSpawnRamp;
    if (bulletSpawnRate < 0)
        bulletSpawnRate = 0;
    bulletSpeed += bulletSpeedRamp;
    if (bulletSpeed > MAX_BULLET_SPEED)
    {
        bulletSpeed = MAX_BULLET_SPEED;
    }
    for (int i = 0; i < maxBullets; i++)
    {
        if (bullets[i].active)
        {
            float oldX = bullets[i].x;
            float oldY = bullets[i].y;
            bullets[i].x += bullets[i].dx * 0.01;
            bullets[i].y += bullets[i].dy * 0.01;
            Erase(oldX, oldY, bullets[i].x, bullets[i].y, bulletSize);
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
                incrementDodgeScore();
            }
        }
    }
}

void incrementDodgeScore()
{
    dodgePlayerScore += 1;
    if (dodgePlayerScore > dodgeHighScore)
    {
        dodgeHighScore = dodgePlayerScore;
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
    switch (curState)
    {
    case Dodge_Start_Game:
        // Display Intro Sequence
        Serial.println("CMD:GET dodge");

        if (Serial.available() > 0)
        {
            Serial.println("inside the if stat");
            String message = Serial.readStringUntil('\n');
            Serial.print("Received Message: ");
            Serial.println(message);

            processResponse(message, dodgeHighScore);
        }

        // Intro Sequence
        gfx->fillScreen(BLACK);
        gfx->setCursor(0, 0);
        gfx->setTextSize(4);
        gfx->setTextColor(WHITE);
        gfx->print("Welcome to");
        gfx->setCursor(10, screenHeight / 4 - 25);
        gfx->setTextColor(CYAN);
        gfx->println("  Dodge");
        gfx->setTextSize(3);
        gfx->setCursor(-3, screenHeight / 2);
        gfx->setTextColor(WHITE);
        gfx->println("  High Score");
        gfx->setCursor(screenWidth / 2 - 22, screenHeight / 2 + 60);
        gfx->setTextSize(4);
        gfx->setTextColor(YELLOW);
        gfx->println(dodgeHighScore);
        delay(1500);

        // Display Game
        gfx->fillScreen(BLACK);
        drawPlayer();
        nextState = Dodge_Move_Step;
        dodgePlayerScore = 0;
        playerColor = 0xFFE0;
        break;

    case Dodge_Move_Step:
        spawnBullet();
        updatePlayer(Joystick_input);
        drawPlayer();
        updateBullets();
        checkBulletCollisions();
        if (!player.alive)
        {
            nextState = Dodge_GAME_OVER;
        }
        else
        {
            nextState = Dodge_Move_Step;
        }
        break;
    case Dodge_GAME_OVER:
        displayDodgeGameOver();
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

void displayDodgeLossCutscene()
{
    playerColor = RED;
    drawPlayer();
    delay(500);
    for (int i = 0; i < maxBullets; i++)
    {
        if (bullets[i].active)
        {
            eraseBullet(bullets[i]);
            delay(125);
        }
    }
    delay(500);
    playerColor = 0x5000;
    drawPlayer();
    delay(500);
    erasePlayer();
}
/*!
  @brief  Displays Game Over Screen
*/
void displayDodgeGameOver()
{
    // TODO: save scores to computer here
    displayDodgeLossCutscene();
    gfx->setTextColor(ORANGE);
    gfx->setTextSize(4);

    gfx->setCursor(screenWidth / 15, screenHeight / 3);
    gfx->println("Game Over");
    gfx->setCursor(screenWidth / 6, 2 * (screenHeight / 4));
    gfx->setTextColor(YELLOW);
    gfx->setTextSize(3);
    gfx->print("Score: ");
    gfx->println(dodgePlayerScore);

    sendNewScore("dodge", dodgePlayerScore);
}
