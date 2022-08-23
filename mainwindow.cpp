#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent) {
  init();
}

void MainWindow::init(void) {
  /* mouse */
  view  = new QGraphicsView(this);
  scene = new QGraphicsScene();
  setMouseTracking(true);

  /* sound & music */
  sound = new QSoundEffect();
  sound->setSource(QUrl("qrc:/sound/sound.wav"));
  sound->setVolume(0.2);

  player = new QMediaPlayer();
  player->setMedia(QUrl("qrc:/sound/music.mp3"));
  player->setVolume(10);

  /* chess board */
  brush.setStyle(Qt::BrushStyle::SolidPattern);
  view->setScene(scene);
  view->setSceneRect(0, 0, LENGTH, LENGTH);
  scene->installEventFilter(this);

  /* timer */
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(count()));

  timeboard = new QLabel();
  timeboard->setText("倒计时:" + QString::number(counter) + "s / " + QString::number(MAX_TIME) +  "s");
  timeboard->setAlignment(Qt::AlignLeft);

  /* button */
  button = new QPushButton();
  button->setText("开始游戏");
  connect(button, SIGNAL(clicked()), this, SLOT(startGame()));

  /* file-button */
  readfile = new QPushButton();
  readfile->setText("读取文件");
  connect(readfile, SIGNAL(clicked()), this, SLOT(readFromFile()));

  writefile = new QPushButton();
  writefile->setText("写入文件");
  connect(writefile, SIGNAL(clicked()), this, SLOT(saveToFile()));

  /* message */
  message = new QLabel();
  message->setText("点击开始");

  /* checkbox */
  checkbox = new QCheckBox();
  checkbox->setText("关闭背景音乐");
  connect(checkbox, SIGNAL(stateChanged(int)), this, SLOT(mute(int)));
  checkbox->setChecked(false);

  /* layout */
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(timeboard);
  layout->addWidget(view);
  layout->addWidget(button, Qt::AlignCenter);

  layout->addWidget(readfile);
  layout->addWidget(writefile);


  QHBoxLayout *hlayout = new QHBoxLayout();
  hlayout->addWidget(message, Qt::AlignRight);
  hlayout->addWidget(checkbox, Qt::AlignLeft);

  layout->addLayout(hlayout);

  ani = new Ani();
  layout->addWidget(ani);
  ani->show();

  widget = new QWidget();
  widget->setLayout(layout);
  widget->installEventFilter(this);
  widget->setFocusPolicy(Qt::NoFocus);
  setCentralWidget(widget);
}

void MainWindow::mute(int status) {
  player->setMuted(status != Qt::Unchecked);
}

void MainWindow::count() {
  counter--;
  if( counter >= 0 ) {
    timeboard->setText("倒计时:" + QString::number(counter) + "s / " + QString::number(MAX_TIME) +  "s");
  } else {
    gameover(-1);
  }
}

void MainWindow::drawChess(int x, int y) {
  /* sound */
  sound->play();

  /* put chess */
  move(x, y);
  map[x][y]->clicked = turn;

  if( turn ) brush.setColor(QColor("red"));
  else brush.setColor(QColor("blue"));

  QGraphicsEllipseItem* chess = new QGraphicsEllipseItem();
  chess->setRect( (x * LENGTH / N) + R() / 2.0,   ( y * LENGTH / N) + R() / 2.0, R(), R());
  chess->setBrush(brush);
  scene->addItem(chess);

  /* check game */
  if(check(x, y)) return;

  /* modify message */
  turn ^= 1;
  message->setText(getname(turn) + "的回合");

  counter = MAX_TIME;
  timer->start(1000);
  timeboard->setText("倒计时:" + QString::number(counter) + "s / " + QString::number(MAX_TIME) +  "s");
}

void MainWindow::putChess(int x, int y, int t) {
  /* put chess */
  map[x][y]->clicked = t;

  if( t ) brush.setColor(QColor("red"));
  else brush.setColor(QColor("blue"));

  QGraphicsEllipseItem* chess = new QGraphicsEllipseItem();
  chess->setRect( (x * LENGTH / N) + R() / 2.0,   ( y * LENGTH / N) + R() / 2.0, R(), R());
  chess->setBrush(brush);
  scene->addItem(chess);
}

/* event */
bool MainWindow::eventFilter(QObject * object, QEvent * event) {
  if(event->type() == QEvent::GraphicsSceneMousePress) {
    if( gamestatus != 1 ) return true;
  }
  if(event->type() == QEvent::KeyPress ) {
    if( gamestatus != 1 ) return true;
    keyPressEvent(static_cast<QKeyEvent*>(event));
    return true;
  }
  view->setFocus();
  return QMainWindow::eventFilter(object, event);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {

  if( event->key() == Qt::Key_Left )
    move( (sx - 1 + N) % N, sy );
  if( event->key() == Qt::Key_Right )
    move( (sx + 1) % N, sy );
  if( event->key() == Qt::Key_Up )
    move( sx, (sy - 1 + N) % N );
  if( event->key() == Qt::Key_Down )
    move( sx, (sy + 1 ) % N );
  if( event->key() == Qt::Key_Space)
    map[sx][sy]->click();

}

/* game control: start, pause, gameover(restart) */
void MainWindow::startGame() {
  player->play();

  scene->clear();
  view->setScene(scene);

  map.resize(N);
  for(auto &v : map) v.resize(N);

  for(int i = 0; i < N ; i++)
    for(int j = 0; j < N; j++) {
      map[i][j] = new Grid(i, j);
      map[i][j]->setRect( (1.0 * i * LENGTH / N), ( 1.0 * j * LENGTH / N), 1.0 * LENGTH / N - 1, 1.0 * LENGTH / N - 1);

      connect(map[i][j], SIGNAL(gridClicked(int, int)), this, SLOT(drawChess(int, int)));
      scene->addItem(map[i][j]);
    }

  /* button */
  button->setText("暂停游戏");
  button->disconnect();
  connect(button, SIGNAL(clicked()), this, SLOT(pauseGame()));

  /* game info */
  turn = 0;
  gamestatus = 1;
  message->setText(getname(turn) + "的回合");

  counter = MAX_TIME;
  timer->start(1000);
  timeboard->setText("倒计时:" + QString::number(counter) + "s / " + QString::number(MAX_TIME) +  "s");

  move(0, 0);
}

void MainWindow::pauseGame() {
  if( gamestatus == 1 ) {
    player->pause();
    gamestatus = 2;
    timer->stop();
    button->setText("继续游戏");
    message->setText("游戏已暂停");
  } else {
    player->play();
    gamestatus = 1;
    timer->start(1000);
    button->setText("暂停游戏");
    message->setText(getname(turn) + "的回合");
  }
}

void MainWindow::gameover(int status) {
  gamestatus = -1;

  timer->stop();
  button->disconnect();
  button->setText("重新开始");
  connect(button, SIGNAL(clicked()), this, SLOT(startGame()));

  if( status == -1 ) {
    move(-1, -1);
    message->setText(getname(turn) + "超时," + getname(turn ^ 1) + "获胜");
  }
  if( status == 1 ) {
    message->setText(getname(turn) + "获胜");
  }
}

/* game logic */

bool MainWindow::isvalid(int x, int y) {
  if( x < N && x >= 0 && y < N && y >= 0 ) return true;
  return false;
}

QString MainWindow::getname(int x) {
  if( x == 0 ) return "先手";
  return "后手";
}

bool MainWindow::check(int x, int y) {
  int px[] = { 1, 0, 1, 1};
  int py[] = { 0, 1, 1, -1};

  for(int d = 0; d < 4; d++) {
    int len = 0;
    for(int i = - M + 1; i < M ; i ++ ) {
      if( isvalid(x + i * px[d], y + i * py[d]) && map[x + i * px[d]][y + i * py[d]]->clicked == turn ) {
        len ++;
        if( len == M ) {
          for(int j = i; j > i - M; j--) {
            map[x + j * px[d]][ y + j * py[d]]->select(true);
          }
          sx = x;
          sy = y;
          gameover(1);
          return true;
        }
      } else len = 0;
    }
  }
  return false;

}

void MainWindow::move(int x, int y) {
  if( isvalid(sx, sy)) map[sx][sy]->select(false);
  sx = x;
  sy = y;
  if( isvalid(sx, sy) ) map[sx][sy]->select(true);
}

/* file */
void MainWindow::readFromFile() {
  QFile* file = new QFile("./file.json");
  if( !file->exists() ) {
    message->setText("文件不存在");
    return;
  }
  file->open(QIODevice::ReadOnly);
  QByteArray bytes = file->readAll();
  file->close();

  QJsonDocument doc = QJsonDocument::fromJson(bytes);
  QJsonObject info = doc.object();
  N = info.value("N").toInt();
  M = info.value("M").toInt();
  QJsonArray array = info.value("chess").toArray();

  startGame();

  foreach(QJsonValue v, array) {
    QJsonObject obj = v.toObject();
    int x = obj.value("x").toInt();
    int y = obj.value("y").toInt();
    int clicked = obj.value("clicked").toInt();
    putChess(x, y, clicked);
  }

  turn = info.value("turn").toInt();
  message->setText(getname(turn) + "的回合");
  move(info.value("sx").toInt(), info.value("sy").toInt());
  check(sx, sy);
}

void MainWindow::saveToFile() {
  QFile* file = new QFile("./file.json");
  file->open(QIODevice::WriteOnly);
  QJsonArray array;
  for(auto& v : map)
    for(auto& g : v) {
      if( g->clicked != -1 ) {
        QJsonObject o;
        o.insert("x", g->x);
        o.insert("y", g->y);
        o.insert("clicked", g->clicked);
        array.append(o);
      }
    }
  QJsonObject info;
  info.insert("chess", array);
  info.insert("N", N);
  info.insert("M", M);
  info.insert("turn", turn);
  info.insert("sx", sx);
  info.insert("sy", sy);
  QJsonDocument doc(info);
  QByteArray bytes = doc.toJson();
  file->write(bytes);
  file->close();
}

MainWindow::~MainWindow() {
}

