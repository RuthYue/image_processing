#pragma once

#include <QObject>
#include <QLabel>
#include <QMouseEvent>

class MyQLabel : public QLabel
{
	Q_OBJECT

public:
	MyQLabel(QWidget *parent );
	~MyQLabel();
    void mousePressEvent(QMouseEvent* ev) override;
	
    QString x1, y1;     //存取得到的x,y

signals:
    void Mouse_Pressed();

};
