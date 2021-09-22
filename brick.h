/* BRICK */

typedef struct Brick {
  Brick *prev;
  Brick *next;
  uint16_t x, y;
  uint8_t  w, h;
  uint16_t color;
  int8_t  hits, old_hits;
} Brick;

uint8_t countBricks(Brick *brick) {
  uint8_t count = 0;
  while (brick) { 
    if ( brick->hits > 0 )
      ++count;

    brick = brick->next;
  }
  return count;
}

void drawBrick(Brick *brick) {
  tft.fillRect( brick->x, brick->y,
                brick->w, brick->h,
                brick->hits > 1 ? (brick->hits > 2 ? BLACK : WHITE) : brick->color );
  if (brick->hits) {    
    tft.drawRect( brick->x, brick->y,
                  brick->w, brick->h,
                  WHITE );
    tft.drawRect( brick->x + brick->hits * 3, brick->y + brick->hits * 3,
                  brick->w - brick->hits * 6, brick->h - brick->hits * 6,
                  WHITE );
  }
}

void drawBricks(Brick *brick) {
  while (brick) { 
    drawBrick(brick);
    brick = brick->next;
  }
}

Brick *makeBrick( Brick *pbrick, uint16_t x, uint16_t y, uint8_t w, uint8_t h, uint16_t color, int8_t hits ) {
  Brick *brick = (Brick *)malloc(sizeof(Brick));
  brick->next = NULL;
  brick->prev = pbrick;
  brick->x = x;
  brick->y = y;
  brick->w = w;
  brick->h = h;
  brick->color = color;
  brick->hits = hits;
  brick->old_hits = 0;

  /*brick->left   = { { x, y   }, { x, y+h   } }; // LEFT
  brick->right  = { { x+w, y }, { x+w, y+h } }; // RIGHT
  brick->top    = { { x, y   }, { x+w, y   } }; // TOP
  brick->bottom = { { x, y+h }, { x+w, y+h } }; // BOTTOM*/

  return brick;
}

void removeBrick(Brick *brick) {
  if (brick->next != NULL)
    brick->next->prev = (brick->prev == NULL) ? NULL : brick->prev;
  if (brick->prev != NULL)
    brick->prev->next = (brick->next == NULL) ? NULL : brick->next;
  brick->color = BLACK;
  drawBrick(brick);
  brick = NULL;
}
