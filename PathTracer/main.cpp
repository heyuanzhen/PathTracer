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
    int spc = 1;
    Renderer renderer = Renderer(reso, spc);
    renderer.test();
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
