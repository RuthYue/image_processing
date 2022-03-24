#include "program.h"


program::program(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    
}
//開啟檔案
void program::OpenImage()  
{
    filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images(*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (QString::compare(filename, QString()) != 0)
    {
        bool voild = image.load(filename);
       
        if (voild)
        {
            int image_w = ui.lbl_image->width();         //取得圖片寬
            int image_h = ui.lbl_image->height();        //取得圖片長
            image = image.scaled(image_w, image_h, Qt::KeepAspectRatio, Qt::SmoothTransformation);      //圖片比例縮放
            int width = image.width();
            int height = image.height();
            ui.lbl_image->setPixmap(QPixmap::fromImage(image));

            ui.x_spinbox->setMinimum(0);
            ui.x_spinbox->setMaximum(200);
            ui.x_spinbox->setSingleStep(1);
            ui.x_slider->setMinimum(0);
            ui.x_slider->setMaximum(200);
            ui.x_slider->setSingleStep(1);

            ui.y_spinbox->setMinimum(0);
            ui.y_spinbox->setMaximum(200);
            ui.y_spinbox->setSingleStep(1);
            ui.y_slider->setMinimum(0);
            ui.y_slider->setMaximum(200);
            ui.y_slider->setSingleStep(1);

            ui.w_spinbox->setMinimum(0);
            ui.w_spinbox->setMaximum(image_w);
            ui.w_spinbox->setSingleStep(1);
            ui.w_slider->setMinimum(0);
            ui.w_slider->setMaximum(image_w);
            ui.w_slider->setSingleStep(1);

            ui.h_spinbox->setMinimum(0);
            ui.h_spinbox->setMaximum(image_h);
            ui.h_spinbox->setSingleStep(1);
            ui.h_slider->setMinimum(0);
            ui.h_slider->setMaximum(image_h);
            ui.h_slider->setSingleStep(1);

            ui.BV_slider->setMinimum(1);
            ui.BV_slider->setMaximum(255);
            ui.BV_slider->setSingleStep(1);
            ui.BV_spinbox->setMinimum(1);
            ui.BV_spinbox->setMaximum(255);
            ui.BV_spinbox->setSingleStep(1);



        }
        else
        {
            QMessageBox::warning(NULL, "載入失敗", "載入失敗");
        }
    }
}
//ROI
void program::ROI()
{
    
    int x = ui.x_spinbox->value();
    int y = ui.y_spinbox->value();
    int width = ui.w_spinbox->value();
    int height = ui.h_spinbox->value();
    cv::Mat image = cv::imread(filename.toLocal8Bit().toStdString());   //Qstring 轉 cv::String
    cv::Mat m_roi = image(cv::Rect(x, y, width, height));                //顯示ROI 的範圍
    cv::imshow("顯示ROI", m_roi);
}
//
void program::Histogram()
{   
    src = cv::imread(filename.toLocal8Bit().toStdString());
    if (src.empty())
        QMessageBox::warning(NULL, "Error", "Histogram Error");

    cvtColor(src, graysrc, cv::COLOR_BGR2GRAY);
    equalizeHist(graysrc, dst);
    imshow("灰階化", graysrc);
    imshow("Equalized Image", dst);
    Equalization(dst);
    cv::waitKey();
}

void program::Equalization(cv::Mat image)
{
    //定义变量
    cv::Mat dstHist;
    int dims = 1;
    float hranges[] = { 0, 256 };
    const float* ranges[] = { hranges };   // 这里需要为const类型
    int size = 256;
    int channels = 0;

    //计算图像的直方图
    calcHist(&image, 1, &channels, cv::Mat(), dstHist, dims, &size, ranges);

    cv::Mat dstImage(size, size, CV_8U, cv::Scalar(0));
    //获取最大值和最小值
    double minValue = 0;
    double maxValue = 0;
    minMaxLoc(dstHist, &minValue, &maxValue, 0, 0);  //  在cv中用的是cvGetMinMaxHistValue

    //绘制出直方图
    //saturate_cast函数的作用即是：当运算完之后，结果为负，则转为0，结果超出255，则为255。
    int hpt = cv::saturate_cast<int>(0.9 * size);
    for (int i = 0; i < 256; i++)
    {
        float binValue = dstHist.at<float>(i);           //   注意hist中是float类型   
        //拉伸到0-max
        int realValue = cv::saturate_cast<int>(binValue * hpt / maxValue);
        line(dstImage, cv::Point(i, size - 1), cv::Point(i, size - realValue), cv::Scalar(255));
    }
    cv::imshow("直方圖", dstImage);
    
}
void program::binary_value()
{
    double value = ui.BV_slider->value();
    
    QImage bv = image;
    bv = bv.convertToFormat(QImage::Format_Indexed8);  //灰階化

    bv.setColorCount(256);

    for (int i = 0; i < 256; i++)
    {
        int j;
        if (i < value)
        {
            j = 0;
        }
        else
        {
            j = 255;
        }
        bv.setColor(i, qRgb(j, j, j));
    }
    ui.lbl_image->setPixmap(QPixmap::fromImage(bv));
    
}
