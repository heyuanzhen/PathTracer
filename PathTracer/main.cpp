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
    
    
    int reso[2] = {1000 / 2, 1150 / 2};
    int sampleCount = 4096;
    int maxDepth = 50;
    
    Scene scene = Scene(14, 2);
    
    Spectrum3d zero = Spectrum3d(0.0, 0.0, 0.0);
    Spectrum3d one = Spectrum3d(0.999, 0.999, 0.999);
    Spectrum3d threeQuater = Spectrum3d(0.75, 0.75, 0.75);
    Spectrum3d oneQuater = Spectrum3d(0.25, 0.25, 0.25);
    Spectrum3d half = Spectrum3d(0.5, 0.5, 0.5);
    Spectrum3d red = Spectrum3d(0.0, 0.0, 1.00);
    Spectrum3d blue = Spectrum3d(1.00, 0.0, 0.0);
    
//    illum 4
//    Kd 0.52 0.52 0.52
//    Ka 0.27 0.27 0.27
//    Tf 0.00 0.00 0.00
//    Ni 1.80
//    Ks 1.00 1.00 1.00
//    newmtl blinn2SG
//    illum 4
//    Kd 0.80 0.80 0.80
//    Ka 0.78 0.78 0.78
//    Tf 1.00 1.00 1.00
//    Ni 1.00
//    Ks 0.50 0.50 0.50
//
//    Spectrum3d kd1(0.8, 0.8, 0.8);
//    Spectrum3d ka1(0.78, 0.78, 0.78);
//
//    Spectrum3d kd2(0.52, 0.52, 0.52);
//    Spectrum3d ka2(0.27, 0.27, 0.27);
//    Spectrum3d T(1.0, 1.0, 1.0);
//    double eta = 1.8;
    
    Sphere sphere1 = Sphere(1.58, Point3d(-2.36, 1.585, -1.8), false);
    scene.addShape(&sphere1);
    BlinnPhongBSDF bpBSDF1 = BlinnPhongBSDF(zero, zero, one);
    bpBSDF1.buildBSDF();
    Material mat1 = Material(Material::PHONG, &bpBSDF1);
    sphere1.setMaterial(&mat1);
//
    Sphere sphere2 = Sphere(1.58, Point3d(2.25, 1.585, 1.92), false);
    scene.addShape(&sphere2);
    BlinnPhongBSDF bpBSDF2 = BlinnPhongBSDF(zero, zero, zero, one, 1.8);
    bpBSDF2.buildBSDF();
    Material mat2 = Material(Material::PHONG, &bpBSDF2);
    sphere2.setMaterial(&mat2);
    
    
    BlinnPhongBSDF bpBSDFCeil1 = BlinnPhongBSDF(zero, half, zero);
    Material matCeil1 = Material(Material::PHONG, &bpBSDFCeil1);
    
    BlinnPhongBSDF bpBSDFCeil2 = BlinnPhongBSDF(zero, half, zero);
    Material matCeil2 = Material(Material::PHONG, &bpBSDFCeil2);
    
    BlinnPhongBSDF bpBSDFBack1 = BlinnPhongBSDF(zero, half, zero);
    Material matBack1 = Material(Material::PHONG, &bpBSDFBack1);

    BlinnPhongBSDF bpBSDFBack2 = BlinnPhongBSDF(zero, half, zero);
    Material matBack2 = Material(Material::PHONG, &bpBSDFBack2);

    BlinnPhongBSDF bpBSDFLeft1 = BlinnPhongBSDF(zero, red, zero);
    Material matLeft1 = Material(Material::PHONG, &bpBSDFLeft1);

    BlinnPhongBSDF bpBSDFLeft2 = BlinnPhongBSDF(zero, red, zero);
    Material matLeft2 = Material(Material::PHONG, &bpBSDFLeft2);

    BlinnPhongBSDF bpBSDFRight1 = BlinnPhongBSDF(zero, blue, zero);
    Material matRight1 = Material(Material::PHONG, &bpBSDFRight1);

    BlinnPhongBSDF bpBSDFRight2 = BlinnPhongBSDF(zero, blue, zero);
    Material matRight2 = Material(Material::PHONG, &bpBSDFRight2);

    BlinnPhongBSDF bpBSDFFloor1 = BlinnPhongBSDF(zero, half, zero);
    Material matFloor1 = Material(Material::PHONG, &bpBSDFFloor1);

    BlinnPhongBSDF bpBSDFFloor2 = BlinnPhongBSDF(zero, half, zero);
    Material matFloor2 = Material(Material::PHONG, &bpBSDFFloor2);

    
    Point3d p1(-5.763970, 0.000000, 5.000000);
    Point3d p2(5.763970, 0.000000, 5.000000);
    Point3d p3(-5.763970, 10.000000, 5.000000);
    Point3d p4(5.763970, 10.000000, 5.000000);
    Point3d p5(-5.763970, 10.000000, -5.000000);
    Point3d p6(5.763970, 10.000000, -5.000000);
    Point3d p7(-5.763970, 0.000000, -5.000000);
    Point3d p8(5.763970, 0.000000, -5.000000);
    Triangle ceil1 = Triangle(p3, p4, p6, false);
    Triangle ceil2 = Triangle(p6, p5, p3, false);
    Triangle back1 = Triangle(p5, p6, p8, false);
    Triangle back2 = Triangle(p8, p7, p5, false);
    Triangle floor1 = Triangle(p7, p8, p2, false);
    Triangle floor2 = Triangle(p2, p1, p7, false);
    Triangle right1 = Triangle(p2, p8, p6, false);
    Triangle right2 = Triangle(p6, p4, p2, false);
    Triangle left1 = Triangle(p7, p1, p3, false);
    Triangle left2 = Triangle(p3, p5, p7, false);
    
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
    
    ceil1.setMaterial(&matCeil1);
    ceil2.setMaterial(&matCeil2);
    back1.setMaterial(&matBack1);
    back2.setMaterial(&matBack2);
    floor1.setMaterial(&matFloor1);
    floor2.setMaterial(&matFloor2);
    right1.setMaterial(&matRight1);
    right2.setMaterial(&matRight2);
    left1.setMaterial(&matLeft1);
    left2.setMaterial(&matLeft2);
    
//    PointLight pl1 = PointLight(Point3d(0.0, 8.0, 0.0), Spectrum3d(40.0, 40.0, 40.0));
//    scene.addLight(&pl1);
//    PointLight pl2 = PointLight(Point3d(2.0, 2.0, 0.0), Spectrum3d(3.0, 2.0, 1.0));
//    scene->addLight(&pl2);
    BlinnPhongBSDF bsdfTriLight1 = BlinnPhongBSDF(zero, one, zero);
    Material matTriLight1 = Material(Material::PHONG, &bsdfTriLight1);
    
    BlinnPhongBSDF bsdfTriLight2 = BlinnPhongBSDF(zero, one, zero);
    Material matTriLight2 = Material(Material::PHONG, &bsdfTriLight2);
    
    Point3d lA(-1.05, 9.99, -1.05), lB(1.05, 9.99, -1.05), lC(1.05, 9.99, 1.05), lD(-1.05, 9.99, 1.05);
    Point3d tl(-3.0, 0.0, 0.0);
    
    Spectrum3d li(50.0, 50.0, 50.0);
//    Spectrum3d li(10.0, 10.0, 10.0);
    Triangle triLightShape1 = Triangle(lA, lC, lB, true);
    scene.addShape(&triLightShape1);
    triLightShape1.setMaterial(&matTriLight1);
    AreaLight triLight1 = AreaLight(li, &triLightShape1);
    triLightShape1.setAreaLight(&triLight1);
    scene.addLight(&triLight1);
    
    Triangle triLightShape2 = Triangle(lA, lD, lC, true);
    scene.addShape(&triLightShape2);
    triLightShape2.setMaterial(&matTriLight2);
    AreaLight triLight2 = AreaLight(li, &triLightShape2);
    triLightShape2.setAreaLight(&triLight2);
    scene.addLight(&triLight2);
    
    StratifiedSampler pixelSampler = StratifiedSampler(sampleCount);
    RandomSampler normalSampler = RandomSampler();
    double lookAt[9] = {0.0,5.0,0.0, 0.0,5.0,18.0, 0.0,1.0,0.0};
    double fov = M_PI / 4.0;
    Renderer renderer = Renderer(reso, sampleCount, maxDepth, &scene,
                                 &pixelSampler, &normalSampler, lookAt, fov);
    renderer.test();
}

void test() {
    Point3d p0(-3, 0.0, 0.0), p1(0, -3, 0.0), p2(0, 3, 0.0);
    Triangle tri1 = Triangle(p0, p1, p2, true);
    cout<<tri1.Area()<<endl;
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
