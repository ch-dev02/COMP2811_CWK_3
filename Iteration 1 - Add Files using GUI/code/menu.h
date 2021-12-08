#ifndef THE_MENU_H
#define THE_MENU_H

#include <QMainWindow>
#include <QApplication>
#include <QPlainTextEdit>
#include "the_player.h"
#include "the_button.h"
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>

class TheMenu :public QMainWindow{

  public:
    ThePlayer *player;
    std::vector<TheButton*> buttons;
    std::vector<TheButtonInfo> videos;
    QWidget* buttonWidget;
    QHBoxLayout *layout;
    TheMenu(ThePlayer *player,std::vector<TheButton*> buttons,std::vector<TheButtonInfo> videos,QHBoxLayout *layout,QWidget *parent = nullptr);
    void loadFile(const QString &fileName);

   private slots:
    void openFile();
    void openFolder();
    std::vector<TheButtonInfo> getInfoIn (std::string loc);

   private:
    void setCurrentFile(const QString &fileName);
//    QPlainTextEdit *textEdit;
//    QString curFile;
};

#endif // THE_MENU_H
