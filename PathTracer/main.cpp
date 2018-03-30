//
//  main.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/3.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include <iostream>

#include "Examples.h"
#include "FileParser.h"

using namespace std;


void parallelTest(){
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < 100; i++) {
        cout<<i<<endl;
    }
}

int main(int argc, const char * argv[]) {
    srand((unsigned)time(NULL));
    rand();
    
    string fileName = "config.txt";
    FileParser fp(fileName);
    fp.readAndSchedule();
    return 0;
}
