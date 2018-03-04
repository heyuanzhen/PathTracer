//
//  Sampler.h
//  PathTracer
//
//  Created by HYZ on 2018/2/3.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#ifndef Sampler_h
#define Sampler_h

#include <eigen3/Eigen/Dense>
#include "typeAlias.h"

class Sampler{
protected:
public:
    Sampler();
    virtual ~Sampler(); //virtual
    
    double random1D() const;
    Point2f random2D() const;
    virtual void initializeSampler() = 0;
    virtual double get1D() = 0;
    virtual Point2f get2D() = 0;
};

class StratifiedSampler : public Sampler {
public:
    StratifiedSampler(const int _blocks);
    ~StratifiedSampler();
    
    //clip input blocks to nearest n^2 (floor)
    void clipInputBlocks();
    
    //initialize stratified sampler
    void initializeSampler();
    
    //generate 2-D random number array
    void genRandNumArr2D();
    
    //test if there are random number in array that is not used
    bool noNumRemains();
    
    //get 1-D random number in array
    virtual double get1D();
    
    //get 2-D random number in array
    virtual Point2f get2D();
    
private:
    int blocks;
    int blocksPerAxis;
    Point2f* randNumArr;
    int currentNumberIndex;
};

class RandomSampler : public Sampler{
public:
    RandomSampler();
    ~RandomSampler();
    
    virtual void initializeSampler();
    virtual double get1D();
    virtual Point2f get2D();
};

#endif /* Sampler_h */
