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

double BSDF::getWeightSum() const {
    return weightSum;
}


////Blinn-Phong Reflection Model
PhongBSDF::PhongBSDF() {}

PhongBSDF::PhongBSDF(Spectrum3d _ka, Spectrum3d _kd, Spectrum3d _ks):
                ka(_ka), kd(_kd), ks(_ks), T(Spectrum3d(0.0, 0.0, 0.0)), eta(1.0){
    buildBSDF();
    weightSum = 0.0;
    for (int i = 0; i < getBxDFCount(); i++) {
        weightSum += bxdfs[i]->getWeight();
    }
}

PhongBSDF::PhongBSDF(Spectrum3d _ka, Spectrum3d _kd, Spectrum3d _ks,
                               Spectrum3d _T, double e):
ka(_ka), kd(_kd), ks(_ks), T(_T), eta(e){
    buildBSDF();
    
}


PhongBSDF::~PhongBSDF() {
    if (isBuilt) {
        for (int i = 0; i < BxDFCount; i++) {
            delete bxdfs[i];
        }
    }
}

void PhongBSDF::buildBSDF() {
    if (isBuilt) {
        return;
    }
    BxDF* ambientReflection = new ConstantReflection(ka);
    BxDF* diffuseReflection = new LambertianDiffuseReflection(kd);
    BxDF* specularReflection = new SpecularReflection(ks);
    BxDF* specularTransmission = new SpecularTransmission(T, 1.0, eta);
    addBxDF(ambientReflection);
    addBxDF(diffuseReflection);
    addBxDF(specularReflection);
    addBxDF(specularTransmission);
    setBuilt(true);
    weightSum = 0.0;
    for (int i = 0; i < getBxDFCount(); i++) {
        weightSum += bxdfs[i]->getWeight();
    }
}

void PhongBSDF::buildBSDF(Spectrum3d _ka, Spectrum3d _kd, Spectrum3d _ks) {
    ka = _ka;
    kd = _kd;
    ks = _ks;
    T = Spectrum3d(0.0, 0.0, 0.0);
    eta = 0.0;
    buildBSDF();
}

////Blinn Phong BSDF
BlinnPhongBSDF::BlinnPhongBSDF(Spectrum3d _ka, Spectrum3d _kd, Spectrum3d _ks, Spectrum3d _T, double sh, double e) :
ka(_ka), kd(_kd), ks(_ks), T(_T), shininess(sh), eta(e) {
    buildBSDF();
    weightSum = 0.0;
    for (int i = 0; i < getBxDFCount(); i++) {
        weightSum += bxdfs[i]->getWeight();
    }
}

BlinnPhongBSDF::BlinnPhongBSDF(Spectrum3d _ka, Spectrum3d _kd, Spectrum3d _ks, double sh) :
ka(_ka), kd(_kd), ks(_ks), shininess(sh), T(Spectrum3d(0.0, 0.0, 0.0)), eta(1.0){
    buildBSDF();
    weightSum = 0.0;
    for (int i = 0; i < getBxDFCount(); i++) {
        weightSum += bxdfs[i]->getWeight();
    }
}

BlinnPhongBSDF::~BlinnPhongBSDF() {
    if (isBuilt) {
        for (int i = 0; i < BxDFCount; i++) {
            delete bxdfs[i];
        }
    }
}

void BlinnPhongBSDF::buildBSDF() {
    if (isBuilt) {
        return;
    }
    BxDF* ambientReflection = new ConstantReflection(ka);
    BxDF* diffuseReflection = new LambertianDiffuseReflection(kd);
    BxDF* specularReflection = new BlinnPhongSpecularReflection(ks, shininess);
    BxDF* specularTransmission = new SpecularTransmission(T, 1.0, eta);
    addBxDF(ambientReflection);
    addBxDF(diffuseReflection);
    addBxDF(specularReflection);
    addBxDF(specularTransmission);
    setBuilt(true);
}


//// Fresnel BSDF
FresnelBSDF::FresnelBSDF(Spectrum3d _kr, Spectrum3d _kt, double _eA, double _eB) :
                kr(_kr), kt(_kt), etaA(_eA), etaB(_eB){
    buildBSDF();
    weightSum = 0.0;
    for (int i = 0; i < getBxDFCount(); i++) {
        weightSum += bxdfs[i]->getWeight();
    }
}

FresnelBSDF::~FresnelBSDF() {
    if (isBuilt) {
        for (int i = 0; i < BxDFCount; i++) {
            delete bxdfs[i];
        }
    }
}

void FresnelBSDF::buildBSDF() {
    if (isBuilt) {
        return;
    }
    BxDF* fresnelSpecular = new FresnelSpecular(kr, kt, etaA, etaB);
    addBxDF(fresnelSpecular);
    setBuilt(true);
}







