#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_program.h"
#include "opencv2/opencv.hpp"
#include <Qmessagebox>
#include <qfiledialog.h>
#include<qspinbox.h>
#include<qslider.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include "opencv2/imgproc/types_c.h"


class program : public QMainWindow
{
    Q_OBJECT

public:
    program(QWidget *parent = Q_NULLPTR);

private slots:
    void OpenImage(void);
    void ROI(void);
    void Histogram(void);
    void Equalization(cv::Mat Image);
    void binary_value(void);


private:
    Ui::programClass ui;
    QImage image;                 //載入圖片
    QString filename;             //檔案位置
    cv::Mat src;
    cv::Mat graysrc;
    cv::Mat dst;
};
