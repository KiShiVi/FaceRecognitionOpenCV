#pragma once

#include <QLabel>
#include <QPainter>

class BarChart  : public QLabel
{
	Q_OBJECT

public:
    BarChart(QWidget* parent=nullptr);
	~BarChart();

    void DRAW(int x, int x2, int y, int y2)
    {
        QPainter p(this);
        QPen myPen;
        myPen.setColor(Qt::black);
        myPen.setWidth(5);

        p.setPen(myPen);
        _x = x;
        _x2 = x2;
        _y = y;
        _y2 = y2;
        update();
    }

    int _x, _x2, _y, _y2;

    virtual void paintEvent(QPaintEvent* e) {
        QLabel::paintEvent(e);
        QPainter p(this);
        QPen myPen;
        myPen.setColor(Qt::black);
        myPen.setWidth(5);

        p.setPen(myPen);
        p.drawLine(_x, _x2, _y, _y2);
        p.drawLine(0, 0, 0, 300);
    }
};
