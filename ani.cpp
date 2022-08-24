#include "ani.h"

Ani::Ani(QWidget *parent)
  : QWidget{parent} {

  timer = new QTimer();
  timer->start(100);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));

  pixmap = new QPixmap(":/image/animal.png");
  size = pixmap->size();

  setFixedHeight(size.height() / 2);
}

void Ani::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  m += 1;
  if( m == M ) {
    n = (n + 1) % N;
    m = 0;
  }

  pos -= 10;
  if( pos < -10 ) pos = width() + 10;

  painter.drawPixmap(pos - 100, 0, *pixmap,
                     m * size.width() / M, n * size.height() / N,  size.width() / M, size.height() / N);
}
