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
    
    int spC = 1, liC = 0;
    float r = 0.5;
    Vector3f cenPos = Vector3f(0.0, 0.0, 0.0);
    Scene* scene = new Scene(spC, liC);
    Sphere* sphere1 = new Sphere(r, cenPos);
    scene->addShape(sphere1);
    
    Ray r1 = Ray(Vector3f(0.5, 0.0, 2.0), Vector3f(0.0, 0.0, -1.0), 0.0);
    Shape* sp = scene->getShape(0);
    float ds[3] = {0.5, 0.0, 0.0};
    sp->trans.setTranslation(ds);
    Intersection* its = nullptr;
    float t = sp->isIntersected(&r1, its);
    cout<<t<<endl;
    
    delete sphere1;
    delete scene;
}

int main(int argc, const char * argv[]) {
    test();
    return 0;
}
