#include <Arduino_GFX_Library.h>
#include <vector>

/* MOSI: 11 */
/* SCK: 13 */
Arduino_DataBus *bus = new Arduino_HWSPI(8 /* DC */, 10 /* CS */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, 9 /* RST */);

const int screenWidth = 240;
const int screenHeight = 320;

// Bullet structure
struct Bullet {
  uint16_t x, y;   
  uint16_t dx, dy;
  uint16_t width, height;
  uint16_t color;
  bool active; 
};

// Maximum number of bullets
const int maxBullets = 100;
Bullet bullets[maxBullets];

// Spawn a new bullet along the screen edges
void spawnBullet() {
  for (int i = 0; i < maxBullets; i++) {
    if (!bullets[i].active) {
      // Randomly select one of the screen edges
      int edge = random(0, 4); // 0 = top, 1 = bottom, 2 = left, 3 = right

      if (edge == 0) { // Top edge
        bullets[i].x = random(0, screenWidth);
        bullets[i].y = 0;
      } else if (edge == 1) { // Bottom edge
        bullets[i].x = random(0, screenWidth);
        bullets[i].y = screenHeight - 1;
      } else if (edge == 2) { // Left edge
        bullets[i].x = 0;
        bullets[i].y = random(0, screenHeight);
      } else if (edge == 3) { // Right edge
        bullets[i].x = screenWidth - 1;
        bullets[i].y = random(0, screenHeight);
      }

      // Random movement direction (-1, 0, or 1 for dx and dy)
      bullets[i].dx = random(-1, 2);
      bullets[i].dy = random(-1, 2);

      // Ensure the bullet has some movement
      while (bullets[i].dx == 0 && bullets[i].dy == 0) {
        bullets[i].dx = random(-1, 2);
        bullets[i].dy = random(-1, 2);
      }

      // Random size and color
      bullets[i].width = random(5, 20);
      bullets[i].height = random(5, 20);
      bullets[i].color = random(0xFFFF); // Random color

      // Activate the bullet
      bullets[i].active = true;
      break;
    }
  }
}

// Update all bullets
void updateBullets() {
  for (int i = 0; i < maxBullets; i++) {
    if (bullets[i].active) {
      // Erase the current bullet by drawing over it with a white rectangle
      gfx->fillRect(bullets[i].x, bullets[i].y, bullets[i].width, bullets[i].height, 0xFFFF);

      // Update the position
      bullets[i].x += bullets[i].dx;
      bullets[i].y += bullets[i].dy;

      // Check if the bullet is off-screen
      if (bullets[i].x < -bullets[i].width || bullets[i].x > screenWidth ||
          bullets[i].y < -bullets[i].height || bullets[i].y > screenHeight) {
        bullets[i].active = false;
      } else {
        // Draw the bullet in its new position
        gfx->fillRect(bullets[i].x, bullets[i].y, bullets[i].width, bullets[i].height, bullets[i].color);
      }
    }
  }
}

void setup() {
  // Initialize the display
  gfx->begin();
  gfx->setRotation(0);
  gfx->fillScreen(0xFFFF); // Set background to white

  // Seed the random number generator
  randomSeed(analogRead(0));
}

void loop() {
  // Spawn a new bullet occasionally
  if (random(0, 10) < 1) { // Adjust frequency of spawning
    spawnBullet();
  }

  // Update and draw all bullets
  updateBullets();

  delay(30); // Control animation speed
}