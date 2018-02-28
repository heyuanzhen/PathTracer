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

using namespace std;

void test(){
    srand((unsigned)time(NULL));
    rand();
    
    
    int reso[2] = {300, 400};
    int sampleCount = 4;
    int maxDepth = 2;
    
    Scene* scene = new Scene(2, 1);
    Sphere sphere1 = Sphere(0.5, Point3f(0.3, 0.0, 0.0));
    scene->addShape(&sphere1);
    Sphere sphere2 = Sphere(0.1, Point3f(-0.2, 0.0, 0.1));
    scene->addShape(&sphere2);
    Sampler* sampler = new StratifiedSampler(sampleCount);
    float lookAt[9] = {0.0,0.0,0.0, 0.0,0.0,2.0, 0.0,1.0,0.0};
    float fov = M_PI / 4.0;
    Ray* rays = new Ray[reso[1] * reso[0] * sampleCount]();
    Camera* camera = new PerspectiveCamera(lookAt, reso, fov, sampleCount, sampler, rays);
    
    Renderer renderer = Renderer(reso, sampleCount, maxDepth, scene, camera, sampler, rays);
    renderer.test();
    
    delete scene;
    delete sampler;
    delete camera;
}

void parallelTest(){
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < 100; i++) {
        cout<<i<<endl;
    }
}

int main(int argc, const char * argv[]) {
    test();
//    parallelTest();
    return 0;
}
