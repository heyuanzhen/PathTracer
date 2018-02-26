//
//  Renderer.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/26.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Renderer.h"
#include "Shape.h"
#include <iostream>

#include <opencv2/opencv.hpp>

Renderer::Renderer(int* reso, int spc) {
    xres = reso[1];
    yres = reso[0];
    sampleCount = spc;
    scene = nullptr;
    camera = nullptr;
    sampler = nullptr;
    isInit = false;
    rays = new Ray[xres * yres * sampleCount]();
    pixels = new float*[yres]();
    for (int i = 0; i < yres; i++) {
        pixels[i] = new float[xres * 3](); //3 channels
    }
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
    cv::Mat img = cv::Mat::zeros(yres, xres, CV_32FC3);
    for (int i = 0; i < yres; i++) {
        for (int j = 0; j < xres; j++) {
            img.at<cv::Vec3f>(i, j)[0] = pixels[i][j * 3];
            img.at<cv::Vec3f>(i, j)[1] = pixels[i][j * 3 + 1];
            img.at<cv::Vec3f>(i, j)[2] = pixels[i][j * 3 + 2];
        }
    }
    cv::imshow("window", img);
    cv::waitKey(0);
}

void Renderer::initRenderer(Scene *scene, Camera *camera, Sampler* sampler) {
    this->scene = scene;
    this->camera = camera;
    this->sampler = sampler;
}

void Renderer::startRendering() {
//    int ind = 150 * xres + 200;
//    Point3f o = rays[ind].getOrigin();
//    Vector3f d = rays[ind].getDirection();
//    float t = rays[ind].getT();
//    std::cout<<"o = "<<o.transpose()<<", d = "<<d.transpose()<<", t = "<<t<<std::endl;
//    rays[ind].findIntersection(scene);
//    std::cout<<rays[ind].getIntersection()->getIsInter()<<std::endl;
    
//    #pragma omp parallel for schedule(dynamic)
    for (int rowi = 0; rowi < yres; rowi++) {
        for (int coli = 0; coli < xres; coli++) {
            for (int spi = 0; spi < sampleCount; spi++) {
                int offset = (rowi * xres + coli) * sampleCount + spi;
                rays[offset].findIntersection(scene);
                if (rays[offset].getIntersection()->getIsInter()) {
                    pixels[rowi][coli * 3] = 1.0;
                    pixels[rowi][coli * 3 + 1] = 1.0;
                    pixels[rowi][coli * 3 + 2] = 1.0;
                }
            }
        }
    }
}

void Renderer::test() {
    scene = new Scene(1, 1);
    Sphere sphere1 = Sphere(0.5, Point3f(0.3, 0.0, 0.0));
    scene->addShape(&sphere1);
    sampler = new StratifiedSampler(1);
    float lookAt[9] = {0.0,0.0,0.0, 0.0,0.0,2.0, 0.0,1.0,0.0};
    int reso[2] = {yres, xres};
    float fov = M_PI / 4.0;
    int sampleCount = 1;
    camera = new PerspectiveCamera(lookAt, reso, fov, sampleCount, sampler, rays);
    camera->generateRays();
    startRendering();
    showImage();

    delete scene;
    delete sampler;
    delete camera;
}
