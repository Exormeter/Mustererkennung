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


int main(int argc, const char * argv[]) {
    
    cv::Mat pt_mat(5, 2, CV_32FC1);
    pt_mat.at<float>(0,0) = 1;
    pt_mat.at<float>(0,1) = 1.1;
    pt_mat.at<float>(1,0) = 2;
    pt_mat.at<float>(1,1) = 1.6;
    pt_mat.at<float>(2,0) = 2.3;
    pt_mat.at<float>(2,1) = 2.7;
    pt_mat.at<float>(3,0) = 1.9;
    pt_mat.at<float>(3,1) = 2.2;
    pt_mat.at<float>(4,0) = 3.1;
    pt_mat.at<float>(4,1) = 3.0;
    
    cv::PCA pt_pca(pt_mat, cv::Mat(), CV_PCA_DATA_AS_ROW, 0);
    
    // Mean
    // Rows: 1 Cols: 6
    cv::Mat pt_mean = pt_pca.mean;
    
    // Eigen values
    // In highest to lowest order
    // Rows: 6 Cols: 1
    cv::Mat pt_eig_vals = pt_pca.eigenvalues;
    
    //for (int i = 0; i < 2; ++i)
    std::cout << "Eingenwert: " << pt_eig_vals.at<float>(0, 0) << std::endl;
    std::cout << "Eigenwert: " << pt_eig_vals.at<float>(0, 1) << std::endl;
    
    // Eigen vectors
    cv::Mat pt_eig_vecs = pt_pca.eigenvectors;
    
    std::cout << "Eigenvector: " << pt_eig_vecs.at<float>(0,0) << " " << pt_eig_vecs.at<float>(0,1) << " " << std::endl;
    
    std::cout << "Eigenvector: " << pt_eig_vecs.at<float>(1,0) << " " << pt_eig_vecs.at<float>(1,1) << " " << std::endl;
    
    cv::Mat pt_data_projected = pt_pca.project(pt_mat);
    
    for(int i = 0; i < 5; i++){
        std::cout << "Punkt nach Transformation ";
        for(int j = 0; j < 2; j++){
            std::cout << pt_data_projected.at<float>(j, i) << " ";
        }
        std::cout << std::endl;
    }
}
