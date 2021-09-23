#include "breakout.h"

Board board;

void setup() {
  tft.begin(tft.readID());
  //Serial.begin(9600);

  initBoard(&board);
  
  tft.fillScreen(BLACK);
}

void(* resetFunc)(void) = 0;

void loop() {

  Point p = {0,false};
  while ( !p.touched ) {
    printResult("*TOUCH to PLAY*", HEIGHT/2);
    p = getPoint();
  }

  tft.fillScreen(BLACK);
  drawBoard(&board, true);

  unsigned long ms = millis();
  
  while(board.brick_count && board.ball->lives){
    uint8_t old_count = board.brick_count;
    board.brick_count = countBricks(board.bricks);

    ballCollision(&board);

    if ( millis() - ms > (1000 / 60) ) { // every 60th of a second
      ms = millis();
      drawBoard(&board, false);
      ballMove(board.ball);
      paddleMove(board.paddle);

      if (board.ball->yspeed <= 1 && board.ball->yspeed >= -1)
        board.ball->yspeed = board.ball->yspeed > 0.0 ? 2 : -2;
    }
  }

  tft.fillScreen(BLACK);

  p.touched = false;
  while ( !p.touched ) {
    printResult(board.ball->lives ? "***WINNER***" : "***LOSER***", HEIGHT/2);
    p = getPoint();
  }
  
  delay(2000);
  resetFunc();
  
}
