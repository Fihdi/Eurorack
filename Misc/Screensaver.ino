#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// Define the screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED reset pin
#define OLED_RESET    -1

// Create the display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define sphere structure
struct Sphere {
  float x, y;       // Position
  float vx, vy;     // Velocity
  int radius;       // Radius
  float mass;       // Mass
};

// Define the spheres
int amount = 4;
Sphere spheres[4];
// Initialize spheres with random positions and velocities
void initSpheres() {

spheres[0].radius = 3;
spheres[1].radius = 3;
spheres[2].radius = 5;
spheres[3].radius = 7;

  for (int i = 0; i < amount; i++) {
    spheres[i].mass = spheres[i].radius * spheres[i].radius;  // Mass proportional to the area
    spheres[i].x = random(spheres[i].radius, SCREEN_WIDTH - spheres[i].radius);
    spheres[i].y = random(spheres[i].radius, SCREEN_HEIGHT - spheres[i].radius);
    spheres[i].vx = random(-2, 3);  // Random velocity between -2 and 2
    spheres[i].vy = random(-2, 3);
  }
}

// Check for collisions and exchange momentum
void checkCollisions() {
  for (int i = 0; i < amount; i++) {
    for (int j = i + 1; j < amount; j++) {
      float dx = spheres[i].x - spheres[j].x;
      float dy = spheres[i].y - spheres[j].y;
      float distance = sqrt(dx * dx + dy * dy);
      if (distance <= spheres[i].radius + spheres[j].radius) {
        // Calculate the normal and tangent vectors
        float nx = dx / distance;
        float ny = dy / distance;

        // Calculate the relative velocity in the normal direction
        float vn1 = spheres[i].vx * nx + spheres[i].vy * ny;
        float vn2 = spheres[j].vx * nx + spheres[j].vy * ny;

        // Calculate the new velocities in the normal direction
        float vn1Final = (vn1 * (spheres[i].mass - spheres[j].mass) + 2 * spheres[j].mass * vn2) / (spheres[i].mass + spheres[j].mass);
        float vn2Final = (vn2 * (spheres[j].mass - spheres[i].mass) + 2 * spheres[i].mass * vn1) / (spheres[i].mass + spheres[j].mass);

        // Update the velocities
        spheres[i].vx += (vn1Final - vn1) * nx;
        spheres[i].vy += (vn1Final - vn1) * ny;
        spheres[j].vx += (vn2Final - vn2) * nx;
        spheres[j].vy += (vn2Final - vn2) * ny;
      }
    }
  }
}

// Update sphere positions and wrap around screen boundaries
void updateSpheres() {
  for (int i = 0; i < amount; i++) {
    spheres[i].x += spheres[i].vx;
    spheres[i].y += spheres[i].vy;

    // Wrap around the screen boundaries
    if (spheres[i].x < -spheres[i].radius) spheres[i].x = SCREEN_WIDTH + spheres[i].radius;
    if (spheres[i].x > SCREEN_WIDTH + spheres[i].radius) spheres[i].x = -spheres[i].radius;
    if (spheres[i].y < -spheres[i].radius) spheres[i].y = SCREEN_HEIGHT + spheres[i].radius;
    if (spheres[i].y > SCREEN_HEIGHT + spheres[i].radius) spheres[i].y = -spheres[i].radius;
  }
}

// Draw spheres on the screen
void drawSpheres() {

  display.clearDisplay();
  //display.drawTriangle(spheres[0].x, spheres[0].y,spheres[1].x, spheres[1].y,spheres[2].x, spheres[2].y, SSD1306_WHITE);

  
  for (int i = 0; i < amount; i++) {
    display.fillCircle((int)spheres[i].x, (int)spheres[i].y, spheres[i].radius, SSD1306_WHITE);
  }
  
  display.display();
}

void setup() {
  // Initialize display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);  // Pause for 2 seconds

  // Initialize spheres
  initSpheres();
}

void loop() {
  // Update and draw spheres
  checkCollisions();
  updateSpheres();
  drawSpheres();

  // Delay for smooth animation
  delay(10);
}
