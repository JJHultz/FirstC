#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>

#define WIDTH 126
#define HEIGHT 32
#define PLATFORM_COUNT 10

typedef struct {
    float x, y;
    float vx, vy;
    float ax, ay;
    bool inAir;
} Character;

typedef struct {
    float x, y, width; // will begin drawing platform at x, y in draw function for width amount
} Platform;

Platform* platArr[PLATFORM_COUNT];

void draw(Character player) {
    clear();  // Use ncurses clear

    mvprintw((int)player.y, (int)player.x, "O");  // Draw character

    // loop to draw platforms
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        if (platArr[i]) {
            int w = (int)platArr[i]->width;
            for (int j = 0; j < w; j++) {
                mvprintw((int)platArr[i]->y, (int)platArr[i]->x + j, "-");
            }
        }
    }

    refresh();  // Update screen
}

int main() {
    Character player = {10.0, 5.0, 0.0, 0.0, 0.0, 0.0, true};

    // Create a platform
    Platform plat1 = {20, 30, 80};
    platArr[0] = &plat1;

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    while (1) {
        draw(player);

        int ch = getch();

        // Handle input only if in air
        if (player.inAir) {
            if (ch == 'a' && player.vx > -0.1) player.ax -= 0.01;
            if (ch == 'd' && player.vx < 0.1) player.ax += 0.01;
            player.ax *= 0.7;

            // Apply gravity
            if (player.vy < 1) player.vy += 0.01;
        }

        // Apply acceleration to velocity
        player.vx += player.ax;

        // Handle grounded movement and jumping
        if (!player.inAir) {
            player.vx = 0;
            if (ch == 'a') player.vx = -0.5;
            if (ch == 'd') player.vx = 0.5;
            if (ch == ' ') {
                player.vy = -0.25;
                player.inAir = true;
            }
        }

        // Update position
        player.x += player.vx;
        player.y += player.vy;

        // Assume player is in air by default
        player.inAir = true;

        // Platform collision detection
        for (int i = 0; i < PLATFORM_COUNT; i++) {
            if (platArr[i]) {
                int py = (int)player.y;
                int px = (int)player.x;
                int platformY = (int)platArr[i]->y;
                int platformXStart = (int)platArr[i]->x;
                int platformXEnd = platformXStart + (int)platArr[i]->width;

                // Check if player lands on top of platform
                if (py + 1 == platformY && px >= platformXStart && px <= platformXEnd && player.vy >= 0) {
                    player.y = platformY - 1;  // Snap to top of platform
                    player.vy = 0;
                    player.inAir = false;
                }
            }
        }

        // Screen boundary collision
        if (player.x < 0 || player.x > WIDTH)  player.vx *= -1;
        if (player.y < 0 || player.y > HEIGHT) player.vy = -0.5;

        usleep(10000);  // 10ms delay
    }

    endwin();  // Clean up ncurses
    return 0;
}

