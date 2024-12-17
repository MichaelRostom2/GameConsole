#include "Dodge_utils.h"

// Bullet Constants
const int bulletSize = 12;
float bulletSpeed; // px/s
const float MAX_BULLET_SPEED = 300.0;
const uint16_t bulletColor = 0x8410;
float bulletSpawnRate;
float bulletSpawnTimer;
const float bulletSpawnRamp = -0.00005;
const float bulletSpeedRamp = 0.02;
const int maxBullets = 30;
Bullet bullets[30];

// Player Constants
const int playerSize = 16;
const float playerSpeed = 50.0; // px/s
uint16_t playerColor = 0xFFE0;

// Background color
const uint16_t bgColor = 0x0000;

// Global objects
Player player;
int dodgePlayerScore;

// Game Variables
bool DodgeGameOver = false;

////////////// Player functions //////////////
/*!
  @brief  Draws player on the screen using the global player object
*/
void drawPlayer()
{
    gfx->fillRect(player.x, player.y, playerSize, playerSize, playerColor);
}

/*!
  @brief  Updates and draws player position based on Joystick input. And erases the old player position.
  @param  Joystick_input Latest input from Joystick
*/
void updatePlayer(struct Joystick_input Joystick_input, Player &player)
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
    // Erase the old player
    EraseRect(oldX, oldY, player.x, player.y, playerSize);

    // Draw the new player
    drawPlayer();
}

////////////// Bullet functions //////////////

/*!
  @brief Spawns a bullet at a random edge of the screen
*/
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

/*!
  @brief Updates bullet positions and draws them on the screen
*/
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
            EraseRect(oldX, oldY, bullets[i].x, bullets[i].y, bulletSize);

            // Draw the new bullet
            gfx->fillRect(bullets[i].x, bullets[i].y, bulletSize, bulletSize, bulletColor);
        }
    }
}
/*!
    @brief  Checks if a bullet has collided with the player
    @param  bullet Bullet object to check
    @param  player Player object to check
    @return True if collision has occurred, False otherwise
*/
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

/*!
  @brief  Checks if any bullet has collided with the player or has gone out of bounds
*/
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
                dodgePlayerScore += 1;
                if (dodgePlayerScore > dodgeHighScore)
                {
                    dodgeHighScore = dodgePlayerScore;
                }
            }
        }
    }
}
/*!
  @brief  Updates Dodge according to FSM states and guards
  @param  curState Current state of Game
  @param  Joystick_input Latest input from Joystick
  @return New dodge State
*/
DodgeState DodgeUpdateFSM(DodgeState curState, struct Joystick_input Joystick_input)
{
    DodgeState nextState;
    String message;
    switch (curState)
    {
    case DODGE_START:
        // reset/initialize global variables
        bulletSpeed = 150.0;
        bulletSpawnRate = 0.25;
        bulletSpawnTimer = 0;
        player = {(screenWidth * 1.0f) / 2 - playerSize / 2, (screenHeight * 1.0f) / 2 - playerSize / 2, true};
        dodgePlayerScore = 0;
        for (int i = 0; i < maxBullets; i++)
        {
            bullets[i].active = false;
        }

        // sendNewScore("dodge", dodgeHighScore);
        Serial.println("CMD:GET dodge");
        delay(10);
        if (Serial.available() > 0)
        {
            String message = Serial.readStringUntil('\n');
            Serial.print("Received Message: ");
            Serial.println(message);
            processResponse(message, dodgeHighScore);
        }

        DrawDodgeIntro();

        // Initialize Game
        gfx->fillScreen(BLACK);
        drawPlayer();
        nextState = DODGE_MOVE;
        dodgePlayerScore = 0;
        playerColor = 0xFFE0;
        break;

    case DODGE_MOVE:
        spawnBullet();
        updatePlayer(Joystick_input, player);
        updateBullets();
        checkBulletCollisions();
        if (!player.alive)
        {
            nextState = DODGE_GAME_OVER;
        }
        else
        {
            nextState = DODGE_MOVE;
        }
        break;
    case DODGE_GAME_OVER:
        if (!DodgeGameOver)
        {
            displayDodgeGameOver();
            DodgeGameOver = true;
        }
        nextState = DODGE_GAME_OVER;
        break;
    }
    return nextState;
}
DodgeState DODGE_CURRENT_STATE = DODGE_START;

/*!
  @brief  Plays the Dodge Game by calling updateFSM of Dodge
  @param  joystickInput Latest input from Joystick
*/
void playDodge(struct Joystick_input joystickInput)
{
    DODGE_CURRENT_STATE = DodgeUpdateFSM(DODGE_CURRENT_STATE, joystickInput);
}
/*!
  @brief Draws Dodge Intro welcome screen
*/
void DrawDodgeIntro()
{
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
}
/*!
  @brief  Displays Dodge Loss Cutscene.
*/
void displayDodgeLossCutscene()
{

    playerColor = RED;
    drawPlayer();
    delay(500);
    for (int i = 0; i < maxBullets; i++)
    {
        if (bullets[i].active)
        {
            // Erase the bullet
            gfx->fillRect(bullets[i].x, bullets[i].y, bulletSize, bulletSize, bgColor);
            delay(100);
        }
    }
    delay(500);
    playerColor = 0x5000;
    drawPlayer();
    delay(500);
    // Erase the player
    gfx->fillRect(player.x, player.y, playerSize, playerSize, bgColor);
}
/*!
  @brief  Displays Game Over Screen
*/
void displayDodgeGameOver()
{
    sendNewScore("dodge", dodgePlayerScore);
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
}
