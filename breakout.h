#include "colors.h"
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;

#define WIDTH tft.width()
#define HEIGHT tft.height()
#define XSPEED 2
#define YSPEED 3

#include "point.h"
#include "paddle.h"
#include "brick.h"
#include "ball.h"
#include "board.h"

void printResult(const char *s, int y) {
    int len = strlen(s) * 6;
    tft.setTextColor(WHITE, RED);
    tft.setCursor((WIDTH - len) / 2, y);
    tft.print(s);
}
