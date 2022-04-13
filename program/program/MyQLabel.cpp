#include "MyQLabel.h"

MyQLabel::MyQLabel(QWidget *parent) : QLabel(parent)
{
}

MyQLabel::~MyQLabel()
{
}

void MyQLabel::mousePressEvent(QMouseEvent* ev)
{
    if (ev->buttons() & Qt::LeftButton)
    {
        
        //取得Qlabel的座標
        MyQLabel::x1 = QString::number(ev->x());  
        
        MyQLabel::y1 = QString::number(ev->y());

        emit Mouse_Pressed();  //發射到program 裡的Mouse_Pressed()
    }
    
    
}

