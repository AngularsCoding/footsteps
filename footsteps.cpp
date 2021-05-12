#include<iostream>

#include<graphics.h>

#include<conio.h>

#include<dos.h>

#include<time.h>

#include<stdio.h>

#include<math.h>

#define ScWidth 1400
#define ScHeight 700
#define OpWidth 400

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define SPACEBAR 32

using namespace std;

// Dispaly Gameover screen and reset score
void GameOver();

// Draw game board and score / control area
void DrawBoard();

// if enemy hit player this function returns 1 
int EnemyHitPlayer();

// if player hit fruit this function returns 1 
int PlayerHitFruit();

// Player Variables
int pSize = 40, step = 20; // player size, player speed, direction
int dir = 0; // 0-top, 1-right, 2-bottom, 3-left
double px = ((ScWidth - OpWidth) / 2), py = 350; // player X, player Y

int path[2000][2]; // player Path
int pathIndex = 0; // keep tracks of current position in path

// Enemy Variables
// Enemy X, Enemy Y, Enemy Position on Path
double enemy[100][3];
// enemy index, size, speed
int ei = 0, eSize = 10, eStep = 10;
// Enemy Generator: timer to generate new enemy
int eGen = 0;

// Fruit Variables
int fx = px, fy = ScHeight / 2, fSize = 10;

// Other Variables
char option, txtScore[5];
int score = 0;

int main() {
  // to make sure generation of random numbers
  srand((unsigned) time(0));

  // Initialize Graphics
  int gd = DETECT, gm, err, tmp;
  initgraph( & gd, & gm, "C:\\tc\\bgi");

  // Initilize Path with 0 
  for (int i = 0; i < 2000; i++) {
    path[i][0] = path[i][1] = 0;
  }

  // Initialize first index of path with player position
  path[0][0] = px;
  path[0][1] = py;

  while (1) { // for keeping game playing

    // Draws game board and score/controls area
    DrawBoard();

    // drawing player
    setcolor(WHITE);
    setfillstyle(1, WHITE);
    bar(px, py, px + pSize, py + pSize);

    // drawing fruit
    setcolor(LIGHTGREEN);
    setfillstyle(1, LIGHTGREEN);
    fillellipse(fx, fy, fSize, fSize);

    // change direction if any of arrow keys is pressed
    if (kbhit()) { // checks if any key is pressed
      option = getch(); // catch input in options variable
      if (option == KEY_UP) {
        dir = 0; // up
      } else if (option == KEY_RIGHT) {
        dir = 1; // right
      } else if (option == KEY_DOWN) {
        dir = 2; // down
      } else if (option == KEY_LEFT) {
        dir = 3; // left
      }
    }

    // Drawing enemies
    if (pathIndex > 0) { // skip first index

      for (int i = 0; i < ei; i++) {
        int tmpEnemyLoc = enemy[i][2];
        enemy[i][0] = path[tmpEnemyLoc][0]; // update enemy x
        enemy[i][1] = path[tmpEnemyLoc][1]; // update enemy y
        enemy[i][2]++; //update enemy position index

        // draw enemy
        setcolor(LIGHTRED);
        setfillstyle(1, LIGHTRED);
        fillellipse(enemy[i][0], enemy[i][1], eSize, eSize);
      }

    }

    // Generate new enemy if timer reaches 30
    if (eGen == 30) {
      enemy[ei][2] = 0;
      ei++;
      eGen = 0;
    }
    eGen++; // update enemy generation timer

    // if player and fruit collide
    if (EnemyHitPlayer() == 1) {
      GameOver();

      // reset variables
      score = 0;
      pathIndex = 0;
      eGen = 0;
      ei = 0;
    }

    // if player and fruit collide
    if (PlayerHitFruit() == 1) {
      score++; // increase score
      fy = 150 + rand() % 400; // set fruit to a new random position
    }

    // add new player position in path
    path[pathIndex][0] = px;
    path[pathIndex][1] = py;

    // update player position
    if (dir == 0) py -= step;
    else if (dir == 1) px += step;
    else if (dir == 2) py += step;
    else if (dir == 3) px -= step;

    // restrict player from going outside boundry
    if (px <= 0) px = 0;
    if (py <= 0) py = 0;
    if (px + pSize >= 1000) px = 1000 - pSize;
    if (py + pSize >= 700) py = 700 - pSize;

    // do not generate path index variable if player is stegnant
    if (pathIndex != 0) {
      if (path[pathIndex][0] == px && path[pathIndex][1] == py) {
        pathIndex--;
      }
    }

    pathIndex++;
    delay(40);
    cleardevice();
  }

  getch();
  closegraph();
}

void GameOver() {
  cleardevice(); // clear screen

  setcolor(WHITE);
  // Print Game Over Text
  settextstyle(4, 0, 9);
  outtextxy(300, 300, "Game Over");

  settextstyle(4, 0, 2);
  outtextxy(500, 420, "Press any key to restart...");
  getch();
  score = 0; // reset score
}

void DrawBoard() {

  setcolor(WHITE);

  // draw board
  rectangle(0, 0, ScWidth - OpWidth, ScHeight);
  rectangle(ScWidth - OpWidth, 0, ScWidth, ScHeight);

  // draw title
  settextstyle(4, 0, 4);
  outtextxy(ScWidth - 340, 50, "Foot Steps");

  // print score
  // itoa function converts number to character array
  settextstyle(4, 0, 9);
  if (score < 10)
    outtextxy(ScWidth - 270, 250, itoa(score, txtScore, 10));
  else if (score >= 10)
    outtextxy(ScWidth - 320, 250, itoa(score, txtScore, 10));

  settextstyle(4, 0, 1);
  outtextxy(ScWidth - 375, 500, "Controls: ");
  outtextxy(ScWidth - 300, 540, "- Arrow Keys");
}

// returns 1 if enemy hits player
int EnemyHitPlayer() {
  for (int i = 0; i < ei; i++) {
    // Check horizontal collision
    if (enemy[i][0] >= px && enemy[i][0] <= px + pSize) {
      // Check vertical collision
      if (enemy[i][1] >= py && enemy[i][1] <= py + pSize) {
        delay(500);
        return 1;
      }
    }
  }
  return 0;
}

// return 1 if player collects fruit
int PlayerHitFruit() {

  // Check horizontal collision
  if (fx >= px && fx <= px + pSize) {
    // Check vertical collision
    if (fy >= py && fy <= py + pSize) {
      return 1;
    }
  }

  return 0;
}
