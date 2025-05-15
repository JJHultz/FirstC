#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>

#define WIDTH 126
#define HEIGHT 32
#define PLATFORM_COUNT 10

int MAX_BULLETS = 10;
int HEALTH = 10;
int LEVEL = 0;
int CROUCHING = 0;
int CUR_BULLETS = 0;
int CUR_BOUNCES = 1;
bool SLOMOACTIVE = false;
int SLOMO = 100;
int SLOMOTIMER = 0;
int SLOMOCD = 400;
int SLOMOCDTIMER = 0;

typedef struct {
    float x, y;
    float vx, vy;
    float ax, ay;
    bool inAir;
} Character;

typedef struct {
    float x, y, width; // will begin drawing platform at x, y in draw function for width amount
} Platform;

typedef struct {
	float x, y;
	float vx, vy;
	int bounces;
	bool active;
} Bullet;

Platform* platArr[PLATFORM_COUNT];
Character player = {10.0, 5.0, 0.0, 0.0, 0.0, 0.0, true};


void spawnBullet(Bullet* bullets[], int bx, int by, int bvx, int bvy, char key) {
    for (int i = 0; i < MAX_BULLETS; i++) {
	if (!bullets[i]->active) {
		bullets[i]->active = true;
		bullets[i]->x = player.x;
		bullets[i]->y = player.y;
		bullets[i]->vx = player.vx;
		bullets[i]->vy = player.vy;
		bullets[i]->bounces = CUR_BOUNCES;
		
		if (key == 'u') {
			bullets[i]->vx -= 0.7;
			bullets[i]->vy -= 0.7;
		}
		if (key == 'i') {
			bullets[i]->vx += 0.0;
			bullets[i]->vy -= 1.0;
		}
		if (key == 'o') {
			bullets[i]->vx += 0.7;
			bullets[i]->vy -= 0.7;
		}
		if (key == 'j') {
			bullets[i]->vx -= 1.0;
			bullets[i]->vy -= 0.3;
		}
		if (key == 'l') {
			bullets[i]->vx += 1.0;
			bullets[i]->vy -= 0.3;
		}
		if (key == 'm') {
			bullets[i]->vx -= 0.7;
			bullets[i]->vy += 0.7;
		}
		if (key == ',') {
			bullets[i]->vx += 0.0;
			bullets[i]->vy += 1.0;
		}
		if (key == '.') {
			bullets[i]->vx += 0.7;
			bullets[i]->vy += 0.7;
		}
		return;
	}
    }
}


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
    player.vx = 0;
    player.vy = 0;
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

    //refresh();  // Update screen
}

int main() {

    Bullet* bulletArr[MAX_BULLETS];

    // here we will initialize 10 bullets, should be refactored later to allow for better scaling & !manual initialization
    Bullet bul0 = {0.0, 0.0, 0.0, 0.0, 1, false};
    Bullet bul1 = {0.0, 0.0, 0.0, 0.0, 1, false};
    Bullet bul2 = {0.0, 0.0, 0.0, 0.0, 1, false};
    Bullet bul3 = {0.0, 0.0, 0.0, 0.0, 1, false};
    Bullet bul4 = {0.0, 0.0, 0.0, 0.0, 1, false};
    Bullet bul5 = {0.0, 0.0, 0.0, 0.0, 1, false};
    Bullet bul6 = {0.0, 0.0, 0.0, 0.0, 1, false};
    Bullet bul7 = {0.0, 0.0, 0.0, 0.0, 1, false};
    Bullet bul8 = {0.0, 0.0, 0.0, 0.0, 1, false};
    Bullet bul9 = {0.0, 0.0, 0.0, 0.0, 1, false};
    
    bulletArr[0] = &bul0;
    bulletArr[1] = &bul1;
    bulletArr[2] = &bul2;
    bulletArr[3] = &bul3;
    bulletArr[4] = &bul4;
    bulletArr[5] = &bul5;
    bulletArr[6] = &bul6;
    bulletArr[7] = &bul7;
    bulletArr[8] = &bul8;
    bulletArr[9] = &bul9;

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
    bool debug;
    
    while (1) {
        draw(player);
	// debug = false;

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
		// debug = false;
	    }
            if (ch == 'a') {
		    player.vx = -0.5;
		    CROUCHING = 0;
		    // debug = false;
	    }
            if (ch == 'd') {
		    player.vx = 0.5;
		    CROUCHING = 0;
		    // debug = false;
	    }
            if (ch == ' ') {
                player.vy = -0.25;
                player.inAir = true;
		CROUCHING = 0;

	    if (ch == 's' && CROUCHING) CROUCHING = 0; // debug = false;
	    if (ch == 'w') CROUCHING = 0;
	    
	    player.ax = 0;
	    player.ay = 0;
            }

	    if (ch == 'S') {
		player.vx = 0;
		CROUCHING = 1;
		// debug = true;
	    }
            if (ch == 'A') {
		    player.vx = -0.5;
		    CROUCHING = 0;
		    // debug = true;
	    }
            if (ch == 'D') {
		    player.vx = 0.5;
		    CROUCHING = 0;
		    // debug = true;
	    }
        }

        // Update position
        player.x += player.vx;
        player.y += player.vy;

	// Check for if player is shooting and spawn bullets
	if (ch == 'u' || ch == 'i' || ch == 'o' || ch == 'j' || ch == 'l' || ch == 'm' || ch == ',' || ch == '.') {
		spawnBullet(bulletArr, player.x, player.y, player.vx, player.vy, ch);
	}

	// Draw bullets & update positions
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bulletArr[i]->active) {
			mvprintw((int)bulletArr[i]->y, (int)bulletArr[i]->x, "*");
			// debug = true;
			if (bulletArr[i]->x < 0 || bulletArr[i]->x > WIDTH) {
				if (bulletArr[i]->bounces > 0) bulletArr[i]->vx *= -1;
				if (bulletArr[i]->bounces < 1) bulletArr[i]->active = false;
				bulletArr[i]->bounces -= 1;
			}
			if (bulletArr[i]->y < 0 || bulletArr[i]->y > HEIGHT) {
				if (bulletArr[i]->bounces > 0) bulletArr[i]->vy *= -1;
				if (bulletArr[i]->bounces < 1) bulletArr[i]->active = false;
				bulletArr[i]->bounces -= 1;
			}
			if (bulletArr[i]->vy < 1) bulletArr[i]-> vy += 0.02;
			bulletArr[i]->x += bulletArr[i]->vx;
			bulletArr[i]->y += bulletArr[i]->vy;
		}
	}

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

	if (SLOMOACTIVE) mvprintw(0, 30, "SLOMO ACTIVE");

	if (!SLOMOACTIVE && SLOMOCDTIMER > 0) {
		SLOMOCDTIMER -= 1;
		if (SLOMOCDTIMER >= SLOMOCD) mvprintw(0, 30, "SLOMO COOLDOWN: [][][][]");
		if (SLOMOCDTIMER >= SLOMOCD * 3/4) mvprintw(0, 30, "SLOMO COOLDOWN: [][][]");
		if (SLOMOCDTIMER >= SLOMOCD / 2) mvprintw(0, 30, "SLOMO COOLDOWN: [][]");
		if (SLOMOCDTIMER >= SLOMOCD * 1/4) mvprintw(0, 30, "SLOMO COOLDOWN: []");
	}

	if (!SLOMOACTIVE && SLOMOCDTIMER <= 0) mvprintw(0, 30, "SLOMO READY");

    	refresh();  // Update screen

	if (ch == 'e' && SLOMOCD >= 0) {
	    SLOMOACTIVE = true;
	    SLOMOTIMER = SLOMO;
	}

	if (SLOMOACTIVE && SLOMOTIMER > 0) {
		SLOMOTIMER -= 1;
		usleep (30000);
	}
	if (SLOMOACTIVE && SLOMOTIMER < 1) {
		SLOMOACTIVE = false;
		SLOMOCDTIMER += SLOMOCD;
	}


        usleep(13000);  // 10ms delay
	if (debug) usleep (30000);
	if (HEALTH <= 0) loadLevel();
    }

    endwin();  // Clean up ncurses
    return 0;
}

