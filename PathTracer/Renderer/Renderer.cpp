//
//  Renderer.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/26.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Renderer.h"
#include "Shape.h"
#include "PathIntegrator.h"

#include <iostream>
#include <opencv2/opencv.hpp>

Renderer::Renderer(int* reso, int spc, int mD, Scene* sc, Sampler* psp,
                   Sampler* nsp, double* lookAt, double fov) {
    xres = reso[1];
    yres = reso[0];
    sampleCount = spc;
    maxDepth = mD;
    pixels = new double*[yres]();
    for (int i = 0; i < yres; i++) {
        pixels[i] = new double[xres * 3](); //3 channels
    }
    scene = sc;
    pixelSampler = psp;
    normalSampler = nsp;
    rays = new Ray[reso[1] * reso[0] * sampleCount]();
    camera = new PerspectiveCamera(lookAt, reso, fov, sampleCount, pixelSampler, rays);
}


Renderer::~Renderer() {
    for (int i = 0; i < yres; i++) {
        delete[] pixels[i];
    }
    delete[] pixels;
    delete[] rays;
}

void Renderer::printPixels() const {
    for (int rowi = 0; rowi < yres; rowi++) {
        for (int coli = 0; coli < xres; coli++) {
            if (coli != xres - 1) {
                printf("(%f, %f, %f), ", pixels[rowi][coli * 3],
                       pixels[rowi][coli * 3 + 1], pixels[rowi][coli * 3 + 2]);
            }
            else{
                printf("(%f, %f, %f)\n", pixels[rowi][coli * 3],
                       pixels[rowi][coli * 3 + 1], pixels[rowi][coli * 3 + 2]);
            }
        }
    }
}

void Renderer::showImage() const {
    cv::Mat img = cv::Mat::zeros(yres, xres, CV_64FC3);
    for (int i = 0; i < yres; i++) {
        for (int j = 0; j < xres; j++) {
            img.at<cv::Vec3d>(i, j)[0] = pixels[i][j * 3];
            img.at<cv::Vec3d>(i, j)[1] = pixels[i][j * 3 + 1];
            img.at<cv::Vec3d>(i, j)[2] = pixels[i][j * 3 + 2];
        }
    }
    cv::imwrite("resul/img.png", img * 255.0);
    cv::imshow("window", img);
    cv::waitKey(0);
}




void Renderer::startRendering() {
    std::cout<<"–––––––––––––––––––Start Rendering––––––––––––––––––"<<std::endl;
    double* pixelBuffer = new double[sampleCount * 3]();
    for (int rowi = 0; rowi < yres; rowi++) {
        for (int coli = 0; coli < xres; coli++) {
//    for (int rowi = 250; rowi < 251; rowi++) {
//        for (int coli = 200; coli < 201; coli++) {
            #pragma omp parallel for schedule(dynamic)
            for (int spi = 0; spi < sampleCount; spi++) {
                int offset = (rowi * xres + coli) * sampleCount + spi;
                if (!rays[offset].isInit()) {
                    std::cout<<"This ray has not been initialized !"<<std::endl;
                    continue;
                }
//                std::cout<<rays[offset].getDirection().transpose()<<std::endl;
                PathIntegrator path = PathIntegrator(&rays[offset], scene, normalSampler, maxDepth);
                Spectrum3d rad = path.Li();
                pixelBuffer[spi * 3] = rad(0);
                pixelBuffer[spi * 3 + 1] = rad(1);
                pixelBuffer[spi * 3 + 2] = rad(2);
//                pixels[rowi][coli * 3] = rad(0);
//                pixels[rowi][coli * 3 + 1] = rad(1);
//                pixels[rowi][coli * 3 + 2] = rad(2);
            }
            Spectrum3d pix;
            for (int spi = 0; spi < sampleCount; spi++) {
                pix += Spectrum3d(pixelBuffer[spi * 3], pixelBuffer[spi * 3 + 1], pixelBuffer[spi * 3 + 2]);
            }
            pix /= (sampleCount * 1.0);
//            std::cout<<"("<<rowi<<", "<<coli<<"), pix = "<<pix.transpose()<<std::endl;
            pixels[rowi][coli * 3] = pix(0);
            pixels[rowi][coli * 3 + 1] = pix(1);
            pixels[rowi][coli * 3 + 2] = pix(2);
        }
        if ((rowi + 1) % 50 == 0) {
            std::cout<<"         "<<rowi + 1<<" lines have been rendered."<<std::endl;
        }
    }
    delete[] pixelBuffer;
    std::cout<<"––––––––––––––––––Finish Rendering––––––––––––––––––"<<std::endl;
}

void Renderer::test() {
    camera->generateRays();
    startRendering();
    showImage();
}
