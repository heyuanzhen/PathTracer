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
    int sampleCount = 64;
    int maxDepth = 2;
    
    Scene* scene = new Scene(2, 1);
    
    Sphere sphere1 = Sphere(0.5, Point3d(0.3, 0.0, 0.0));
//    Sphere sphere1 = Sphere(0.5, Point3d(0.0, 0.0, 0.0));
    scene->addShape(&sphere1);
    Sphere sphere2 = Sphere(0.2, Point3d(-0.5, 0.0, 0.0));
    scene->addShape(&sphere2);
    
    BlinnPhongBSDF bpBSDF1 = BlinnPhongBSDF(0.0, 1.0, 0.2, 2.0);
    bpBSDF1.buildBSDF();
    BlinnPhongBSDF bpBSDF2 = BlinnPhongBSDF(0.0, 1.0, 1.0, 1.0);
    bpBSDF2.buildBSDF();
    
    Material mat1 = Material(Material::PHONG, &bpBSDF1);
    Material mat2 = Material(Material::PHONG, &bpBSDF2);
    sphere1.setMaterial(&mat1);
    sphere2.setMaterial(&mat2);
    
    PointLight pl1 = PointLight(Point3d(-2.0, 0.0, 2.0), Spectrum3d(1.0, 2.0, 3.0));
    scene->addLight(&pl1);
//    PointLight pl2 = PointLight(Point3d(2.0, 2.0, 0.0), Spectrum3d(3.0, 2.0, 1.0));
//    scene->addLight(&pl2);
    
    Sampler* pixelSampler = new StratifiedSampler(sampleCount);
    Sampler* normalSampler = new RandomSampler();
    double lookAt[9] = {0.0,0.0,0.0, 0.0,0.0,2.0, 0.0,1.0,0.0};
    double fov = M_PI / 4.0;
    Renderer renderer = Renderer(reso, sampleCount, maxDepth, scene,
                                 pixelSampler, normalSampler, lookAt, fov);
    renderer.test();
    
    delete scene;
    delete pixelSampler;
    delete normalSampler;
}

void test() {
//    Scene scene = Scene(1, 1);
//    PointLight pl = PointLight(Point3d(1.5, 1.0, 0.0), Spectrum3d(1.0, 1.0, 1.0));
//    scene.addLight(&pl);
//    Sphere sphere1 = Sphere(1.0, Point3d(1.0, 0.0, 0.0));
//    scene.addShape(&sphere1);
//    bool vis = pl.testVisibility( pl.getPos(), Point3d(1.0, 1.0, 0.0), &scene);
//    cout<<vis<<endl;
    Vector3d a(1.0, 2.0, 3.0);
    Vector3d b(4.0, 5.0, 6.0);
    cout<<a.cwiseProduct(b).transpose()<<endl;
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
