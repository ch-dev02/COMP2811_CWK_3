#ifndef THE_SLIDER_H
#define THE_SLIDER_H

#include <QSlider>

class TheSlider : public QSlider {

Q_OBJECT

public:
    TheSlider() : QSlider(Qt::Horizontal) {}
};

#endif // THE_SLIDER_H
