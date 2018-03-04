//
//  Material.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/8.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Material.h"
#include <iostream>

////Material
Material::Material(materialType mt, BSDF* bs) : mType(mt), bsdf(bs) {
    M.setZero();
}

Material::~Material() {}

void Material::calcRotateMartix(const Vector3d nW){
    Vector3d v = nW.cross(nG);
    double s = v.norm();
    double c = nW.dot(nG);
    Matrix3d vx;
    vx << 0.0, -v.z(), v.y(),
          v.z(), 0.0, -v.x(),
          -v.y(), v.x(), 0.0;
    Matrix3d I = Eigen::Matrix3d::Identity();
    M = I + vx + vx * vx * (1.0 - c) / (s * s);
}

Vector3d Material::rotateNormalToLocal(const Vector3d vW) const{
    if (M.isZero()) {
        std::cout<<"M is a zero matrix !"<<std::endl;
        exit(0);
    }
    return M * vW;
}

Vector3d Material::getGeometryNormal() const {
    return nG;
}

Matrix3d Material::getM() const {
    return M;
}


void Material::eval(const Vector3d woW, const Vector3d wiW,
                    Vector3d& woL, Vector3d& wiL,
                    Spectrum3d& f, double& pdf) const {
    if (!bsdf->getBuilt()) {
        std::cout<<"The BSDF is not build !"<<std::endl;
        return;
    }
    woL = rotateNormalToLocal(woW);
    wiL = rotateNormalToLocal(wiW);
//    std::cout<<"wo = "<<wo.transpose()<< ", wi = "<<wi.transpose()<<std::endl;
    if (!isSameHemisphere(woL, wiL)) {
        std::cout<<"Back ray !"<<" woW = "<<woL.transpose()<< ", wiW = "<<wiL.transpose()<<std::endl;
        f = Spectrum3d(0.0, 0.0, 0.0);
        pdf = 0.0;
        return;
    }
    
    if (!bsdf->getBxDFCount()) {
        std::cout<<"Empty material !"<<std::endl;
        f = Spectrum3d(0.0, 0.0, 0.0);
        pdf = 0.0;
        return;
    }
    f = Spectrum3d(0.0, 0.0, 0.0);
    pdf = 0.0;
    for (int i = 0; i < bsdf->getBxDFCount(); i++) {
        f += bsdf->bxdfs[i]->eval(woL, wiL);
        pdf += bsdf->bxdfs[i]->calcPDF(woL, wiL);
    }
//    std::cout<<"f = "<<f.transpose()<< ", pdf = "<<pdf<<std::endl;
}

////BlinnPhong










