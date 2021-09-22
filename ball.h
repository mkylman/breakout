/* BALL */

typedef struct {
  uint16_t color;
  int16_t  x, y;
  uint16_t oldx, oldy;
  int8_t   xspeed, yspeed;
  uint8_t  d; // diameter
  int8_t   lives;
} Ball;

void drawBall(Ball *ball) {
  tft.fillCircle( ball->oldx, ball->oldy,
                  ball->d/2,
                  BLACK );
  tft.fillCircle( ball->x, ball->y,
                  ball->d/2,
                  ball->color );
}

void ballMove(Ball *ball) {
  if (!ball->xspeed && !ball->yspeed) { // if ball is not moving
    if ( touched() ) {                   // if touch, launch ball
      ball->xspeed = getXP() > WIDTH / 2 ? XSPEED : -XSPEED;
      ball->yspeed = -YSPEED;
    }
  }
  ball->oldx = ball->x;
  ball->oldy = ball->y;
  ball->x += ball->xspeed;
  ball->y += ball->yspeed;
}

uint8_t edgeCollision(Ball *ball) { // returns 0 if die, 1 if regular collision
  // Edge collision
  if (ball->x <= ball->d) {         // left side
    ball->x += ball->d;
    ball->xspeed *= -1;
  }
  if (ball->x >= WIDTH - ball->d) { // right side
    ball->x -= ball->d;
    ball->xspeed *= -1;
  }
  if (ball->y <= ball->d) {         // top side
    ball->y += ball->d;
    ball->yspeed *= -1;
    ball->xspeed = ball->xspeed ? ball->xspeed : XSPEED;
  }
  if (ball->y > HEIGHT - ball->d) { // if ball hits bottom, reset
    ball->yspeed = 0;
    ball->xspeed = 0;

    return 0;
  }
  return 1;
}

void paddleCollision(Ball *ball, Paddle *paddle) {
  // Paddle collision
  if (ball->x < paddle->x + paddle->w && ball->x           > paddle->x
  &&  ball->y < paddle->y + paddle->h && ball->y + ball->d > paddle->y) {
    ball->y -= ball->d;
    ball->yspeed = -YSPEED;
    uint8_t pthird = paddle->w / 3;
    if (ball->xspeed == 0) {
      ball->xspeed = -XSPEED;
    }
    if ((ball->x < paddle->x + pthird   && ball->xspeed > 0) //left side
    ||  (ball->x > paddle->x + pthird*2 && ball->xspeed < 0)) {//right side
      ball->xspeed *= -1;
    } else if (ball->x > paddle->x + pthird
            && ball->x < paddle->x + pthird*2) { //center
      ball->xspeed = 0;
    }
  }
}

bool brickCollision(Ball *ball, Brick *brick){

  uint8_t  bx = ball->x + (ball->d / 2);
  uint16_t by = ball->y + (ball->d / 2);

  if ( by > brick->y && by < brick->y + brick->h && bx > brick->x && bx < brick->x + brick->w) {
    brick->old_hits = brick->hits;
    
    if (brick->hits > 0) {
      brick->hits -= 1;
      if ( (bx > brick->x && bx < brick->x + ball->d ) || // LEFT
           (bx > brick->x + brick->w && bx < brick->x + brick->w + ball->d ) ) // RIGHT
      {
        ball->xspeed *= -1;
        ball->yspeed *= -1;
      }
      
      ball->xspeed = ball->xspeed ? ball->xspeed : (millis() % 2 ? XSPEED : -XSPEED);
      ball->yspeed *= -1;
    }
    return true;
  } else return false;
}
