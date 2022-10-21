#pragma once

#include <QLabel>
#include <QPainter>

class BarChart  : public QLabel
{
	Q_OBJECT

public:
    BarChart(QWidget* parent=nullptr);
	~BarChart();

    void draw(QList<QList<QList<float>>> _barCharts)
    {
        barCharts = _barCharts;
        update();
    }

    int _x, _x2, _y, _y2;

    virtual void paintEvent(QPaintEvent* e) {
        QLabel::paintEvent(e);
        QPainter p(this);
        QPen myPen;

        if (!barCharts.isEmpty())
        for (int i = 0; i < barCharts[0].size(); ++i)
        {
            myPen.setColor(Qt::red);
            myPen.setWidth(2);
            p.setPen(myPen);
            p.drawLine(barCharts[0][i][0], barCharts[0][i][1], barCharts[0][i][2], barCharts[0][i][3]);
            myPen.setColor(Qt::green);
            myPen.setWidth(2);
            p.setPen(myPen);
            p.drawLine(barCharts[1][i][0], barCharts[1][i][1], barCharts[1][i][2], barCharts[1][i][3]);
            myPen.setColor(Qt::blue);
            myPen.setWidth(2);
            p.setPen(myPen);
            p.drawLine(barCharts[2][i][0], barCharts[2][i][1], barCharts[2][i][2], barCharts[2][i][3]);
        }
        myPen.setColor(Qt::black);
        myPen.setWidth(3);
        p.setPen(myPen);
        p.drawLine(0, 0, width(), 0);
        p.drawLine(width(), 0, width(), height());
        p.drawLine(width(), height(), 0, height());
        p.drawLine(0, height(), 0, 0);

    }

    QList<QList<QList<float>>> barCharts;
};
