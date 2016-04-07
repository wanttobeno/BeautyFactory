#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QpaintEvent>
#include <QTimer>
#include <QPainter>
#include <QMessageBox>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/objdetect/objdetect.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    //鼠标事件
    void mouseMoveEvent(QMouseEvent *);


private slots:

    //打开图片click()槽声明
    void on_buttonOpenImage_clicked();

    //处理图片click()槽声明
    void on_buttonProcess_clicked();

    //一些关于选用滤镜的RadioButton的槽声明
    void on_radioButton_clicked();

    void on_radioButton_6_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_7_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_8_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_9_clicked();

    void on_radioButton_5_clicked();

    void on_radioButton_10_clicked();

    void on_radioButton_11_clicked();

    void on_radioButton_12_clicked();

    void on_radioButton_13_clicked();

    void on_radioButton_14_clicked();

    //透明度选择的水平滑块槽声明
    void on_horizontalSlider_valueChanged(int value);

    //保存图片的按钮槽声明
    void on_buttonSave_clicked();

    //返回原图的按钮槽声明
    void on_buttonInitImage_clicked();

    //转关标签页的槽声明
    void on_tabWidget_currentChanged(int index);

    //改变对比度滑块槽声明
    void on_sliderContrast_valueChanged(int value);

    //改变亮度滑块槽声明
    void on_sliderBrightness_valueChanged(int value);

    //改变饱和度滑块槽声明
    void on_sliderSaturation_valueChanged(int value);

    //是否启用滤镜的复选框clicked()槽声明
    void on_checkBox_2_clicked(bool checked);

    //涂鸦笔蓝色改变槽声明
    void on_sliderBlue_valueChanged(int value);

    //涂鸦笔绿色改变槽声明
    void on_sliderGreen_valueChanged(int value);

    //涂鸦笔红色改变槽声明
    void on_sliderRed_valueChanged(int value);

    //涂鸦笔粗细改变槽声明
    void on_sliderThickness_valueChanged(int value);

    //启用人脸识别复选框的槽声明
    void on_checkBox_stateChanged(int arg1);

    //拍照按钮clicked槽声明
    void on_buttonPhoto_clicked();

    //录像按钮clicked槽声明
    void on_buttonVideo_clicked();


private:
    //ui界面指针
    Ui::MainWindow *ui;

    //计时器
    QTimer theTimer;

    //滤镜相关参数
    int filterOpen;
    int processMode;
    double transparency;

    //调色板相关参数
    double brightness;
    double contrast;
    double saturation;

    //涂鸦笔相关参数
    int blueValue;
    int redValue;
    int greenValue;
    int thickness;

    //图片处理中图片的缓存
    cv::Mat image;
    cv::Mat originalImage;

    //摄像头当前帧
    cv::Mat frame;

    //帧捕获器
    cv::VideoCapture capture;

    //人脸检测器和参数
    cv::CascadeClassifier cascade, nestedCascade;
    int faceDetectMode;


protected:

    void paintEvent(QPaintEvent * e);

    void switchCamera(int index);

    void updateImage();

};


#endif // MAINWINDOW_H
