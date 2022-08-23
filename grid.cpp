#include "grid.h"

Grid::Grid(int _x, int _y) : x(_x), y(_y) {
}

void Grid::mousePressEvent(QGraphicsSceneMouseEvent*) {
  click();
}

void Grid::click() {
  if( clicked != -1) return;
  emit gridClicked(x, y);
}

void Grid::select(bool status) {
  if( status ) {
    this->setBrush(QBrush(Qt::green));
  } else {
    this->setBrush(QBrush(Qt::white));
  }
}



