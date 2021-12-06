#include <QMenu>
#include <QMenuBar>
#include "the_menu.h"

TheMenu::TheMenu(QWidget *parent)
    : QMainWindow(parent) {

  QPixmap openpix("open.png");
  QPixmap quitpix("quit.png");

  auto *open = new QAction(openpix, "&Open", this);
  auto *quit = new QAction(quitpix, "&Quit", this);
  quit->setShortcut(tr("CTRL+Q"));

  QMenu *file = menuBar()->addMenu("&File");
    file->addAction(open);
    file->addSeparator();
    file->addAction(quit);
  qApp->setAttribute(Qt::AA_DontShowIconsInMenus, false);
  connect(quit, &QAction::triggered, qApp, QApplication::quit);
}
