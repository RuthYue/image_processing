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
        
        //���oQlabel���y��
        MyQLabel::x1 = QString::number(ev->x());  
        
        MyQLabel::y1 = QString::number(ev->y());

        emit Mouse_Pressed();  //�o�g��program �̪�Mouse_Pressed()
    }
    
    
}

