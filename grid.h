#ifndef GRID_H
#define GRID_H

#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QSoundEffect>

class Grid : public QObject, public QGraphicsRectItem {
  Q_OBJECT
 public:
  Grid(int, int);
  void click();
  int x, y;
  int clicked = -1; // -1: unclicked, 0/1: chess

 public slots:
  void select(bool);

 protected:
  void mousePressEvent(QGraphicsSceneMouseEvent* ) override;

 signals:
  void gridClicked(int, int);
};


#endif // GRID_H
