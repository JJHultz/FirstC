#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>

#define WIDTH 126
#define HEIGHT 32

typedef struct {
    float x, y;
    float vx, vy;
    float ax, ay;
    bool inAir;
} Character;

void draw(Character player) {
    clear();  // Use ncurses clear

    mvprintw((int)player.y, (int)player.x, "O");  // Draw character

    refresh();  // Update screen
}

int main() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    Character player = {10.0, 5.0, 0.0, 0.0, 0.0, 0.0, true};

    while (1) {
        draw(player);

        int ch = getch();

        // Handle input only if in air
        if (player.inAir) {
            if (ch == 'a' && player.vx > -0.4) player.ax -= 0.1;
            if (ch == 'd' && player.vx < 0.4) player.ax += 0.1;
	    player.ax *= 0.7;
        }

        // Apply acceleration to velocity
        player.vx += player.ax;

        // Update position
        player.x += player.vx;
        player.y += player.vy;

        // Collision handling
        if (player.x < 0 || player.x > WIDTH)  player.vx *= -1;
        if (player.y < 0 || player.y > HEIGHT) player.vy *= -1;

        usleep(10000);  // 10ms delay
    }

    endwin();  // Clean up ncurses
    return 0;
}

