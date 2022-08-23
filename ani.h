#ifndef ANI_H
#define ANI_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QSize>
#include <QTimer>

class Ani : public QWidget {
  Q_OBJECT
 public:
  Ani(QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent* e) override;

 signals:

 private:
  QTimer* timer;
  QPixmap* pixmap;

  const int N = 2;
  const int M = 4; // 2 * 4 atlas
  int n = 0, m = 0;
  int pos = -100;
  QSize size;

};

#endif // ANI_H
