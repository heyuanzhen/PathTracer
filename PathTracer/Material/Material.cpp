//
//  Material.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/8.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Material.h"
#include <iostream>

////MAterial
Material::Material(materialType mt) : mType(mt) {
    M.setZero();
    bsdfCount = 0;
}

Material::~Material() {}

void Material::getRotateMartix(const Vector3f nW){
    Vector3f v = nW.cross(nG);
    float s = v.norm();
    float c = nW.dot(nG);
    Matrix3f vx;
    vx << 0.0, -v.z(), v.y(),
          v.z(), 0.0, -v.x(),
          -v.y(), v.x(), 0.0;
    Matrix3f I = Eigen::Matrix3f::Identity();
    M = I + vx + vx * vx * (1.0 - c) / (s * s);
}

Vector3f Material::rotateNormalToLocal(const Vector3f vW) const{
    if (M.isZero()) {
        std::cout<<"M is a zero matrix !"<<std::endl;
        exit(0);
    }
    return M * vW;
}

Vector3f Material::getGeometryNormal() const {
    return nG;
}

Matrix3f Material::getM() const {
    return M;
}

void Material::addBSDF(BxDF *bsd) {};

Spectrum3f Material::eval(const Vector3f woW, const Vector3f wiW) const {
    Vector3f wo = rotateNormalToLocal(woW);
    Vector3f wi = rotateNormalToLocal(wiW);
    if (!isSameHemisphere(wo, wi)) {
        std::cout<<"Back ray !"<<std::endl;
        return Spectrum3f(0.0, 0.0, 0.0);
    }
    Spectrum3f f(1.0, 1.0, 1.0);
    if (!bsdfCount) {
        std::cout<<"Empty material !"<<std::endl;
        return Spectrum3f(0.0, 0.0, 0.0);
    }
    
    for (int i = 0; i < bsdfCount; i++) {
        f += bsdfs[i]->eval(wo, wi);
    }
    return f;
}

////Phong










