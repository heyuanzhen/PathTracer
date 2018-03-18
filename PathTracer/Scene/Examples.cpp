//
//  Examples.cpp
//  PathTracer
//
//  Created by HYZ on 2018/3/18.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include <iostream>
#include "Examples.h"
#include "typeAlias.h"
#include "Scene.h"
#include "Shape.h"
#include "Light.h"
#include "Renderer.h"

using namespace std;

void cbox(){
    srand((unsigned)time(NULL));
    rand();
    
    
    int reso[2] = {1000 / 2, 1150 / 2};
    int sampleCount = 3600;
    int maxDepth = 20;
    
    Scene scene = Scene(8, 1);
    
    Spectrum3d zero = Spectrum3d(0.0, 0.0, 0.0);
    Spectrum3d one = Spectrum3d(1.0, 1.0, 1.0);
    Spectrum3d threeQuater = Spectrum3d(0.75, 0.75, 0.75);
    Spectrum3d oneQuater = Spectrum3d(0.25, 0.25, 0.25);
    Spectrum3d half = Spectrum3d(0.5, 0.5, 0.5);
    Spectrum3d red = Spectrum3d(0.1, 0.1, 0.9);
    Spectrum3d blue = Spectrum3d(0.9, 0.1, 0.1);

    
    Sphere sphere1 = Sphere(1.98, Point3d(-2.56, 1.985, -1.5), false);
    scene.addShape(&sphere1);
    BlinnPhongBSDF bpBSDF1 = BlinnPhongBSDF(zero, zero, one);
    bpBSDF1.buildBSDF();
    Material mat1 = Material(Material::PHONG, &bpBSDF1);
    sphere1.setMaterial(&mat1);
    //
    Sphere sphere2 = Sphere(1.98, Point3d(2.95, 1.985, 1.52), false);
    scene.addShape(&sphere2);
    FresnelBSDF bpBSDF2 = FresnelBSDF(one, one, 1.0, 1.5);
    bpBSDF2.buildBSDF();
    Material mat2 = Material(Material::FRESNEL, &bpBSDF2);
    sphere2.setMaterial(&mat2);
    
    
    BlinnPhongBSDF bpBSDFCeil = BlinnPhongBSDF(zero, threeQuater, zero);
    Material matCeil = Material(Material::PHONG, &bpBSDFCeil);
    
    BlinnPhongBSDF bpBSDFBack = BlinnPhongBSDF(zero, threeQuater, zero);
    Material matBack = Material(Material::PHONG, &bpBSDFBack);
    
    BlinnPhongBSDF bpBSDFLeft = BlinnPhongBSDF(zero, red, zero);
    Material matLeft = Material(Material::PHONG, &bpBSDFLeft);
    
    BlinnPhongBSDF bpBSDFRight = BlinnPhongBSDF(zero, blue, zero);
    Material matRight = Material(Material::PHONG, &bpBSDFRight);
    
    BlinnPhongBSDF bpBSDFFloor = BlinnPhongBSDF(zero, threeQuater, zero);
    Material matFloor = Material(Material::PHONG, &bpBSDFFloor);
    
    
    Point3d p1(-5.763970, 0.000000, 5.000000);
    Point3d p2(5.763970, 0.000000, 5.000000);
    Point3d p3(-5.763970, 10.000000, 5.000000);
    Point3d p4(5.763970, 10.000000, 5.000000);
    Point3d p5(-5.763970, 10.000000, -5.000000);
    Point3d p6(5.763970, 10.000000, -5.000000);
    Point3d p7(-5.763970, 0.000000, -5.000000);
    Point3d p8(5.763970, 0.000000, -5.000000);

    Rectangular ceil = Rectangular(p3, p4, p6, false);
    Rectangular back = Rectangular(p5, p6, p8, false);
    Rectangular floor = Rectangular(p7, p8, p2, false);
    Rectangular right = Rectangular(p2, p8, p6, false);
    Rectangular left = Rectangular(p7, p1, p3, false);

    scene.addShape(&ceil);
    scene.addShape(&back);
    scene.addShape(&floor);
    scene.addShape(&right);
    scene.addShape(&left);

    ceil.setMaterial(&matCeil);
    back.setMaterial(&matBack);
    floor.setMaterial(&matFloor);
    right.setMaterial(&matRight);
    left.setMaterial(&matLeft);
    
    Point3d lA(-1.05, 9.99, -1.05), lB(1.05, 9.99, -1.05), lC(1.05, 9.99, 1.05), lD(-1.05, 9.99, 1.05);
    Spectrum3d li(40.0, 40.0, 40.0);
//    Point3d lA(-3.05, 9.99, -3.05), lB(3.05, 9.99, -3.05), lC(3.05, 9.99, 3.05), lD(-3.05, 9.99, 3.05);
//    Spectrum3d li(4.0, 4.0, 4.0);
//    Spectrum3d li(2.0, 2.0, 2.0);

    BlinnPhongBSDF bsdfRecLight = BlinnPhongBSDF(zero, one, zero);
    Material matRecLight = Material(Material::PHONG, &bsdfRecLight);
    Rectangular recLightShape = Rectangular(lA, lD, lC, true);
    scene.addShape(&recLightShape);
    recLightShape.setMaterial(&matRecLight);
    AreaLight recLight = AreaLight(li, &recLightShape);
    recLightShape.setAreaLight(&recLight);
    scene.addLight(&recLight);
    
    StratifiedSampler pixelSampler = StratifiedSampler(sampleCount);
    RandomSampler normalSampler = RandomSampler();
    double lookAt[9] = {0.0,5.0,0.0, 0.0,5.0,19.0, 0.0,1.0,0.0};
    double fov = M_PI / 4.0;
    Renderer renderer = Renderer(reso, sampleCount, maxDepth, &scene,
                                 &pixelSampler, &normalSampler, lookAt, fov);
    renderer.test();
}


void veach() {
//    v -10 -4.14615 -10
//    v -10 -4.14615  10
//    v  10 -4.14615  10
//
//    v  10 -4.14615  10
//    v  10 -4.14615 -10
//    v -10 -4.14615 -10
//
//    v -10 -10 -2
//    v  10 -10 -2
//    v  10  10 -2
//
//    v  10  10 -2
//    v -10  10 -2
//    v -10 -10 -2
//    f 1 2 3
//    f 4 5 6
//    f 7 8 9
//    f 10 11 12
    srand((unsigned)time(NULL));
    rand();
    
    
    int reso[2] = {512 / 2, 768 / 2};
    int sampleCount = 100;
    int maxDepth = 10;
    
    Scene scene = Scene(3, 1);
    
    Spectrum3d zero(0.0, 0.0, 0.0);
    Spectrum3d little(0.001, 0.001, 0.001);
    Spectrum3d li1(800.0, 800.0, 800.0);
    Spectrum3d floorColor(0.4, 0.4, 0.4);
    
    Point3d pfloor1(-10, -4.14615, -10), pfloor2(-10, -4.14615, 10), pfloor3(10, -4.14615, 10);
    Point3d pfloor4(-10, -10, -2), pfloor5(10, -10, 2), pfloor6(10, 10, -2);
    Point3d cen1(10.0, 10.0, 4.0);

    
    Rectangular floor1 = Rectangular(pfloor1, pfloor2, pfloor3, false);
    Rectangular floor2 = Rectangular(pfloor4, pfloor5, pfloor6, false);
    Sphere sphere1 = Sphere(0.5, cen1, true);
    
    
    BlinnPhongBSDF bsdfFloor1 = BlinnPhongBSDF(zero, floorColor, zero);
    Material matFloor1 = Material(Material::PHONG, &bsdfFloor1);
    BlinnPhongBSDF bsdfFloor2 = BlinnPhongBSDF(zero, floorColor, zero);
    Material matFloor2 = Material(Material::PHONG, &bsdfFloor2);
    BlinnPhongBSDF bsdfSphere1 = BlinnPhongBSDF(zero, little * 1000, zero);
    Material matSphere1 = Material(Material::PHONG, &bsdfSphere1);
    
    AreaLight sphereLight1 = AreaLight(li1, &sphere1);

    scene.addShape(&floor1);
    scene.addShape(&floor2);
    scene.addShape(&sphere1);
    
    floor1.setMaterial(&matFloor1);
    floor2.setMaterial(&matFloor2);
    sphere1.setMaterial(&matSphere1);
    
    
    sphere1.setAreaLight(&sphereLight1);
    scene.addLight(&sphereLight1);
    
    StratifiedSampler pixelSampler = StratifiedSampler(sampleCount);
    RandomSampler normalSampler = RandomSampler();
    double lookAt[9] = {0.0,2.0,15.0, 0.0,-2.0,2.5, 0.0,1.0,0.0};
    double fov = M_PI * 28.0 / 180.0;
    Renderer renderer = Renderer(reso, sampleCount, maxDepth, &scene,
                                 &pixelSampler, &normalSampler, lookAt, fov);
    renderer.test();
}







