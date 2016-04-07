#pragma once
#ifndef FILTER_H
#define FILTER_H

#endif // FILTER_H
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>
#define MAXSIZE (32768)
float mSize = 0.5;
using namespace cv;

//通过伽马校正调整来对比度和亮度
class GammaCorrector{
private:
    Mat image;
    Mat dstImage;
    double contrastValue;
    double brightValue;
public:
    GammaCorrector(Mat img){
        img.copyTo(image);
        img.copyTo(dstImage);
    }
    Mat process(double contrast_value = 100, double bright_value = 0){
        contrastValue = contrast_value;
        brightValue = bright_value;
        //遍历
        for (int y = 0; y < image.rows; y++)
        {
            for (int x = 0; x < image.cols; x++)
            {
                for (int c = 0; c < 3; c++)
                {
                    //对比度和亮度
                    //对比度解决方案：将像素数值由0-255归一到0-2上，对像素数值进行幂运算。幂次取0-2范围内。，大于1增强对比，小于1减弱对比
                    //亮度直接进行加减操作 （注意上溢和下溢）
                    dstImage.at<Vec3b>(y, x)[c] =
                            saturate_cast<uchar>(pow(image.at<Vec3b>(y, x)[c]/128.0,(contrastValue/100))*128+brightValue);
                    //dstImage.at<Vec3b>(y, x)[c] = (sqrt(image.at<Vec3b>(y, x)[c] / 255.0) * 255);
                }
            }
        }
        return dstImage;
    }
};


//通过转换到HSV颜色空间，调整Saturation的值来调整饱和度
class SaturationChange{
private:
    Mat dstImage;
public:
   SaturationChange(Mat img){
        img.copyTo(dstImage);
    }
    Mat process(double change_value){
        //将图像转换到HSV颜色空间下，通过对Saturation通道数值的修改来改变饱和度
       cv::cvtColor(dstImage,dstImage,CV_BGR2HSV);
        for (int y = 0; y < dstImage.rows; y++)
        {
            for (int x = 0; x < dstImage.cols; x++)
            {
                    //加减操作改变饱和度
                dstImage.at<Vec3b>(y, x)[1] = saturate_cast<uchar>(dstImage.at<Vec3b>(y, x)[1]+change_value);
            }
        }
        //转换到原先的空间
        cv::cvtColor(dstImage,dstImage,CV_HSV2BGR);
        return dstImage;
    }
};


//羽化效果
class Heaven{
private:
    Mat src, dst;
    int width;
    int height;
    int centerX;
    int centerY;
    int maxV;
    int minV;
    int diff;
    float ratio;
public:
    Heaven(Mat SourceImage){
        SourceImage.copyTo(src);
        width = src.cols;
        height = src.rows;
        centerX = width >> 1;		//除2
        centerY = height >> 1;
        maxV = centerX*centerX + centerY*centerY;
        minV = (int)(maxV*(1 - mSize));
        diff = maxV - minV;
        ratio = width > height ? (float)height / (float)width : (float)width / (float)height;
    }
    Mat process(){
        Scalar avg = mean(src);
        Mat dst(src.size(), CV_8UC3);
        Mat mask1u[3];
        float tmp, r;
        for (int y = 0; y<height; y++)
        {
            uchar* srcP = src.ptr<uchar>(y);
            uchar* dstP = dst.ptr<uchar>(y);
            for (int x = 0; x<width; x++)
            {
                int b = srcP[3 * x];
                int g = srcP[3 * x + 1];
                int r = srcP[3 * x + 2];

                float dx = centerX - x;
                float dy = centerY - y;

                if (width > height)
                    dx = (dx*ratio);
                else
                    dy = (dy*ratio);

                int dstSq = dx*dx + dy*dy;

                float v = ((float)dstSq / diff) * 255;

                r = (int)(r + v);
                g = (int)(g + v);
                b = (int)(b + v);
                r = (r>255 ? 255 : (r<0 ? 0 : r));
                g = (g>255 ? 255 : (g<0 ? 0 : g));
                b = (b>255 ? 255 : (b<0 ? 0 : b));

                dstP[3 * x] = (uchar)b;
                dstP[3 * x + 1] = (uchar)g;
                dstP[3 * x + 2] = (uchar)r;
            }
        }
        return dst;
    }
};


//泛黄的怀旧效果
class Classic{
    Mat src;
    Mat dst;
    int width = src.cols;
    int height = src.rows;
    RNG rng;
public:
    Classic(Mat SourceImage){
        SourceImage.copyTo(src);
        dst = Mat(SourceImage.size(), CV_8UC3);
        width = SourceImage.cols;
        height = SourceImage.rows;
    }
    Mat process(){
        for (int y = 0; y < height; y++)
        {
            uchar* P0 = src.ptr<uchar>(y);
            uchar* P1 = dst.ptr<uchar>(y);
            for (int x = 0; x < width; x++)
            {
                float B = P0[3 * x];
                float G = P0[3 * x + 1];
                float R = P0[3 * x + 2];
                float newB = 0.272*R + 0.534*G + 0.131*B;
                float newG = 0.349*R + 0.686*G + 0.168*B;
                float newR = 0.393*R + 0.769*G + 0.189*B;
                if (newB < 0)newB = 0;
                if (newB > 255)newB = 255;
                if (newG < 0)newG = 0;
                if (newG > 255)newG = 255;
                if (newR < 0)newR = 0;
                if (newR > 255)newR = 255;
                P1[3 * x] = (uchar)newB;
                P1[3 * x + 1] = (uchar)newG;
                P1[3 * x + 2] = (uchar)newR;
            }
        }
        return dst;
    }
};


//黑白效果
class Dark{
    Mat src;
    Mat dst;
    int width;
    int height;
    RNG rng;
public:
    Dark(Mat SourceImage){
        SourceImage.copyTo(src);
        dst = Mat(SourceImage.size(), CV_8UC3);
        width = SourceImage.cols;
        height = SourceImage.rows;
    }
    Mat process(){
        for (int y = 0; y < height; y++)
        {
            uchar* P0 = src.ptr<uchar>(y);
            uchar* P1 = dst.ptr<uchar>(y);
            for (int x = 0; x < width; x++)
            {
                float B = P0[3 * x];
                float G = P0[3 * x + 1];
                float R = P0[3 * x + 2];
                float newB = abs(B - G + B + R)*G / 256;
                float newG = abs(B - G + B + R)*R / 256;
                float newR = abs(G - B + G + R)*R / 256;
                if (newB < 0)newB = 0;
                if (newB > 255)newB = 255;
                if (newG < 0)newG = 0;
                if (newG > 255)newG = 255;
                if (newR < 0)newR = 0;
                if (newR > 255)newR = 255;
                P1[3 * x] = (uchar)newB;
                P1[3 * x + 1] = (uchar)newG;
                P1[3 * x + 2] = (uchar)newR;
            }

        }
        Mat gray;
        cvtColor(dst, gray, CV_BGR2GRAY);
        normalize(gray, gray, 255, 0, CV_MINMAX);
        cvtColor(gray, gray, CV_GRAY2BGR);
        return gray;
    }
};


//高斯模糊效果
class Blur{
private:
    Mat dstImage;
public:
    Blur(Mat image){
       image.copyTo(dstImage);
    }
    Mat process(){
        GaussianBlur( dstImage, dstImage, Size(9, 9), 0, 0 );
        return dstImage;
    }
};


//熔铸效果-未加入
class Forge{
    Mat src;
    Mat dst;
    int width;
    int height;
    RNG rng;
public:
    Forge(Mat SourceImage){
        SourceImage.copyTo(src);
        dst = Mat(SourceImage.size(), CV_8UC3);
        width = SourceImage.cols;
        height = SourceImage.rows;
    }
    Mat process(){
        for (int y = 0; y<height; y++)
        {
            uchar* imgP = src.ptr<uchar>(y);
            uchar* dstP = dst.ptr<uchar>(y);
            for (int x = 0; x<width; x++)
            {
                float b0 = imgP[3 * x];
                float g0 = imgP[3 * x + 1];
                float r0 = imgP[3 * x + 2];

                float b = b0 * 255 / (g0 + r0 + 1);
                float g = g0 * 255 / (b0 + r0 + 1);
                float r = r0 * 255 / (g0 + b0 + 1);

                r = (r>255 ? 255 : (r<0 ? 0 : r));
                g = (g>255 ? 255 : (g<0 ? 0 : g));
                b = (b>255 ? 255 : (b<0 ? 0 : b));

                dstP[3 * x] = (uchar)b;
                dstP[3 * x + 1] = (uchar)g;
                dstP[3 * x + 2] = (uchar)r;
            }
        }
        return dst;
    }
};


//冰冻效果-未加入
class Frozen{
    Mat src;
    Mat dst;
    int width = src.cols;
    int height = src.rows;
    RNG rng;
public:
    Frozen(Mat SourceImage){
        SourceImage.copyTo(src);
        dst = Mat(SourceImage.size(), CV_8UC3);
        width = SourceImage.cols;
        height = SourceImage.rows;
    }
    Mat process(){
        for (int y = 0; y<height; y++)
        {
            uchar* imgP = src.ptr<uchar>(y);
            uchar* dstP = dst.ptr<uchar>(y);
            for (int x = 0; x<width; x++)
            {
                float b0 = imgP[3 * x];
                float g0 = imgP[3 * x + 1];
                float r0 = imgP[3 * x + 2];

                float b = (b0 - g0 - r0) * 3 / 2;
                float g = (g0 - b0 - r0) * 3 / 2;
                float r = (r0 - g0 - b0) * 3 / 2;

                r = (r>255 ? 255 : (r<0 ? -r : r));
                g = (g>255 ? 255 : (g<0 ? -g : g));
                b = (b>255 ? 255 : (b<0 ? -b : b));
                //          r = (r>255 ? 255 : (r<0? 0 : r));
                //          g = (g>255 ? 255 : (g<0? 0 : g));
                //          b = (b>255 ? 255 : (b<0? 0 : b));
                dstP[3 * x] = (uchar)b;
                dstP[3 * x + 1] = (uchar)g;
                dstP[3 * x + 2] = (uchar)r;
            }
        }
        return dst;
    }
};


//磨砂效果
class DullPolish{
    Mat src;
    Mat dst;
    int width;
    int height;
    RNG rng;
public:
    DullPolish(Mat SourceImage){
        SourceImage.copyTo(src);
        dst = Mat(SourceImage.size(), CV_8UC3);
        width = SourceImage.cols;
        height = SourceImage.rows;
    }
    Mat process(){
        for (int y = 1; y<height - 1; y++)
        {
            uchar* P0 = src.ptr<uchar>(y);
            uchar* P1 = dst.ptr<uchar>(y);
            for (int x = 1; x<width - 1; x++)
            {
                int tmp = rng.uniform(0, 9);
                P1[3 * x] = src.at<uchar>(y - 1 + tmp / 3, 3 * (x - 1 + tmp % 3));
                P1[3 * x + 1] = src.at<uchar>(y - 1 + tmp / 3, 3 * (x - 1 + tmp % 3) + 1);
                P1[3 * x + 2] = src.at<uchar>(y - 1 + tmp / 3, 3 * (x - 1 + tmp % 3) + 2);
            }

        }
        return dst;
    }
};


//彩色滤镜效果
class ColorMap{
    Mat src;
    int width;
    int height;
    Mat gray;
    Mat imgColor[12];
    Mat display;
public:
    ColorMap(Mat SourceImage){
        SourceImage.copyTo(src);
        width = SourceImage.cols;
        height = SourceImage.rows;
        display = Mat(height * 3, width * 4, CV_8UC3);
    }
    Mat process(int ColorMode){
        cvtColor(src, gray, CV_BGR2GRAY);
        //COLORMAP_AUTUMN = 1,
        //	COLORMAP_BONE = 2,
        //	COLORMAP_WINTER = 3,
        //	COLORMAP_OCEAN = 4,
        //	COLORMAP_SUMMER = 5,
        //	COLORMAP_SPRING = 6,
        //	COLORMAP_PINK = 7,
        //	COLORMAP_HOT = 8
            applyColorMap(gray, display, ColorMode);
        return display;
    }
};


//HDR效果
class HDR{
    int R;
    Mat src;
    int width = src.cols;
    int height = src.rows;
    Mat dst;
    Mat dst1u[3];
public:
    HDR(Mat SourceImage){
        R = 11;
        SourceImage.copyTo(src);
        dst = Mat(SourceImage.size(), CV_8UC3);
        width = SourceImage.cols;
        height = SourceImage.rows;
    }

    Mat process(){
        float tmp, r;
        for (int y = 0; y < height; y++)
        {
            uchar* imgP = src.ptr<uchar>(y);
            uchar* dstP = dst.ptr<uchar>(y);
            for (int x = 0; x<width; x++)
            {
                r = (float)imgP[3 * x];
                if (r>127.5)
                    tmp = r + (255 - r)*(r - 127.5) / 127.5;
                else
                    tmp = r*r / 127.5;
                tmp = tmp>255 ? 255 : tmp;
                tmp = tmp<0 ? 0 : tmp;
                dstP[3 * x] = (uchar)(tmp);

                r = (float)imgP[3 * x + 1];
                if (r>127.5)
                    tmp = r + (255 - r)*(r - 127.5) / 127.5;
                else
                    tmp = r*r / 127.5;
                tmp = tmp > 255 ? 255 : tmp;
                tmp = tmp<0 ? 0 : tmp;
                dstP[3 * x + 1] = (uchar)(tmp);

                r = (float)imgP[3 * x + 2];
                if (r>127.5)
                    tmp = r + (255 - r)*(r - 127.5) / 127.5;
                else
                    tmp = r*r / 127.5;
                tmp = tmp > 255 ? 255 : tmp;
                tmp = tmp < 0 ? 0 : tmp;
                dstP[3 * x + 2] = (uchar)(tmp);
            }
        }
        return  dst;
    }
};


