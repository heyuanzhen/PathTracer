//
//  Sampler.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/3.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Sampler.h"
#include <iostream>

////abstract sampler
Sampler::Sampler(){}

Sampler::~Sampler(){}

double Sampler::random1D() const{
    double xi = rand() * 1.0 / RAND_MAX;
    return xi;
}

Point2d Sampler::random2D() const{
    double xi1 = rand() * 1.0 / RAND_MAX;
    double xi2 = rand() * 1.0 / RAND_MAX;
    Point2d xi = Point2d(xi1, xi2);
    return xi;
}


////StratifiedSampler
StratifiedSampler::StratifiedSampler(const int _blocks) : blocks(_blocks){
    currentNumberIndex = 0;
    clipInputBlocks();
    randNumArr = new Point2d[blocks]();
    initializeSampler();
}

StratifiedSampler::~StratifiedSampler(){
    delete[] randNumArr;
}

void StratifiedSampler::clipInputBlocks(){
    if (blocks < 1) {
        std::cout<<"ERROR: Wrong blocks number !"<<std::endl;
        exit(1);
    }
    else{
        blocksPerAxis = static_cast<int>(sqrt(blocks));
        blocks = blocksPerAxis * blocksPerAxis;
    }
//    std::cout<<"blocks: "<<blocks<<std::endl;
}

void StratifiedSampler::initializeSampler(){
    genRandNumArr2D();
}

void StratifiedSampler::genRandNumArr2D(){
    double blocksDelta = 1.0 / blocksPerAxis;
    Point2d bias = Point2d(blocksDelta * 0.5, blocksDelta * 0.5);
    double curX, curY;
    Point2d jitter;
    for (int rowi = 0; rowi < blocksPerAxis; rowi++) {
        for (int coli = 0; coli < blocksPerAxis; coli++) {
            jitter = random2D() * blocksDelta - bias;
            curY = (rowi + 0.5) * blocksDelta + jitter[0];
            curX = (coli + 0.5) * blocksDelta + jitter[1];
            randNumArr[rowi * blocksPerAxis + coli] = Point2d(curX, curY);
//            std::cout<<"(i, j) = ("<<rowi<<", "<<coli<<")"
//            <<"curX = "<<Point2d(curX, curY).transpose()<<std::endl;
        }
    }
    currentNumberIndex = 0;
}

bool StratifiedSampler::noNumRemains(){
    return currentNumberIndex >= blocks;
}

double StratifiedSampler::get1D(){
    std::cout<<"ERROR: You can not use get1D() in stratified sampling !"<<std::endl;
    system("pause");
    exit(1);
}

Point2d StratifiedSampler::get2D(){
    if (noNumRemains()) {
        genRandNumArr2D();
//        std::cout<<"ERROR: No random number remains in stratified sampler!"<<std::endl;
//        exit(1);
    }
    Point2d xi = randNumArr[currentNumberIndex];
    currentNumberIndex++;
    return xi;
}




////RandomSampler
RandomSampler::RandomSampler(){
    initializeSampler();
}
RandomSampler::~RandomSampler(){}

void RandomSampler::initializeSampler(){}

double RandomSampler::get1D(){
    return random1D();
}

Point2d RandomSampler::get2D(){
    return random2D();
}

