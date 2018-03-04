//
//  BSDF.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/9.
//  Copyright © 2018年 HYZ. All rights reserved.
//
#include "BSDF.h"
#include <iostream>

class BxDF;

////BSDF
BSDF::BSDF() : BxDFCount(0), isBuilt(false) {}
BSDF::~BSDF(){}
//
bool BSDF::isFull() const{
    return BxDFCount == maxBxDFs ? true : false;
}

void BSDF::addBxDF(BxDF *bxdf){
    if (isFull()) {
        std::cout<<"BxDFs array is full !"<<std::endl;
        return;
    }
    bxdfs[BxDFCount] = bxdf;
    BxDFCount++;
}

void BSDF::setBuilt(bool ib){
    isBuilt = ib;
}

bool BSDF::getBuilt() const {
    return isBuilt;
}

int BSDF::getBxDFCount() const {
    return BxDFCount;
}


////Blinn-Phong Reflection Model
BlinnPhongBSDF::BlinnPhongBSDF(double _ka, double _kd, double _ks, double _sh):
                ka(_ka), kd(_kd), ks(_ks), shininess(_sh){}

BlinnPhongBSDF::~BlinnPhongBSDF() {
    if (isBuilt) {
        for (int i = 0; i < BxDFCount; i++) {
            delete bxdfs[i];
        }
    }
}

void BlinnPhongBSDF::buildBSDF() {
    BxDF* ambientReflection = new ConstantReflection(ka);
    BxDF* diffuseReflection = new LambertianDiffuseReflection(kd);
    BxDF* blinnPhongSpecularReflection = new BlinnPhongSpecularReflection(ks, shininess);
    addBxDF(ambientReflection);
    addBxDF(diffuseReflection);
    addBxDF(blinnPhongSpecularReflection);
    setBuilt(true);
}
