#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_program.h"
#include "opencv2/opencv.hpp"
#include <Qmessagebox>
#include <qfiledialog.h>
#include "opencv2/imgproc/types_c.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "qmouseeventtransition.h"
#include <QMouseEvent>
#pragma execution_character_set("utf-8")


class program : public QMainWindow
{
    Q_OBJECT

public:
    program(QWidget *parent = Q_NULLPTR);
    void mousePressEvent(QMouseEvent* ev);

signals:
    void Mouse_Pos();

private slots:
    void OpenImage(void);
    void ROI(void);
    void Histogram(void);
    void Equalization(cv::Mat Image);
    void binary_value(void);
    void HSV(void);
    void slider_value(void);
    void blur(void);
    void GaussianBlur(void);
    void medianBlur(void);
    void bilateralFilter(void);
    void Laplacian(void);
    void Sobel(void);
    void Canny(void);
    void rotation(void);
    void overturn(void);
    void translation(void);




private:
    Ui::programClass ui;
    QImage image;                 //載入圖片
    QString filename;             //檔案位置
    cv::Mat src;
    cv::Mat graysrc;
    cv::Mat dst;
    int width = 0;                  //圖的寬
    int height = 0;                  //圖的長

};
