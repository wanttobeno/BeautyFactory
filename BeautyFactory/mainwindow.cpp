#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filter.h"
#include "facedetect.h"
#include <iostream>

//定义一些视频录制的参数
 int suffixImage = 1;
 int suffixVideo = 1;
 int captureMode = 0; // 0代表没有录制，1表示正在录制
 VideoWriter vw;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&theTimer, &QTimer::timeout, this, &MainWindow::updateImage);
    //初始化私有参数
    filterOpen = 1;
    processMode = 0;
    transparency = 0.0;
    brightness = 0.0;
    contrast = 100.0;
    saturation = 0.0;
    blueValue = 128;
    redValue = 255;
    greenValue = 255;
    thickness = 20;
    cascade.load(".\\resources\\cascade\\haarcascade_frontalface_alt.xml");
    nestedCascade.load(".\\resources\\cascade\\haarcascade_eye_tree_eyeglasses.xml");
    faceDetectMode = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}
//根据画笔实现涂鸦
void MainWindow::mouseMoveEvent(QMouseEvent *m)
{
    //获取鼠标坐标
    int x = m->pos().x();
    int y = m->pos().y();
    //是否在图片的范围之内
    if(!(x>40&&y>40&&x<40+1000&&y<40+700)) return;
    double ScaleX = (double)image.cols/920.0;
    double ScaleY = (double)image.rows/600.0;
    x-=43;
    y-=115;
    x*=ScaleX;
    y*=ScaleY;
    if(m->buttons() & Qt::LeftButton){
        circle(image, Point(x,y), 0, Scalar(redValue,greenValue,blueValue),thickness);
        QImage img = QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
        QImage scaledImg;
        scaledImg = img.scaled(ui->label->size(),Qt::KeepAspectRatio);
        ui->label->setPixmap(QPixmap::fromImage(scaledImg));
        //ui->label->resize(ui->label->pixmap()->size());
    }
}

//【1】image
//打开图片按钮按下后
void MainWindow::on_buttonOpenImage_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),".",tr("Image Files(*.png *.jpg *.jpeg *.bmp)"));
    if(fileName == NULL) return;
    ui->labelOpenImage->close();
    image = cv::imread(fileName.toLatin1().data());
    image.copyTo(originalImage);
    cv::cvtColor(image,image,CV_BGR2RGB);
    QImage img = QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
    QImage scaledImg;
    scaledImg = img.scaled(ui->label->size(),Qt::KeepAspectRatio);
    ui->label->setPixmap(QPixmap::fromImage(scaledImg));
    //ui->label->resize(ui->label->pixmap()->size());
}
//保存图片按钮按下后
void MainWindow::on_buttonSave_clicked()
{
   QString directoryName = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("*.bmp;; *.png;; *.jpg"));
   if(directoryName == NULL) return;
   cv::cvtColor(image,image,CV_RGB2BGR);
   // directoryName.append("\副本.jpg");
   imwrite(directoryName.toLatin1().data(),image);
}
//处理按钮按下后
void MainWindow::on_buttonProcess_clicked()
{
    if(!originalImage.data) return;
    originalImage.copyTo(image);
    //先将所有滤镜声明一个对象再初始化
    Heaven h(image);
    Classic c(image);
    Dark d(image);
    Blur g(image);
    DullPolish p(image);
    ColorMap m(image);
    HDR r(image);
    //每个RadioButton对应一种处理滤镜
    if(filterOpen){
    switch (processMode){
            case 1:image = h.process(); break;
            case 2:image = c.process(); break;
            case 3:image = d.process(); break;
            case 4:image = g.process(); break;
            case 5:image = p.process(); break;
            case 6:image = r.process(); break;
            case 7:image = m.process(0); break;
            case 8:image = m.process(1); break;
            case 9:image = m.process(3); break;
            case 10:image = m.process(5); break;
            case 11:image = m.process(6); break;
            case 12:image = m.process(7); break;
            case 13:image = m.process(10); break;
            case 14:image = m.process(11); break;
            default:{QMessageBox::information(NULL, "提示", "请选择一种滤镜！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);}
        }
    image = transparency * originalImage + (double)(1.0-transparency)*image;
}
    //对比度 亮度 饱和度
    GammaCorrector corrector(image);
    corrector.process(contrast,brightness).copyTo(image);
    SaturationChange schanger(image);
    schanger.process(saturation).copyTo(image);

    cv::cvtColor(image,image,CV_BGR2RGB);
    QImage img = QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
    QImage scaledImg;
    scaledImg = img.scaled(ui->label->size(),Qt::KeepAspectRatio);
    ui->label->setPixmap(QPixmap::fromImage(scaledImg));
    //ui->label->resize(ui->label->pixmap()->size());
}
//每个RadioButton对应一种处理模式
void MainWindow::on_radioButton_clicked()
{
    processMode = 1;
}
void MainWindow::on_radioButton_2_clicked()
{
    processMode = 2;
}
void MainWindow::on_radioButton_3_clicked()
{
    processMode = 3;
}
void MainWindow::on_radioButton_4_clicked()
{
    processMode = 4;
}
void MainWindow::on_radioButton_5_clicked()
{
    processMode = 5;
}
void MainWindow::on_radioButton_6_clicked()
{
    processMode = 6;
}
void MainWindow::on_radioButton_7_clicked()
{
    processMode = 7;
}
void MainWindow::on_radioButton_8_clicked()
{
    processMode = 8;
}
void MainWindow::on_radioButton_9_clicked()
{
    processMode = 9;
}
void MainWindow::on_radioButton_10_clicked()
{
    processMode = 10;
}
void MainWindow::on_radioButton_11_clicked()
{
    processMode = 11;
}
void MainWindow::on_radioButton_12_clicked()
{
    processMode = 12;
}
void MainWindow::on_radioButton_13_clicked()
{
    processMode = 13;
}
void MainWindow::on_radioButton_14_clicked()
{
    processMode = 14;
}
//通过一个水平滑块来控制滤镜的透明度
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    double range = 100.0;
    transparency = value/range;

    QString qs = "透明度：";
    qs.append(QString::number(value));
    qs.append('%');
    ui->trsprtLabel->setText(qs);
}
//返回原图槽函数
void MainWindow::on_buttonInitImage_clicked()
{
    ui->sliderContrast->setValue(100);
    ui->sliderBrightness->setValue(0);
    ui->sliderSaturation->setValue(0);
    originalImage.copyTo(image);
    cv::cvtColor(image,image,CV_BGR2RGB);
    QImage img = QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
    QImage scaledImg;
    scaledImg = img.scaled(ui->label->size(),Qt::KeepAspectRatio);
    ui->label->setPixmap(QPixmap::fromImage(scaledImg));
    //ui->label->resize(ui->label->pixmap()->size());
}


//【2】video
//当转移到标签页时，打开摄像头
void MainWindow::on_tabWidget_currentChanged(int index)
{
   return switchCamera(index);
}
//打开摄像头
void MainWindow::switchCamera(int index){
    //调整摄像头的大小
    //capture.set(CV_CAP_PROP_FRAME_WIDTH, 1024);
    //capture.set(CV_CAP_PROP_FRAME_HEIGHT, 768);
    if(index == 1)
    {
       capture.open(0);
       theTimer.start(10);
    }
    else capture.release();
   // else ;
}
//打印frame到屏幕上
void MainWindow::paintEvent(QPaintEvent *e)
{
    QImage image2 = QImage((uchar*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
    ui->labelCamera->setPixmap(QPixmap::fromImage(image2));
    //imageLabel->resize(image2.size());
    ui->labelCamera->show();
}
//槽函数，用来响应计时器发出的更新frame信号
void MainWindow::updateImage(){
    capture>>frame;
    if(frame.data)
    {
         //如果faceDetectMode，开启人脸检测
        if(faceDetectMode == 1){
            detectAndDraw(frame,cascade,nestedCascade,2,0);
        }
        //如果captureMode, 写入当前frame到视频
        if(captureMode == 1){
            vw<<frame;
        }
        //转换到RGB颜色空间并打印到屏幕上
        cvtColor(frame, frame, CV_BGR2RGB);//Qt中支持的是RGB图像, OpenCV中支持的是BGR
        this->update();  //发送刷新消息
    }
}

//调整对比度
void MainWindow::on_sliderContrast_valueChanged(int value)
{
    contrast = value;
}

//调整亮度
void MainWindow::on_sliderBrightness_valueChanged(int value)
{
    brightness = value;
}

//调整饱和度
void MainWindow::on_sliderSaturation_valueChanged(int value)
{
    saturation = value;
}

//是否开启滤镜
void MainWindow::on_checkBox_2_clicked(bool checked)
{
    filterOpen = !checked;
}

//调整涂鸦笔蓝色值
void MainWindow::on_sliderBlue_valueChanged(int value)
{
    blueValue = value;
}

//调整涂鸦笔绿色值
void MainWindow::on_sliderGreen_valueChanged(int value)
{
    greenValue = value;
}

//调整涂鸦笔红色值
void MainWindow::on_sliderRed_valueChanged(int value)
{
    redValue = value;
}

//调整涂鸦笔粗细
void MainWindow::on_sliderThickness_valueChanged(int value)
{
    thickness = value;
}

//是否开启人脸检测
void MainWindow::on_checkBox_stateChanged(int arg1)
{
    faceDetectMode = !faceDetectMode;
}

//拍照功能
void MainWindow::on_buttonPhoto_clicked()
{
    cvtColor(frame,frame,CV_BGR2RGB);
    //设置图片保存路径及名称
    QString pre = "./Storage/photo";
    QString suf = QString::number(suffixImage++);
    pre.append(suf);
    //图片格式为Jpg
    pre.append(".jpg");
    //保存图片
    imwrite(pre.toLatin1().data(),frame);
}

//录像功能
void MainWindow::on_buttonVideo_clicked()
{
    //开始录像:captureMode==0; 结束录像：captureMode==1
    if(captureMode == 0){
        ui->buttonVideo->setText("停止");
        QString pre = "./Storage/video";
        QString suf = QString::number(suffixVideo++);
        pre.append(suf);
        //用mp4容器格式封装
        pre.append(".avi");
        //打开videoWriter进行录制，压缩方式为MPEG-4
        vw.open(pre.toLatin1().data(),CV_FOURCC('D', 'I', 'V', 'X'),25,Size(frame.cols,frame.rows));
    }
    else{
        ui->buttonVideo->setText("录像");
        //关闭视频
        vw.release();
    }
    //改变状态标识
    captureMode = !captureMode;
}
