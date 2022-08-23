#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "grid.h"
#include "ani.h"

#include <QMainWindow>
#include <QGridLayout>
#include <QString>
#include <QPushButton>
#include <QGraphicsPixmapItem>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QColor>
#include <QTimer>
#include <QMouseEvent>
#include <QLabel>
#include <QVector>
#include <QMediaPlayer>
#include <QCheckBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 public slots:
  void drawChess(int, int); // put a chess on (int, int)
  void count(); // change time counter
  void startGame(); // start new game
  void pauseGame(); // pause (or resume) game
  void move(int, int); // move select grid to (int, int)
  void mute(int); // mute background music
  void readFromFile();
  void saveToFile();

 protected:
  bool eventFilter(QObject*, QEvent*) override;
  void keyPressEvent(QKeyEvent*) override;

 private:
  int N = 7; // N * N board
  int M = 3; // M chesses to win
  const int LENGTH = 300; // view length * length
  int R() {
    return (LENGTH / N) / 2.0;
  }
  const int MAX_TIME = 20; // max time

  QVector<QVector<Grid*>> map;

  int turn = 0; // 0: first, 1: second
  int counter = MAX_TIME; // time counter
  int gamestatus = 0; // 0: init, 1: playing, 2: pausing, -1: end

  int sx = 0, sy = 0; // current select grid (sx, sy)

  QWidget* widget;

  QLabel* timeboard;
  QTimer* timer;

  QGraphicsScene* scene;
  QGraphicsView* view;
  QBrush brush;

  QPushButton* button;
  QPushButton* readfile;
  QPushButton* writefile;

  QLabel* message;

  QCheckBox* checkbox;

  QSoundEffect* sound; // sound effect
  QMediaPlayer* player; // bgm music player

  Ani* ani;

  void init(); // init widget (layout)
  void gameover(int); // params: 1 (goal), -1(time exceeded)
  bool check(int, int); // last chess on (int, int), check if win
  bool isvalid(int, int); // check if (int, int) is a valid position
  void putChess(int, int, int); // put a chess on (int, int) by (turn) from file

  QString getname(int);
};
#endif // MAINWINDOW_H
