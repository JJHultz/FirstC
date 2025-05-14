#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>

#define WIDTH 126
#define HEIGHT 32
#define PLATFORM_COUNT 10

int HEALTH = 10;

int LEVEL = 0;

int CROUCHING = 0;

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
Character player = {10.0, 5.0, 0.0, 0.0, 0.0, 0.0, true};

// here will be the arrays of level platforms

// here we will define a function that will load levels

void loadLevel() {
    for (int i = 0; i < PLATFORM_COUNT; i++) platArr[i] = NULL; // clear old refs
    HEALTH = 10;

    if (LEVEL == 0) {
        static Platform plat1 = {20, 30, 80};
        platArr[0] = &plat1;
        player.x = 21;
        player.y = 29;
    }

    if (LEVEL == 1) {
        static Platform plat1 = {20, 30, 20};
        static Platform plat2 = {60, 30, 20};
        platArr[0] = &plat1;
        platArr[1] = &plat2;

        player.x = 21;
        player.y = 29;
    }
}

void draw(Character player) {
    clear();  // Use ncurses clear

    for (int i = 0; i < HEALTH; i++) mvprintw(0, 2*i, "[]");

    if (!CROUCHING) mvprintw((int)player.y, (int)player.x, "O");  // Draw character
    if (CROUCHING) mvprintw((int)player.y, (int)player.x, "o");

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
    // Character player = {10.0, 5.0, 0.0, 0.0, 0.0, 0.0, true};
    // int CROUCHING = 0;

    // Create a platform
    // Platform plat1 = {20, 30, 80};
    // platArr[0] = &plat1;

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    loadLevel();
    
    while (1) {
        draw(player);

        int ch = getch();

	// here we will check if the player's current/new position is at the level end
	// for now it will be a key press as an event placeholder
	if (ch == '7') {
	    LEVEL += 1;
	    loadLevel();
	}

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
	    // if (ch != 'a' || ch != 'd' || ch != ' ') {
                // player.vx = 0;
	    // }
	    if (ch == 's') {
		player.vx = 0;
		CROUCHING = 1;
	    }
            if (ch == 'a') {
		    player.vx = -0.5;
		    CROUCHING = 0;
	    }
            if (ch == 'd') {
		    player.vx = 0.5;
		    CROUCHING = 0;
	    }
            if (ch == ' ') {
                player.vy = -0.25;
                player.inAir = true;
		CROUCHING = 0;
	    if (ch == 's' && CROUCHING) CROUCHING = 0;
	    if (ch == 'w') CROUCHING = 0;
	    
	    player.ax = 0;
	    player.ay = 0;
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
        if (player.x < 0 || player.x > WIDTH) {
		player.vx *= -1; 
		HEALTH -= 1;
	}
        if (player.y < 0 || player.y > HEIGHT) {
		player.vy = -0.5;
		HEALTH -= 1; 
        }

        usleep(10000);  // 10ms delay
    }

    endwin();  // Clean up ncurses
    return 0;
}

