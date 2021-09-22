/* PADDLE */

typedef struct {
  uint16_t color;
  uint16_t x, y;
  uint16_t oldx;
  uint8_t w, h;
} Paddle;

void drawPaddle(Paddle *paddle) {
  tft.fillRect( paddle->oldx, paddle->y,
                paddle->w, paddle->h,
                BLACK );
  tft.fillRect( paddle->x, paddle->y,
                paddle->w, paddle->h,
                paddle->color );
}

void paddleMove(Paddle *paddle) {
  Point tp = getPoint();
  paddle->oldx = paddle->x;
  if (tp.touched) {
    paddle->x = tp.x - (paddle->w / 2);
  }
}
