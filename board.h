/* BOARD */

typedef struct {
  Brick *bricks;
  Ball *ball;
  Paddle *paddle;
  uint8_t brick_count;
} Board;

void loadPaddle(Board *board) {
  board->paddle = (Paddle *)malloc(sizeof(Paddle));
  board->paddle->color = WHITE;
  board->paddle->w = WIDTH / 8;
  board->paddle->h = HEIGHT / 30;
  board->paddle->x = (WIDTH / 2) - (board->paddle->w / 2);
  board->paddle->y = HEIGHT - (board->paddle->h * 2);
  board->paddle->oldx = board->paddle->x;
}

void loadBall(Board *board) {
  board->ball = (Ball *)malloc(sizeof(Ball));
  board->ball->color = WHITE;
  board->ball->d = 8;
  board->ball->x = board->paddle->x + (board->paddle->w / 2);
  board->ball->y = board->paddle->y - board->paddle->h;
  board->ball->oldx = board->ball->x;
  board->ball->oldy = board->ball->y;
  board->ball->xspeed = 0;
  board->ball->yspeed = 0;
  board->ball->lives = 3;
}

// populate Board.bricks linked list with bricklist
void loadBricks(Board *board) {
  uint16_t bricklist[][4][2] = {
          // col 0          1           2           3
/*row 0*/ { {RED,    1}, {ORANGE, 1}, {YELLOW, 1}, {GREEN,  1} },
/*    1*/ { {ORANGE, 1}, {YELLOW, 1}, {GREEN,  1}, {BLUE,   1} },
/*    2*/ { {YELLOW, 1}, {GREEN,  1}, {BLUE,   1}, {MAGENTA,1} },
/*    3*/ { {GREEN,  1}, {BLUE,   1}, {MAGENTA,2}, {BLUE,   1} },
/*    4*/ { {BLUE,   1}, {MAGENTA,1}, {BLUE,   1}, {GREEN,  1} },
/*    5*/ { {MAGENTA,2}, {BLUE,   3}, {GREEN,  1}, {YELLOW, 2} },
/*    6*/ { {BLUE,   1}, {GREEN,  1}, {YELLOW, 1}, {ORANGE, 1} },
/*    7*/ { {GREEN,  1}, {YELLOW, 1}, {ORANGE, 3}, {RED,    1} },
/*    8*/ { {YELLOW, 1}, {ORANGE, 1}, {RED,    1}, {ORANGE, 1} },
/*    9*/ { {BLACK,  0}, {RED   , 1}, {ORANGE, 1}, {BLACK,  0} }
  };
  
  board->bricks = (Brick *)malloc(sizeof(Brick));

  if (board->bricks != NULL) {
    Brick *pbrick = NULL;
    uint8_t cols = sizeof(bricklist[0]) / sizeof(bricklist[0][0]); // determine size of bricklist array
    uint8_t bricks = sizeof(bricklist) / sizeof(bricklist[0][0]);
    uint8_t rows = bricks / cols;
    uint8_t bw = WIDTH / cols;
    uint8_t bh = HEIGHT / 15;
    
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        uint16_t color = bricklist[i][j][0];
        uint8_t hits = bricklist[i][j][1];

        Brick *brick = makeBrick( pbrick, j * bw, i * bh, bw, bh, color, hits );

        if (pbrick != NULL) {
          pbrick->next = brick;
        } else {
          brick->prev = NULL;
          board->bricks = brick;
        }
        pbrick = brick;
      }
    }
  }
}

void ballCollision(Board *board) {
  Ball *ball = board->ball;
  Paddle *paddle = board->paddle;
  Brick *brick = board->bricks;
  // EDGE
  if ( !edgeCollision(ball) ) { // if we died
    ball->lives -= 1;
    if ( !ball->lives )
      board->brick_count = 0;
    
    paddle->x = WIDTH / 2 - paddle->w / 2;
    ball->x = paddle->x + paddle->w / 2;
    ball->y = paddle->y - paddle->h;

    tft.fillScreen(BLACK);
    drawBall(ball);
    drawPaddle(paddle);
    drawBricks(board->bricks);
  }
  // PADDLE
  paddleCollision(ball, paddle);
  //BRICK
  // Brick collision
  while (brick != NULL) {
    if ( brickCollision(ball, brick) ) {
      if (brick->hits)
        drawBrick(brick);
      if (brick->hits < 1)  // if brick destroyed
        removeBrick(brick);
      board->brick_count--;
    }
    brick = brick->next;
  }
}

void initBoard(Board *board) {
  loadBricks(board);
  loadPaddle(board);
  loadBall(board);
}

void drawBoard(Board *board, bool start) {
  Ball *ball = board->ball;
  Paddle *paddle = board->paddle;
  if (start) {
    board->brick_count = countBricks(board->bricks);
    drawBall(ball);
    drawPaddle(paddle);
    drawBricks(board->bricks);
  } else {
    if (ball->oldx != ball->x || ball->oldy != ball->y)
        drawBall(ball);
    if (paddle->oldx != paddle->x)
      drawPaddle(paddle);
  }
}
