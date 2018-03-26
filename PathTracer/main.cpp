//
//  main.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/3.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include <iostream>

#include "Examples.h"

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
//    cbox();
    veach();
//    test();
//    parallelTest();
    return 0;
}
