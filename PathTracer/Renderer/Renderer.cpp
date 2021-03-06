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
//#include <opencv2/opencv.hpp>

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
    rays = nullptr;
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

//void Renderer::showImage() const {
//    cv::Mat img = cv::Mat::zeros(yres, xres, CV_8UC3);
//    for (int i = 0; i < yres; i++) {
//        for (int j = 0; j < xres; j++) {
//            img.at<cv::Vec3b>(i, j)[0] = toRBGUint8(pixels[i][j * 3]);
//            img.at<cv::Vec3b>(i, j)[1] = toRBGUint8(pixels[i][j * 3 + 1]);
//            img.at<cv::Vec3b>(i, j)[2] = toRBGUint8(pixels[i][j * 3 + 2]);
//        }
//    }
//    cv::imwrite("result/img.png", img);
//    cv::imshow("window", img);
//    cv::waitKey(0);
//}

void Renderer::writePPM() const {
    FILE *f = fopen("result/img.ppm", "w");         // Write image to PPM file.
    fprintf(f, "P3\n%d %d\n%d\n", xres, yres, 255);
    for (int i = 0; i < yres; i++){
        for (int j = 0; j < xres; j++){
            fprintf(f, "%d %d %d ", toRBGUint8(pixels[i][j * 3]), toRBGUint8(pixels[i][j * 3 + 1]), toRBGUint8(pixels[i][j * 3 + 2]));
        }
    }
}




void Renderer::startRendering() {
    std::cout<<"sampleCount = "<<sampleCount<<std::endl;
    std::cout<<"maxDepth = "<<maxDepth<<std::endl;
    std::cout<<"–––––––––––––––––––Start Rendering––––––––––––––––––"<<std::endl;
    for (int rowi = 0; rowi < yres; rowi++) {
        for (int coli = 0; coli < xres; coli++) {
//    for (int rowi = 120; rowi < 121; rowi++) {
//        for (int coli = 70; coli < 71; coli++) {
            rays = camera->generateRays(rowi, coli);
            double* pixelBuffer = new double[sampleCount * 3]();
            #pragma omp parallel for schedule(dynamic)
            for (int spi = 0; spi < sampleCount; spi++) {
                int offset = spi;
                if (!rays[offset].isInit()) {
                    std::cout<<"This ray has not been initialized !"<<std::endl;
                    continue;
                }
                PathIntegrator path = PathIntegrator(&rays[offset], scene, normalSampler, maxDepth);
                Spectrum3d rad = path.Li();
                pixelBuffer[spi * 3] = rad[0];
                pixelBuffer[spi * 3 + 1] = rad[1];
                pixelBuffer[spi * 3 + 2] = rad[2];
            }
            Spectrum3d pix(0.0, 0.0, 0.0), pix_temp(0.0, 0.0, 0.0);
            for (int spi = 0; spi < sampleCount; spi++) {
                pix_temp = Spectrum3d(pixelBuffer[spi * 3], pixelBuffer[spi * 3 + 1], pixelBuffer[spi * 3 + 2]);
                pix = pix + pix_temp;
            }
            
            pix /= (sampleCount * 1.0);
            pixels[rowi][coli * 3] = pix(0);
            pixels[rowi][coli * 3 + 1] = pix(1);
            pixels[rowi][coli * 3 + 2] = pix(2);
            
            delete[] pixelBuffer;
        }
        if ((rowi + 1) % 50 == 0) {
            std::cout<<"         "<<rowi + 1<<" lines have been rendered."<<std::endl;
        }
    }
    
    std::cout<<"––––––––––––––––––Finish Rendering––––––––––––––––––"<<std::endl;
}


void Renderer::test() {
    time_t start,stop;
    start = time(NULL);
    
    startRendering();
    
    stop = time(NULL);
    printf("Rendering used Time: %ld s\n",(stop-start));
    
    writePPM();
}
