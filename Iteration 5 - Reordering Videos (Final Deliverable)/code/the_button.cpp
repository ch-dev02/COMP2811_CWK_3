#include "the_button.h"
#include <QDebug>

TheButton::TheButton(const QString &text, int indx) :
    QPushButton(text)
{
    this->index = indx;
}

void TheButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::RightButton){
        QMenu contextMenu("Context menu", this);

        QAction action1("Move Up", this);
        connect(&action1, SIGNAL(triggered()), this, SLOT(moveUp()));
        contextMenu.addAction(&action1);
        QAction action2("Move Down", this);
        connect(&action2, SIGNAL(triggered()), this, SLOT(moveDown()));
        contextMenu.addAction(&action2);
        QAction action3("Remove", this);
        connect(&action3, SIGNAL(triggered()), this, SLOT(remove()));
        contextMenu.addAction(&action3);

        contextMenu.exec(e->globalPos());
    }else if (e->button()==Qt::LeftButton)
        emit leftClicked(this->index);
}

void TheButton::moveUp(){
    emit moveVideoUp(index);
}

void TheButton::moveDown(){
    emit moveVideoDown(index);
}

void TheButton::remove(){
    emit removeVideo(index);
}
