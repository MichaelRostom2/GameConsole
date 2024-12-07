// #include "GameConsole.h"

// const int bulletSize = 12;
// const int playerSize = 16;
// const int bulletSpeed = 2; // px/s
// const int playerSpeed = 4; // px/s
// const uint16_t playerColor = 0xFFE0;
// const uint16_t bulletColor = 0x8410;
// const uint16_t bgColor = 0x0000;

// // Structures
// struct Bullet
// {
//   int x, y;
//   int dx, dy;
//   bool active;
// };

// struct Player
// {
//   int x, y;
//   int dx, dy; // Based on joystick input
//   bool alive;
// };

// // Global objects
// Player player = {screenWidth / 2 - playerSize / 2, screenHeight / 2 - playerSize / 2, 0, 0, true};
// const int maxBullets = 50;
// Bullet bullets[maxBullets];

// // Function prototypes
// void updateFSM();
// bool checkCollision(Bullet bullet, Player player);
// void erasePlayer();
// void drawPlayer();
// void updatePlayer();
// void pollInputs();
// void spawnBullet();
// void updateBullets();
// void eraseBullet(Bullet bullet);
// void drawBullet(Bullet bullet);
// void checkBulletCollisions();

// ////////////// Player functions //////////////

// void erasePlayer()
// {
//   gfx->fillRect(player.x, player.y, playerSize, playerSize, bgColor);
// }

// void drawPlayer()
// {
//   gfx->fillRect(player.x, player.y, playerSize, playerSize, playerColor);
// }

// void updatePlayer()
// {
//   player.x += player.dx;
//   player.y += player.dy;
// }

// void pollInputs()
// {
//   return; // TODO implement joystick input logic
// }

// ////////////// Bullet functions //////////////

// void spawnBullet()
// {
//   for (int i = 0; i < maxBullets; i++)
//   {
//     if (!bullets[i].active)
//     {
//       int edge = random(0, 4);
//       if (edge == 0)
//       { // Top edge
//         bullets[i] = {random(0, screenWidth - bulletSize), -bulletSize, 0, bulletSpeed, true};
//       }
//       else if (edge == 1)
//       { // Bottom edge
//         bullets[i] = {random(0, screenWidth - bulletSize), screenHeight, 0, -bulletSpeed, true};
//       }
//       else if (edge == 2)
//       { // Left edge
//         bullets[i] = {-bulletSize, random(0, screenHeight - bulletSize), bulletSpeed, 0, true};
//       }
//       else if (edge == 3)
//       { // Right edge
//         bullets[i] = {screenWidth, random(0, screenHeight - bulletSize), -bulletSpeed, 0, true};
//       }
//       break;
//     }
//   }
// }

// void updateBullets()
// {
//   for (int i = 0; i < maxBullets; i++)
//   {
//     if (bullets[i].active)
//     {
//       eraseBullet(bullets[i]);
//       bullets[i].x += bullets[i].dx;
//       bullets[i].y += bullets[i].dy;
//       drawBullet(bullets[i]);
//     }
//   }
// }

// bool checkCollision(Bullet bullet, Player player)
// {
//   if (!bullet.active)
//   {
//     return false;
//   }
//   int bulletRight = bullet.x + bulletSize;
//   int bulletBottom = bullet.y + bulletSize;
//   int playerRight = player.x + playerSize;
//   int playerBottom = player.y + playerSize;

//   return !(bulletRight <= player.x || bullet.x >= playerRight || bulletBottom <= player.y || bullet.y >= playerBottom);
// }

// void checkBulletCollisions()
// {
//   for (int i = 0; i < maxBullets; i++)
//   {
//     if (bullets[i].active)
//     {
//       if (checkCollision(bullets[i], player))
//       {
//         player.alive = false;
//         break;
//       }
//       if (bullets[i].x < -bulletSize || bullets[i].x > screenWidth || bullets[i].y < -bulletSize || bullets[i].y > screenHeight)
//       {
//         bullets[i].active = false;
//       }
//     }
//   }
// }

// void eraseBullet(Bullet bullet)
// {
//   if (bullet.active)
//   {
//     // Erase the current bullet by drawing over it with a black rectangle
//     gfx->fillRect(bullet.x, bullet.y, bulletSize, bulletSize, bgColor);
//   }
// }

// void drawBullet(Bullet bullet)
// {
//   if (bullet.active)
//   {
//     // Erase the current bullet by drawing over it with a black rectangle
//     gfx->fillRect(bullet.x, bullet.y, bulletSize, bulletSize, bgColor);

//     gfx->fillRect(bullet.x, bullet.y, bulletSize, bulletSize, bulletColor);
//   }
// }

// ////////////// Core functions //////////////

// void dodge_setup()
// {
//   Serial.begin(9600);
//   while (!Serial)
//     ;
//   gfx->begin();
//   gfx->setRotation(0);
//   gfx->fillScreen(bgColor);
//   randomSeed(analogRead(0));
// }

// void dodge_loop()
// {
//   if (random(0, 3) < 1)
//   {
//     spawnBullet();
//   }
//   pollInputs();
//   erasePlayer();
//   updatePlayer();
//   drawPlayer();
//   updateBullets();
//   checkBulletCollisions();
//   delay(30);
// }
