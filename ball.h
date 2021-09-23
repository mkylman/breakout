/* BALL */

typedef struct {
  uint16_t color;
  int16_t  x, y;
  uint16_t oldx, oldy;
  float    xspeed, yspeed;
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

void launchBall( Ball *ball ) {
  delay(200);
  Point p = {0,false};
  while ( !p.touched ) {
    p = getPoint();
    if ( p.touched ) {
      double x1, x2, y1, y2;
      double a, b, c;
      
      x1 = p.x;
      y1 = p.y;
      while ( p.touched ) {
        a = p.x - x1;
        b = p.y - y1;
        c = sqrt( (a*a) + (b*b) );
        c = c ? c : 0.1;
        
        x2 = ball->x + (a / c) * 100;
        y2 = ball->y + (b / c) * 100;
        
        tft.drawLine( ball->x, ball->y, x2, y2, WHITE );
        tft.drawLine( ball->x, ball->y, x2, y2, BLACK );
        
        p = getPoint();
      }
      
      ball->xspeed = a / c * 4;
      ball->yspeed = b / c * 4;
      if (ball->xspeed && ball->yspeed) break;
    }
  }
}

void ballMove(Ball *ball) {
  if (!ball->xspeed && !ball->yspeed) { // if ball is not moving
    launchBall( ball );
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
    //ball->xspeed = ball->xspeed ? ball->xspeed : XSPEED;
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
  double a = ball->x - paddle->x + (paddle->w/2);
  double b = paddle->y + (paddle->h/2) - ball->y;
  float  c = sqrt( (a*a) + (b*b) );
  c = c ? c : 0.1;
  if (ball->x < paddle->x + paddle->w && ball->x           > paddle->x
  &&  ball->y < paddle->y + paddle->h && ball->y + ball->d > paddle->y) {
    if (ball->xspeed > 0)
      ball->xspeed = a / c * 4;
    else
      ball->xspeed = (a / c) * -4;
    ball->yspeed = b / c * 4;
    ball->yspeed *= -1;
    
    if (ball->yspeed == 0)
      ball->yspeed = -1;
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
