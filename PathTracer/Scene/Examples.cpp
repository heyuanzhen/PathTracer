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
    int sampleCount = 4;
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
    double lookAt[9] = {0.0,5.0,19.0, 0.0,5.0,0.0, 0.0,1.0,0.0};
    double fov = M_PI / 4.0;
    Renderer renderer = Renderer(reso, sampleCount, maxDepth, &scene,
                                 &pixelSampler, &normalSampler, lookAt, fov);
    renderer.test();
}


void veach() {
    srand((unsigned)time(NULL));
    rand();
    
    
    int reso[2] = {512 / 2, 768 / 2};
    int sampleCount = 256;
    int maxDepth = 20;
    
    Scene scene = Scene(7, 4);
    
    Spectrum3d zero = Spectrum3d(0.0, 0.0, 0.0);
    Spectrum3d little = Spectrum3d(0.001, 0.001, 0.001);
    double lightRatio = 1.0;
//    Spectrum3d li0 = Spectrum3d(800, 800, 800) * lightRatio;
    Spectrum3d li1 = Spectrum3d(100, 100, 100) * lightRatio;
    Spectrum3d li2 = Spectrum3d(901.803, 901.803, 901.803) * lightRatio;
    Spectrum3d li3 = Spectrum3d(11.1111, 11.1111, 11.1111) * lightRatio;
    Spectrum3d li4 = Spectrum3d(1.23457, 1.23457, 1.23457) * lightRatio;
    Spectrum3d floorColor = Spectrum3d(0.05, 0.05, 0.05);
    Spectrum3d plate1Diffuse = Spectrum3d(0.13, 0.09, 0.07);
    Spectrum3d plate1Specular = Spectrum3d(0.09, 0.09, 0.09);
    
    Point3d pfloor1(-10, -4.14615, -10), pfloor2(-10, -4.14615, 10), pfloor3(10, -4.14615, 10);
    Point3d pfloor4(-10, -10, -2), pfloor5(10, -10, -2), pfloor6(10, 10, -2);
    Point3d pPlate1(4, -2.70651, 0.25609), pPlate2(4, -2.08375, -0.526323), pPlate3(-4, -2.08375, -0.526323);

    Point3d cen0(10.0, 10.0, 4.0);
    Point3d cen1(-1.25, 0.0, 0.0);
    Point3d cen2(-3.75, 0.0, 0.0);
    Point3d cen3(1.25, 0.0, 0.0);
    Point3d cen4(3.75, 0.0, 0.0);

    
    Rectangular floor1 = Rectangular(pfloor1, pfloor2, pfloor3, false);
    Rectangular floor2 = Rectangular(pfloor4, pfloor5, pfloor6, false);
//    Sphere sphere0 = Sphere(0.5, cen0, true);
    Sphere sphere1 = Sphere(0.1, cen1, true);
    Sphere sphere2 = Sphere(0.03333, cen2, true);
    Sphere sphere3 = Sphere(0.3, cen3, true);
    Sphere sphere4 = Sphere(0.9, cen4, true);
    Rectangular plate1 = Rectangular(pPlate3, pPlate2, pPlate1, false);
    
    floor1.setNormal(Vector3d(0.0, 1.0, 0.0));
    floor2.setNormal(Vector3d(0.0, 0.0, 1.0));
    
    
    BlinnPhongBSDF bsdfFloor1 = BlinnPhongBSDF(zero, floorColor, zero);
    Material matFloor1 = Material(Material::PHONG, &bsdfFloor1);
    BlinnPhongBSDF bsdfFloor2 = BlinnPhongBSDF(zero, floorColor, zero);
    Material matFloor2 = Material(Material::PHONG, &bsdfFloor2);
//    BlinnPhongBSDF bsdfSphere0 = BlinnPhongBSDF(zero, little, zero);
//    Material matSphere0 = Material(Material::PHONG, &bsdfSphere0);
    BlinnPhongBSDF bsdfSphere1 = BlinnPhongBSDF(zero, little, zero);
    Material matSphere1 = Material(Material::PHONG, &bsdfSphere1);
    BlinnPhongBSDF bsdfSphere2 = BlinnPhongBSDF(zero, little, zero);
    Material matSphere2 = Material(Material::PHONG, &bsdfSphere2);
    BlinnPhongBSDF bsdfSphere3 = BlinnPhongBSDF(zero, little, zero);
    Material matSphere3 = Material(Material::PHONG, &bsdfSphere3);
    BlinnPhongBSDF bsdfSphere4 = BlinnPhongBSDF(zero, little, zero);
    Material matSphere4 = Material(Material::PHONG, &bsdfSphere4);
    BlinnPhongBSDF bsdfPlate1 = BlinnPhongBSDF(zero, plate1Diffuse, plate1Specular);
    Material matPlate1 = Material(Material::PHONG, & bsdfPlate1);
    
//    AreaLight sphereLight0 = AreaLight(li0, &sphere0);
    AreaLight sphereLight1 = AreaLight(li1, &sphere1);
    AreaLight sphereLight2 = AreaLight(li2, &sphere2);
    AreaLight sphereLight3 = AreaLight(li3, &sphere3);
    AreaLight sphereLight4 = AreaLight(li4, &sphere4);

    scene.addShape(&floor1);
    scene.addShape(&floor2);
//    scene.addShape(&sphere0);
    scene.addShape(&sphere1);
    scene.addShape(&sphere2);
    scene.addShape(&sphere3);
    scene.addShape(&sphere4);
    scene.addShape(&plate1);
    
    floor1.setMaterial(&matFloor1);
    floor2.setMaterial(&matFloor2);
//    sphere0.setMaterial(&matSphere0);
    sphere1.setMaterial(&matSphere1);
    sphere2.setMaterial(&matSphere2);
    sphere3.setMaterial(&matSphere3);
    sphere4.setMaterial(&matSphere4);
    plate1.setMaterial(&matPlate1);
    
    
//    sphere0.setAreaLight(&sphereLight0);
    sphere1.setAreaLight(&sphereLight1);
    sphere2.setAreaLight(&sphereLight2);
    sphere3.setAreaLight(&sphereLight3);
    sphere4.setAreaLight(&sphereLight4);
//    scene.addLight(&sphereLight0);
    scene.addLight(&sphereLight1);
    scene.addLight(&sphereLight2);
    scene.addLight(&sphereLight3);
    scene.addLight(&sphereLight4);
    
    StratifiedSampler pixelSampler = StratifiedSampler(sampleCount);
    RandomSampler normalSampler = RandomSampler();
    double lookAt[9] = {0.0,2.0,15.0, 0.0,-2.0,2.5, 0.0,1.0,0.0};
    double fov = 0.7157733105;
    Renderer renderer = Renderer(reso, sampleCount, maxDepth, &scene,
                                 &pixelSampler, &normalSampler, lookAt, fov);
    renderer.test();
}







