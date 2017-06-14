//
//  main.cpp
//  Praktikum_6_MBV
//
//  Created by Nils on 12.06.17.
//  Copyright © 2017 Nils. All rights reserved.
//


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string>


int main(int argc, const char * argv[]) {
    cv::Mat structurElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    
    cv::Mat orgImage;
    cv::Mat srcImage;
    std::string filename("/Users/Nils/Desktop/MBV-2017-A02c_Daten/gummy_color.jpg");
    
    orgImage = cv::imread(filename, CV_LOAD_IMAGE_UNCHANGED);
    imshow( "Input", orgImage );
    cvWaitKey(0);
    
    srcImage = cv::imread(filename, CV_LOAD_IMAGE_UNCHANGED);
    
    for(int x = 0; x < srcImage.rows; x++){
        for(int y = 0; y < srcImage.cols; y++){
            cv::Vec3b red{0,0,255};
            cv::Vec3b intensity = srcImage.at<cv::Vec3b>(x, y);
            if(intensity.val[2] > 130 && intensity.val[1] > 130 && intensity.val[0] < 35){
                srcImage.at<cv::Vec3b>(x, y) = red;
            }
        }
    }
    imshow( "Input", srcImage );
    cvWaitKey(0);
    
    
    
    cv::cvtColor(srcImage, srcImage, CV_RGB2HSV);
    for(int x = 0; x < srcImage.rows; x++){
        for(int y = 0; y < srcImage.cols; y++){
            if(srcImage.at<cv::Vec3b>(x, y)[1] < 75){
                srcImage.at<cv::Vec3b>(x, y)[1] = 0;
            }
            else{
                srcImage.at<cv::Vec3b>(x, y)[1] = 255;

            }
            srcImage.at<cv::Vec3b>(x, y)[2] = 255;
        }
    }
    
    cv::cvtColor(srcImage, srcImage, CV_HSV2BGR);
    
    cv::cvtColor(srcImage, srcImage, cv::COLOR_BGR2GRAY);
    imshow ("GRAY", srcImage);
    cvWaitKey(0);
    
    cv::equalizeHist(srcImage, srcImage);
    
    //cv::GaussianBlur(srcImage, srcImage, cv::Size(3,3), 1.5);
    
    cv::threshold(srcImage, srcImage, 0, 255, cv::THRESH_BINARY_INV+cv::THRESH_OTSU);
    imshow ("THRESHOLD", srcImage);
    cvWaitKey(0);
    
    cv::morphologyEx(srcImage, srcImage, cv::MORPH_CLOSE, structurElement);
    imshow ("CLOSED", srcImage);
    cvWaitKey(0);
    
    cv::morphologyEx(srcImage, srcImage, cv::MORPH_ERODE, structurElement);
    
    cv::distanceTransform(srcImage, srcImage, CV_DIST_L2, 3);
    normalize(srcImage, srcImage, 0, 1., cv::NORM_MINMAX);
    imshow ("DISTANCE", srcImage);
    
    cv::threshold(srcImage, srcImage, .31, 1., CV_THRESH_BINARY);
    cv::imshow("DISTENCE/THRESHOLD", srcImage);
    cvWaitKey(0);

    
    cv::Mat dist_8u;
    srcImage.convertTo(dist_8u, CV_8U);
    
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    
    cv::Mat markers = cv::Mat::zeros(srcImage.size(), CV_32SC1);
    
    for (size_t i = 0; i < contours.size(); i++){
        cv::drawContours(markers, contours, static_cast<int>(i), cv::Scalar::all(static_cast<int>(i)+1), -1);
    }
    
    
    cv::circle(markers, cv::Point(5,5), 3, CV_RGB(255,255,255), -1);
    imshow("Markers", markers*10000);
    cvWaitKey(0);
    
    cv::watershed(orgImage, markers);
    
    cv::Mat mark = cv::Mat::zeros(markers.size(), CV_8UC1);
    markers.convertTo(mark, CV_8UC1);
    bitwise_not(mark, mark);
    //    imshow("Markers_v2", mark); // uncomment this if you want to see how the mark
    // image looks like at that point
    // Generate random colors
    std::vector<cv::Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int b = cv::theRNG().uniform(0, 255);
        int g = cv::theRNG().uniform(0, 255);
        int r = cv::theRNG().uniform(0, 255);
        colors.push_back(cv::Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    // Create the result image
    cv::Mat dst = cv::Mat::zeros(markers.size(), CV_8UC3);
    // Fill labeled objects with random colors
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i,j);
            if (index > 0 && index <= static_cast<int>(contours.size()))
                dst.at<cv::Vec3b>(i,j) = colors[index-1];
            else
                dst.at<cv::Vec3b>(i,j) = cv::Vec3b(0,0,0);
        }
    }
    // Visualize the final image
    imshow("Final Result", dst);
    cvWaitKey(0);

    
    
    
    return 0;
}
