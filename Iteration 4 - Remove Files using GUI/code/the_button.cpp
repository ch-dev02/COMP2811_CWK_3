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
        QAction action3("Remove", this);
        connect(&action3, SIGNAL(triggered()), this, SLOT(remove()));
        contextMenu.addAction(&action3);

        contextMenu.exec(e->globalPos());
    }else if (e->button()==Qt::LeftButton)
        emit leftClicked(this->index);
}

void TheButton::remove(){
    emit removeVideo(index);
}
