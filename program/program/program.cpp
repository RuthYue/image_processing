#include "program.h"
#include<MyQLabel.h>

program::program(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


    connect(ui.lbl_image, SIGNAL(Mouse_Pressed()), this, SLOT(Mouse_Pressed()));

    //initialVtkWidget();
    
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
            //int image_w = ui.lbl_image->width();         //取得lbl_image寬
            //int image_h = ui.lbl_image->height();        //取得lbl_image長
            //image = image.scaled(image_w, image_h, Qt::KeepAspectRatio, Qt::SmoothTransformation);      //圖片比例縮放
            //width = image.width();                       //取得圖片寬
            //height = image.height();                     //取得圖片長
            //上面是Qimage
            //下面是Mat
            cv::Mat src = cv::imread(filename.toLocal8Bit().toStdString());
            cv::Mat rgb;
            if (src.channels() == 3)    //RGB圖片
            {
                cv::cvtColor(src, rgb, CV_BGR2RGB);
                image = QImage((const uchar*)(rgb.data),  
                    rgb.cols, rgb.rows,
                    rgb.cols * rgb.channels(),   
                    QImage::Format_RGB888);
            }
            else                     //灰階圖
            {
                image = QImage((const uchar*)(src.data),
                    src.cols, src.rows,
                    src.cols * src.channels(),    
                    QImage::Format_Indexed8);
            }

    
            ui.lbl_image->setPixmap(QPixmap::fromImage(image));   //顯示圖片
            ui.lbl_image->resize(ui.lbl_image->pixmap()->size());  //與圖片大小一樣
            ui.lbl_image->show();

            float image_w = ui.lbl_image->width();         //取得lbl_image寬
            float image_h = ui.lbl_image->height();        //取得lbl_image高

            width = image.width();                       //取得圖片寬
            height = image.height();  

            n1 =  width / image_w ;
            n2 =  height /image_w  ;

            ui.x_spinbox->setMinimum(0);             //設定spinbox,slider最大最小值 ,移動區間
            ui.x_spinbox->setMaximum(width);         //設最大數值等於widthc或height
            ui.x_spinbox->setSingleStep(1);          //移動區間為1
            ui.x_slider->setMinimum(0);
            ui.x_slider->setMaximum(width);
            ui.x_slider->setSingleStep(1);

            ui.y_spinbox->setMinimum(0);
            ui.y_spinbox->setMaximum(height);
            ui.y_spinbox->setSingleStep(1);
            ui.y_slider->setMinimum(0);
            ui.y_slider->setMaximum(height);
            ui.y_slider->setSingleStep(1);

            ui.w_spinbox->setMinimum(0);
            ui.w_spinbox->setMaximum(width);
            ui.w_spinbox->setSingleStep(1);
            ui.w_slider->setMinimum(0);
            ui.w_slider->setMaximum(width);
            ui.w_slider->setSingleStep(1);

            ui.h_spinbox->setMinimum(0);
            ui.h_spinbox->setMaximum(height);
            ui.h_spinbox->setSingleStep(1);
            ui.h_slider->setMinimum(0);
            ui.h_slider->setMaximum(height);
            ui.h_slider->setSingleStep(1);

            ui.BV_slider->setMinimum(1);
            ui.BV_slider->setMaximum(255);
            ui.BV_slider->setSingleStep(1);
            ui.BV_spinbox->setMinimum(1);
            ui.BV_spinbox->setMaximum(255);
            ui.BV_spinbox->setSingleStep(1);

            ui.image_wh->setText("height: "+ QString::number(height) + "width: " + QString::number(width));  //顯示圖片的高和寬

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
    if(filename != NULL)
    { 
        int x = ui.x_spinbox->value();       //取得x_spinbox值
        int y = ui.y_spinbox->value();       //取得y_spinbox值
        int width = ui.w_spinbox->value();   //取得w_spinbox值
        int height = ui.h_spinbox->value();  //取得h_spinbox值
        if(width ==0 || height==0)      //設width和height 不能為0  因為大小不能為0
            QMessageBox::warning(NULL, "載入失敗", "width,heigt 不能為0");
        else
        {
            cv::Mat image = cv::imread(filename.toLocal8Bit().toStdString());   //Qstring 轉 cv::String
            cv::Mat m_roi = image(cv::Rect(x, y, width, height));                //顯示ROI 的範圍
            cv::namedWindow("顯示ROI", 0);     
            cv::imshow("顯示ROI", m_roi);
        }
    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//直方圖等化
void program::Histogram()
{   
    if (filename != NULL)
    {
        src = cv::imread(filename.toLocal8Bit().toStdString());     //Qstring 轉 cv::String
        if (src.empty())
            QMessageBox::warning(NULL, "錯誤", "Histogram錯誤");

        cvtColor(src, graysrc, cv::COLOR_BGR2GRAY); //灰階化
        equalizeHist(graysrc, dst);                 //直方圖等化
        imshow("灰階化", graysrc);
        imshow("Equalized Image", dst);
        Equalization(dst);                          //顯示值方圖
        cv::waitKey();
    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//直方圖
void program::Equalization(cv::Mat image)
{
    //定義變量
    cv::Mat dstHist;
    int dims = 1;
    float hranges[] = { 0, 256 };
    const float* ranges[] = { hranges };   // 這裡需要為const類型
    int size = 256;
    int channels = 0;

    //計算圖的直方圖
    calcHist(&image, 1, &channels, cv::Mat(), dstHist, dims, &size, ranges);

    cv::Mat dstImage(size, size, CV_8U, cv::Scalar(0));
    //獲取最大值和最小值
    double minValue = 0;
    double maxValue = 0;
    minMaxLoc(dstHist, &minValue, &maxValue, 0, 0);  //  在cv中用的是cvGetMinMaxHistValue

    //繪製出直方圖
    //saturate_cast函數的作用即是：當運算完之後，結果為負，則轉為0，結果超出255，則為255。
    int hpt = cv::saturate_cast<int>(0.9 * size);
    for (int i = 0; i < 256; i++)
    {
        float binValue = dstHist.at<float>(i);           //   注意hist中是float類型 
        //拉長到0-max
        int realValue = cv::saturate_cast<int>(binValue * hpt / maxValue);
        line(dstImage, cv::Point(i, size - 1), cv::Point(i, size - realValue), cv::Scalar(255));
    }
    cv::imshow("直方圖", dstImage);
    
}
//影像二值化
void program::binary_value()
{
    if (filename != NULL)
    {
        double value = ui.BV_slider->value();       //取得值
        cv::Mat src1 = cv::imread(filename.toLocal8Bit().toStdString());
        cvtColor(src1, src1, cv::COLOR_BGR2GRAY); //灰階化
        cv::threshold(src1, src1, value, 255, CV_THRESH_BINARY);  //二質化
        cv::imshow("BV", src1);
        
        /*
        在Label 直接做二值化
        QImage bv = image;
        bv = bv.convertToFormat(QImage::Format_Indexed8);  //灰階化

        bv.setColorCount(256);                       //色深

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
       */  
        

    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//改變色彩空間
void program::HSV()
{
    if (filename != NULL)
    {
        cv::Mat HSVImage;                                        //HSV圖
        cv::Mat grayImage;                                       //灰階圖
        src = cv::imread(filename.toLocal8Bit().toStdString());   //Qstring 轉 cv::String
        cv::cvtColor(src, grayImage, CV_BGR2GRAY);               //灰階化
        cv::cvtColor(src, HSVImage, cv::COLOR_BGR2HSV);          //HSV
        cv::imshow("灰階", grayImage);
        cv::imshow("HSV", HSVImage);
    }
    else
    {
    QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//設定Slider數值
void program::slider_value()
{
    if(ui.x_slider)                                                //管理ROI截圖的範圍
        ui.w_slider->setMaximum(width - ui.x_slider->value());
    if(ui.w_slider)
        ui.x_slider->setMaximum(width - ui.w_slider->value());
    if (ui.y_slider)
        ui.h_slider->setMaximum(height - ui.y_slider->value());
    if (ui.h_slider)
        ui.y_slider->setMaximum(height - ui.h_slider->value());
    
}
//影像平滑模糊化
void program::blur()
{
    if (filename != NULL)
    {
        cv::Mat src = cv::imread(filename.toLocal8Bit().toStdString());
        cv::Mat image_blur;            //存blur後的值
        cv::blur(src, image_blur, cv::Size(5, 5), cv::Point(-1, -1), 4);
        cv::imshow("Blur", image_blur);
        cv::waitKey();
    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//高斯模糊
void program::GaussianBlur()
{

    if (filename != NULL)
    {
        cv::Mat src = cv::imread(filename.toLocal8Bit().toStdString());
        cv::Mat image_GaussianBlur;       //存高斯模糊的值
        cv::GaussianBlur(src, image_GaussianBlur, cv::Size(15, 15), 15, 0, 4);  
        cv::imshow("GaussianBlur", image_GaussianBlur);
        cv::waitKey();
    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//中值濾波
void program::medianBlur()
{
    if (filename != NULL)
    {
        cv::Mat src = cv::imread(filename.toLocal8Bit().toStdString());
        cv::Mat image_medianBlur;
        cv::medianBlur(src, image_medianBlur, 9);
        cv::imshow("medianBlur", image_medianBlur);
        cv::waitKey();
    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//雙邊濾波
void program::bilateralFilter()
{
    if (filename != NULL)
    {
        cv::Mat src = cv::imread(filename.toLocal8Bit().toStdString());
        cv::Mat image_bilateralFilter;
        cv::bilateralFilter(src, image_bilateralFilter, 10, 10, 10);
        cv::imshow("bilateralFilter", image_bilateralFilter);
        cv::waitKey();
    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//拉普拉斯矩陣
void program::Laplacian()
{
    if (filename != NULL)
    {
        cv::Mat src = cv::imread(filename.toLocal8Bit().toStdString());
        cv::Mat grayImage;
        cv::cvtColor(src, grayImage, CV_BGR2GRAY);
        cv::Mat image_Laplacian;
        cv::Laplacian(grayImage, image_Laplacian, CV_16S, 3, 1, 0);
        cv::convertScaleAbs(image_Laplacian, image_Laplacian); // 絕對值處理
        cv::threshold(image_Laplacian, image_Laplacian, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU); //影像增強
        cv::imshow("bilateralFilter", image_Laplacian);

        cv::waitKey();
    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//sobel
void program::Sobel()
{
    if (filename != NULL)
    {
        cv::Mat src = cv::imread(filename.toLocal8Bit().toStdString());
        cv::Mat src_x, src_y, src_xy;
        cv::Mat grayImage;
        //x方向
        cv::Sobel(src, src_x, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
        cv::imshow("x方向", src_x);
        //x方向
        cv::Sobel(src, src_y, CV_8U, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT);
        cv::imshow("y方向", src_y);
        //合併x,y
        //合并的
        addWeighted(src_x, 0.5, src_y, 0.5, 0, src_xy);
        imshow("x+y", src_xy);
        cv::waitKey();
    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//canny
void program::Canny()
{
    if (filename != NULL)
    {
        cv::Mat src = cv::imread(filename.toLocal8Bit().toStdString());
        cv::Mat src_canny;
        cv::Canny(src, src_canny, 150, 100);
        cv::imshow("Canny", src_canny);
        cv::waitKey();
    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//旋轉
void program::rotation()
{
    if (filename != NULL)
    {
        cv::Mat src = cv::imread(filename.toLocal8Bit().toStdString());
        cv::Mat src_rotation , src_rotation2;

        cv::Point2f srcPoints[3];//原圖中的三點 ,一個包含三維點（x，y）的數組，其中x、y是浮點型數
        cv::Point2f dstPoints[3];//目標圖中的三點  

        srcPoints[0] = cv::Point2f(0, 0);
        srcPoints[1] = cv::Point2f(0, src.rows);
        srcPoints[2] = cv::Point2f(src.cols, 0);
        //映射後的三個座標值
        dstPoints[0] = cv::Point2f(0, src.rows * 0.3);
        dstPoints[1] = cv::Point2f(src.cols * 0.25, src.rows * 0.75);
        dstPoints[2] = cv::Point2f(src.cols * 0.75, src.rows * 0.25);

        cv::Mat M1 = getAffineTransform(srcPoints, dstPoints);

        cv::warpAffine(src, src_rotation, M1, src.size());



        cv::Point2f center(src.cols / 2, src.rows / 2);//旋轉中心  
        double angle = 45;//逆時針旋轉45度  
        double scale = 0.5;//縮放比例  

        cv::Mat M2 = getRotationMatrix2D(center, angle, scale);//計算旋轉加縮放的變換矩陣  
        warpAffine(src, src_rotation2, M2, cv::Size(src.cols, src.rows), cv::INTER_LINEAR);//仿射變換



        cv::imshow("旋轉", src_rotation);
        cv::imshow("旋轉2", src_rotation2);

        cv::waitKey();
    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//翻轉
void program::overturn()
{
    if (filename != NULL)
    {
        cv::Mat src = cv::imread(filename.toLocal8Bit().toStdString());
        cv::Mat src_overturn;

        cv::Point2f srcPoints[3];//原圖中的三點 ,一個包含三維點（x，y）的數組，其中x、y是浮點型數
        cv::Point2f dstPoints[3];//目標圖中的三點  

        srcPoints[0] = cv::Point2i(0, 0);
        srcPoints[1] = cv::Point2i(0, src.rows);
        srcPoints[2] = cv::Point2i(src.cols, 0);

        dstPoints[0] = cv::Point2i(src.cols, 0);
        dstPoints[1] = cv::Point2i(src.cols, src.rows);
        dstPoints[2] = cv::Point2i(0, 0);

        cv::Mat M1 = getAffineTransform(srcPoints, dstPoints);

        cv::warpAffine(src, src_overturn, M1, cv::Size(src.cols, src.rows));

        cv::imshow("翻轉", src_overturn);


        cv::waitKey();
    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//平移
void program::translation()
{
    if (filename != NULL)
    {
        cv::Mat src = cv::imread(filename.toLocal8Bit().toStdString());
        cv::Mat src_translation;

        cv::Point2f srcPoints[3];//原圖中的三點 ,一個包含三維點（x，y）的數組，其中x、y是浮點型數
        cv::Point2f dstPoints[3];//目標圖中的三點  

        srcPoints[0] = cv::Point2i(0, 0);
        srcPoints[1] = cv::Point2i(0, src.rows);
        srcPoints[2] = cv::Point2i(src.cols, 0);

        dstPoints[0] = cv::Point2i(src.cols / 3, 0);
        dstPoints[1] = cv::Point2i(src.cols / 3, src.rows);
        dstPoints[2] = cv::Point2i(src.cols + src.cols / 3, 0);

        cv::Mat M1 = getAffineTransform(srcPoints, dstPoints);

        cv::warpAffine(src, src_translation, M1, cv::Size(src.cols + src.cols / 3, src.rows));

        cv::imshow("平移", src_translation);


        cv::waitKey();
    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//自訂參數仿射轉換
void program::us_translation()
{
    if (filename != NULL)
    {
        cv::Mat src = cv::imread(filename.toLocal8Bit().toStdString());
        cv::Mat src_translation;

        cv::Point2f srcPoints[3];//原圖中的三點 ,一個包含三維點（x，y）的數組，其中x、y是浮點型數
        cv::Point2f dstPoints[3];//目標圖中的三點  

        srcPoints[0] = cv::Point2i(0, 0);
        srcPoints[1] = cv::Point2i(0, src.rows);
        srcPoints[2] = cv::Point2i(src.cols, 0);



        dstPoints[0] = cv::Point2i(ui.affine_lE_x->text().toInt(), ui.affine_lE_y->text().toInt());
        dstPoints[1] = cv::Point2i(ui.affine_lE_x2->text().toInt(), ui.affine_lE_y2->text().toInt());
        dstPoints[2] = cv::Point2i(ui.affine_lE_x3->text().toInt(), ui.affine_lE_y3->text().toInt());

        cv::Mat M1 = getAffineTransform(srcPoints, dstPoints);

        cv::warpAffine(src, src_translation, M1, cv::Size(src.cols, src.rows));

        cv::imshow("自訂仿射轉換", src_translation);


        cv::waitKey();
    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//透視投影
void program::perspective()
{
    if (filename != NULL)
    {
        cv::Mat src = cv::imread(filename.toLocal8Bit().toStdString());
        cv::Mat src_perspective;

        cv::Point2f src_coners[4];
        cv::Point2f dst_coners[4];

        /*src_coners[0] = cv::Point2f(13, 134);
        src_coners[1] = cv::Point2f(271, 24);
        src_coners[2] = cv::Point2f(180, 411);
        src_coners[3] = cv::Point2f(398, 362);*/
        for (int i = 0; i < 4; i++)
        {
            src_coners[i] = cv::Point2f(x[i], y[i]);
            
        }
        /*src_coners[0] = cv::Point2f(x[0] , y[0] );
        src_coners[1] = cv::Point2f(x[1] , y[1] );
        src_coners[2] = cv::Point2f(x[2] , y[2] );
        src_coners[3] = cv::Point2f(x[3] , y[3] );*/

       /* dst_coners[0] = cv::Point2f(0, src.cols);
        dst_coners[1] = cv::Point2f(0, 0);
        dst_coners[2] = cv::Point2f(src.rows, src.cols);
        dst_coners[3] = cv::Point2f(src.rows, 0);*/

        /*dst_coners[0] = cv::Point2f(0, ui.lbl_image->height());
        dst_coners[1] = cv::Point2f(0, 0);
        dst_coners[2] = cv::Point2f(ui.lbl_image->width(), ui.lbl_image->height());
        dst_coners[3] = cv::Point2f(ui.lbl_image->width(), 0);*/

        dst_coners[0] = cv::Point2f(0, 0);         
        dst_coners[1] = cv::Point2f(0, ui.lbl_image->height() - 1);
        dst_coners[2] = cv::Point2f(ui.lbl_image->width() - 1, ui.lbl_image->height() - 1);
        dst_coners[3] = cv::Point2f(ui.lbl_image->width() - 1, 0);
        
        cv::Mat M1 = cv::getPerspectiveTransform(src_coners, dst_coners);

        cv::warpPerspective(src, src_perspective, M1, src.size());

        cv::imshow("透視投影", src_perspective);

        cv::waitKey();
    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}
//滑鼠點擊事件
void program::Mouse_Pressed()
{
    if (count <= 3)
    {
        //Qlabel 回傳x1 y1 
        ui.lbl_xy->setText(QString("X = %1, Y = %2").arg(ui.lbl_image->x1).arg(ui.lbl_image->y1));
        //儲存點擊的x,y
        int c1 = count; //紀錄點擊次數
        x[c1] = ui.lbl_image->x1.toFloat();                     
        y[c1] = ui.lbl_image->y1.toFloat();
        count++;
    }
    else
        count = 0;

}

void program::face()
{
    cv::Mat img = cv::imread(filename.toLocal8Bit().toStdString());
    std::string xmlPath = "C:\\Users\\RuthlessYue\\Downloads\\python-opencv-detect-master\\python-opencv-detect-master\\haarcascade_frontalface_alt.xml";

    

    cv::CascadeClassifier detector;

    detector.load(xmlPath);



    std::vector<Rect> faces;

    detector.detectMultiScale(img, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

    for (size_t t = 0; t < faces.size(); t++) 
        cv::rectangle(img, faces[t], cv::Scalar(0, 0, 255), 1, 8, 0);

    cv::imshow("Result", img);


}

void program::simple_contour()
{
    if (filename != NULL)
    {
        
        cv::Mat src1 = cv::imread(filename.toLocal8Bit().toStdString());
        cv::Mat src1_gray , src1_binary;

        vector<vector<Point>> contours;

        cvtColor(src1, src1_gray, cv::COLOR_BGR2GRAY); //灰階化
        cv::imshow("Contours ", src1_gray);
        cv::threshold(src1_gray, src1_binary, 127, 255, CV_THRESH_BINARY);  //二質化
        cv::imshow("threshold", src1_binary);
        cv::findContours(src1_binary, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
        
        cv::drawContours(src1_binary, contours, -1, Scalar(0,255.0),3);


        cv::imshow("Contours", src1_binary);

    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}

cv::Mat src_1;
cv::Mat src1_gray, src1_binary;
vector<vector<Point>> contours;
int thresh = 100;
RNG rng(12345);
void contour_threshold_callback(int, void*);
void convex_hull_callback(int, void*);
void Bounding_boxes_callback(int, void*);

void program::find_comtour()
{
    if (filename != NULL)
    {
        const char* source_window = "Source";
        src_1 = cv::imread(filename.toLocal8Bit().toStdString());   //讀取開啟的檔案
        cvtColor(src_1, src1_gray, cv::COLOR_BGR2GRAY); //灰階化
        cv::blur(src1_gray, src1_gray, cv::Size(3, 3));
        cv::threshold(src1_gray, src1_binary, 127, 255, CV_THRESH_BINARY);  //二質化
        cv::imshow("Contours ", src1_gray);
   

        //create a window and a trackbar
        namedWindow(source_window);
        cv::imshow("Source", src_1);
        createTrackbar("Canny thresh:", source_window, &thresh, 255, contour_threshold_callback);
        contour_threshold_callback(thresh,0);

        cv::imshow("threshold", src1_binary);


        cv::imshow("Contours", src1_binary);
        waitKey();

    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}

void contour_threshold_callback(int, void*)
{
    Mat canny_output;
    Canny(src1_gray, canny_output, thresh, thresh * 2);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
    }
    imshow("Contours", drawing);
}

void program::Convex_Hull()
{
    if (filename != NULL)
    {
        const char* source_window = "Source";
        src_1 = cv::imread(filename.toLocal8Bit().toStdString());   //讀取開啟的檔案
        cvtColor(src_1, src1_gray, cv::COLOR_BGR2GRAY); //灰階化
        cv::blur(src1_gray, src1_gray, cv::Size(3, 3));
        cv::threshold(src1_gray, src1_binary, 127, 255, CV_THRESH_BINARY);  //二質化
        cv::imshow("Contours ", src1_gray);


        //create a window and a trackbar
        namedWindow(source_window);
        cv::imshow("Source", src_1);
        createTrackbar("Canny thresh:", source_window, &thresh, 255, convex_hull_callback);
        contour_threshold_callback(thresh, 0);

        cv::imshow("threshold", src1_binary);


        cv::imshow("Contours", src1_binary);
        waitKey();

    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}

void convex_hull_callback(int, void*)
{
    Mat canny_output;
    Canny(src1_gray, canny_output, thresh, thresh * 2);
    vector<vector<Point> > contours;
    findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point> >hull(contours.size());
    for (size_t i = 0; i < contours.size(); i++)
    {
        convexHull(contours[i], hull[i]);
    }
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        drawContours(drawing, contours, (int)i, color);
        drawContours(drawing, hull, (int)i, color);
    }

    imshow("Contours", drawing);
}

void program::Bounding_boxes()
{
    if (filename != NULL)
    {
        const char* source_window = "Source";
        src_1 = cv::imread(filename.toLocal8Bit().toStdString());   //讀取開啟的檔案
        cvtColor(src_1, src1_gray, cv::COLOR_BGR2GRAY); //灰階化
        cv::blur(src1_gray, src1_gray, cv::Size(3, 3));
        cv::threshold(src1_gray, src1_binary, 127, 255, CV_THRESH_BINARY);  //二質化
        cv::imshow("Contours ", src1_gray);


        //create a window and a trackbar
        namedWindow(source_window);
        cv::imshow("Source", src_1);
        createTrackbar("Canny thresh:", source_window, &thresh, 255, Bounding_boxes_callback);
        contour_threshold_callback(thresh, 0);

        cv::imshow("threshold", src1_binary);


        cv::imshow("Contours", src1_binary);
        waitKey();

    }
    else
    {
        QMessageBox::warning(NULL, "載入失敗", "請載入圖片");
    }
}

void Bounding_boxes_callback(int, void*)
{
    Mat canny_output;
    Canny(src1_gray, canny_output, thresh, thresh * 2);
    vector<vector<Point> > contours;
    findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

    vector<vector<Point> > contours_poly(contours.size());
    vector<Rect> boundRect(contours.size());
    vector<Point2f>centers(contours.size());
    vector<float>radius(contours.size());
    for (size_t i = 0; i < contours.size(); i++)
    {
        approxPolyDP(contours[i], contours_poly[i], 3, true);
        boundRect[i] = boundingRect(contours_poly[i]);
        minEnclosingCircle(contours_poly[i], centers[i], radius[i]);
    }
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        drawContours(drawing, contours_poly, (int)i, color);
        rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2);
        circle(drawing, centers[i], (int)radius[i], color, 2);
    }
    imshow("Contours", drawing);
}

void program::basic_operations()
{
    cv::Mat src = cv::imread(filename.toLocal8Bit().toStdString());
    cv::Mat src_gray, erosion_dst, dilation_dst, opening_dst;
    cvtColor(src, src_gray, cv::COLOR_BGR2GRAY); //灰階化
    cv::imshow("Source image", src);
    cv::threshold(src_gray, src_gray, 127, 255, CV_THRESH_BINARY);  //二質化
    cv::imshow("threshold", src_gray);


    int erosion_size = 1;
    Mat element = getStructuringElement(MORPH_CROSS, Size(2 * erosion_size + 1, 2 * erosion_size + 1), Point(erosion_size, erosion_size));
    erode(src_gray, erosion_dst, element);
    imshow("Erosion ", erosion_dst);

    int dilation_size = 1;
    Mat dilation = getStructuringElement(MORPH_RECT, Size(2 * dilation_size + 1, 2 * dilation_size + 1), Point(dilation_size, dilation_size));
    dilate(erosion_dst, dilation_dst, dilation);
    imshow("Dilation ", dilation_dst);

    int opening_size = 1;
    Mat opening = getStructuringElement(MORPH_RECT, Size(2 * dilation_size + 1, 2 * dilation_size + 1), Point(dilation_size, dilation_size));
    morphologyEx(src_gray, opening_dst, MORPH_OPEN, opening);
    imshow("Opening ", opening_dst);
}

void program::advanced_mprphology()
{

}