//
//  main.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/3.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include <iostream>

#include "Sampler.h"
#include "sampling.h"
#include "BxDF.h"
#include "Material.h"
#include "Camera.h"
#include "Scene.h"
#include "Shape.h"
#include "Renderer.h"
#include "Light.h"

using namespace std;

void exapmle(){
    srand((unsigned)time(NULL));
    rand();
    
    
    int reso[2] = {300, 400};
    int sampleCount = 1024;
    int maxDepth = 2;
    
    Scene scene = Scene(10, 1);
    
    Spectrum3d zero = Spectrum3d(0.0, 0.0, 0.0);
    Spectrum3d one = Spectrum3d(1.0, 1.0, 1.0);
    
//    Sphere sphere1 = Sphere(0.5, Point3d(0.3, 0.0, 0.0));
//    scene.addShape(&sphere1);
//    BlinnPhongBSDF bpBSDF1 = BlinnPhongBSDF(zero, one, zero, 0.0);
//    bpBSDF1.buildBSDF();
//    Material mat1 = Material(Material::PHONG, &bpBSDF1);
//    sphere1.setMaterial(&mat1);
//
//    Sphere sphere2 = Sphere(0.2, Point3d(-0.5, 0.0, 0.0));
//    scene.addShape(&sphere2);
//    BlinnPhongBSDF bpBSDF2 = BlinnPhongBSDF(zero, zero, one, 0.0);
//    bpBSDF2.buildBSDF();
//    Material mat2 = Material(Material::PHONG, &bpBSDF2);
//    sphere2.setMaterial(&mat2);
    
    Point3d pt0(-5, 0.0, -5.0), pt1(0, 5, -5.0), pt2(0, -5, -5.0);
//    Triangle tri1 = Triangle(pt0, pt2, pt1);
//    scene.addShape(&tri1);
    BlinnPhongBSDF bpBSDF3 = BlinnPhongBSDF(zero, one, zero, 0.0);
    bpBSDF3.buildBSDF();
    Material mat3 = Material(Material::PHONG, &bpBSDF3);
//    tri1.setMaterial(&mat3);
    
    Point3d p1(-5.763970, 0.000000, 5.000000);
    Point3d p2(5.763970, 0.000000, 5.000000);
    Point3d p3(-5.763970, 10.000000, 5.000000);
    Point3d p4(5.763970, 10.000000, 5.000000);
    Point3d p5(-5.763970, 10.000000, -5.000000);
    Point3d p6(5.763970, 10.000000, -5.000000);
    Point3d p7(-5.763970, 0.000000, -5.000000);
    Point3d p8(5.763970, 0.000000, -5.000000);
    Triangle ceil1 = Triangle(p3, p4, p6);
    Triangle ceil2 = Triangle(p6, p5, p3);
    Triangle back1 = Triangle(p5, p6, p8);
    Triangle back2 = Triangle(p8, p7, p5);
    Triangle floor1 = Triangle(p7, p8, p2);
    Triangle floor2 = Triangle(p2, p1, p7);
    Triangle right1 = Triangle(p2, p8, p6);
    Triangle right2 = Triangle(p6, p4, p2);
    Triangle left1 = Triangle(p7, p1, p3);
    Triangle left2 = Triangle(p3, p5, p7);
    
    scene.addShape(&ceil1);
    scene.addShape(&ceil2);
    scene.addShape(&back1);
    scene.addShape(&back2);
    scene.addShape(&floor1);
    scene.addShape(&floor2);
    scene.addShape(&right1);
    scene.addShape(&right2);
    scene.addShape(&left1);
    scene.addShape(&left2);
    
    ceil1.setMaterial(&mat3);
    ceil2.setMaterial(&mat3);
    back1.setMaterial(&mat3);
    back2.setMaterial(&mat3);
    floor1.setMaterial(&mat3);
    floor2.setMaterial(&mat3);
    right1.setMaterial(&mat3);
    right2.setMaterial(&mat3);
    left1.setMaterial(&mat3);
    left2.setMaterial(&mat3);
    
    PointLight pl1 = PointLight(Point3d(0.0, 5.0, 0.0), Spectrum3d(20.0, 40.0, 60.0));
    scene.addLight(&pl1);
//    PointLight pl2 = PointLight(Point3d(2.0, 2.0, 0.0), Spectrum3d(3.0, 2.0, 1.0));
//    scene->addLight(&pl2);
    
    StratifiedSampler pixelSampler = StratifiedSampler(sampleCount);
    RandomSampler normalSampler = RandomSampler();
    double lookAt[9] = {0.0,5.0,0.0, 0.0,5.0,25.0, 0.0,1.0,0.0};
    double fov = M_PI / 4.0;
    Renderer renderer = Renderer(reso, sampleCount, maxDepth, &scene,
                                 &pixelSampler, &normalSampler, lookAt, fov);
    renderer.test();
}

void test() {
    Point3d p0(-0.3, 0.0, 0.0), p1(-0.1, -0.3, 0.0), p2(-0.1, 0.3, 0.0);
    Triangle tri1 = Triangle(p0, p1, p2);
    Point3d o(0.0, 0.0, 2.0), p(-0.1, 0.0, 0.0);
    Vector3d d = (p - o).normalized();
    Ray ray = Ray(o, d, 0.0);
    double t = tri1.isIntersected(&ray);
    cout<<"t = "<<t<<"n = "<<tri1.getNormal(p).transpose()<<endl;
}

void parallelTest(){
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < 100; i++) {
        cout<<i<<endl;
    }
}

int main(int argc, const char * argv[]) {
    exapmle();
//    test();
//    parallelTest();
    return 0;
}
