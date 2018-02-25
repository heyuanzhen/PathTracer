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

using namespace std;

void test(){
    srand((unsigned)time(NULL));
    rand();
    
//    int spC = 1, liC = 0;
//    float r = 0.5;
//    Vector3f cenPos = Vector3f(0.0, 0.0, 0.0);
//    Scene* scene = new Scene(spC, liC);
//    Sphere* sphere1 = new Sphere(r, cenPos);
//    scene->addShape(sphere1);
//
//    Ray r1 = Ray(Vector3f(0.5, 0.0, 2.0), Vector3f(0.0, 0.0, -1.0), 0.0);
//    Shape* sp = scene->getShape(0);
//    float ds[3] = {0.5, 0.0, 0.0};
//    sp->trans.setTranslation(ds);
//    Intersection* its = nullptr;
//    float t = sp->isIntersected(&r1, its);
//    cout<<t<<endl;
//
//    delete sphere1;
//    delete scene;
    
    float lookAt[9] = {0.0,0.0,0.0, 0.0, 0.0,2.0, 0.0,1.0,0.0};
    int reso[2] = {256, 256};
    float fov = M_PI / 4.0;
    
    StratifiedSampler sp(1);
    PerspectiveCamera pc(lookAt, reso, fov, 1, &sp);
    Point2f pi(0.0, 0.0);
    Point3f pw = pc.imgToWorld(pi);
    pc.generateRays();
//    cout<<pw.transpose()<<endl;
    
//    cout<<sp.get2D().transpose()<<endl;
    
//    for (int i = 0; i < 20; i++) {
//        cout<<sp.get2D().transpose()<<endl;
//    }
}

int main(int argc, const char * argv[]) {
    test();
    return 0;
}
