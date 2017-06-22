//
//  main.cpp
//  Praktikum_6_MBV
//
//  Created by Nils on 19.06.17.
//  Copyright © 2017 Nils. All rights reserved.
//


#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui.hpp>

void onMouse( int event, int x, int y, int, void* );

std::vector<cv::Point2d> featureVector;
int col = 0;
int main(int argc, const char * argv[]) {
    
    
    
    cv::Mat image;
    cv::Mat featureVectorMat;
    image = cv::imread("/Users/Nils/Desktop/MBV-2017-A02c_Daten/gummy_color.jpg", CV_LOAD_IMAGE_COLOR);
    
    if(! image.data )
    {
        std::cout <<  "Could not open or find the image" << std::endl;
        return -1;
    }
    
    cv::namedWindow( "Hauptachsentransformation", cv::WINDOW_AUTOSIZE );
    cv::imshow( "Hauptachsentransformation", image );
    cv::setMouseCallback( "Hauptachsentransformation", onMouse, 0 );
    cv::waitKey(0);
    
    featureVectorMat = cv::Mat((int)featureVector.size(), 2, CV_64FC1);
    for(int k = 0; k < featureVector.size(); k++){
        featureVectorMat.at<double>(k, 0) = featureVector.at(k).x;
        featureVectorMat.at<double>(k, 1) = featureVector.at(k).y;
    }
    
    
    cv::PCA transformer(featureVectorMat, cv::Mat(), CV_PCA_DATA_AS_ROW);
    
    cv::Point mean = cv::Point(transformer.mean.at<double>(0, 0), transformer.mean.at<double>(0, 1));
    
    std::vector<cv::Point2d> eigen_vecs(2);
    std::vector<double> eigen_val(2);
    for (int i = 0; i < 2; ++i)
    {
        eigen_vecs[i] = cv::Point2d(transformer.eigenvectors.at<double>(i, 0) ,  transformer.eigenvectors.at<double>(i, 1));
        
        eigen_val[i] = transformer.eigenvalues.at<double>(0, i);
    }
    
    cv::line(image, mean, mean + cv::Point(eigen_vecs[0].x * eigen_val[0], eigen_vecs[0].y * eigen_val[0]) , CV_RGB(255, 255, 0));
    cv::line(image, mean, mean + cv::Point(eigen_vecs[1].x * eigen_val[1], eigen_vecs[1].y * eigen_val[1]) , CV_RGB(0, 255, 255));
    cv::imshow( "Hauptachsentransformation", image );
    cv::waitKey(0);
    return 0;
}


void onMouse( int event, int x, int y, int, void* )
{
    if( event != CV_EVENT_LBUTTONDOWN )
        return;
    
    featureVector.push_back(cv::Point2d(x, y));
    std::cout<<"x="<<featureVector.at(col).x <<"\t y="<< featureVector.at(col).y<<"\n";
    col = col + 1;
}
