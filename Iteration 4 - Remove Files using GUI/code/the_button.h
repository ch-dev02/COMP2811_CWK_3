//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_BUTTON_H
#define CW2_THE_BUTTON_H

#include <QPushButton>
#include <QUrl>
#include <QMouseEvent>
#include <QCursor>
#include <QPoint>
#include <QMenu>
#include <QAction>

class TheButtonInfo {

public:
    QString url; // video file to play

    TheButtonInfo (QString url) : url (url) {}
};

class TheButton : public QPushButton {

    Q_OBJECT

public:
    TheButton(const QString &text, int indx);
    int index = -1;

private slots:
    void mousePressEvent(QMouseEvent *e);
    void remove();

signals:
    void removeVideo(int index);
    void leftClicked(int index);
};

#endif //CW2_THE_BUTTON_H
