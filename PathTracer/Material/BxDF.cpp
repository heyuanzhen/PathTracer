//
//  BxDF.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/8.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "BxDF.h"
#include "sampling.h"
#include <iostream>

////BxDF
BxDF::BxDF(BxDFType tp) : type(tp) {
    n = Spectrum3d(0.0, 1.0, 0.0);
}
BxDF::~BxDF(){}

double BxDF::calcPDF(const Vector3d wo, const Vector3d wi) const {
    return isSameHemisphere(wo, wi) ? absCosTheta(wi) * InvPi : 0.0;
}

Spectrum3d BxDF::sampleWiAndEval(const Vector3d wo, Vector3d &wi, Point2f u, double &pdf) const {
    wi = uniformSampleHemisphere(u);
    wi.y() = wi.y() < 0.0 ? wi.y() *= -1.0 : wi.y();
    pdf = calcPDF(wo, wi);
    return eval(wo, wi);
}


////Specular Reflection
SpecularReflection::SpecularReflection(double _ks, double _shiniess) :
ks(_ks), shininess(_shiniess), BxDF(SPECULAR){}

SpecularReflection::~SpecularReflection(){}

Spectrum3d SpecularReflection::eval(const Vector3d wo, const Vector3d wi) const {
    Vector3d H = wo + wi / (wo + wi).norm();
    return Spectrum3d(1.0, 1.0, 1.0) * ks * pow(H.dot(n), shininess);
}


////Lambertian Diffuse Reflection
LambertianDiffuseReflection::LambertianDiffuseReflection(double _kd) : kd(_kd), BxDF(DIFFUSE) {}

LambertianDiffuseReflection::~LambertianDiffuseReflection() {}

Spectrum3d LambertianDiffuseReflection::eval(const Vector3d wo, const Vector3d wi) const {
    Spectrum3d f = Spectrum3d(1.0, 1.0, 1.0) * kd * n.dot(wi);
//    std::cout<<"wi = "<<wi.transpose()<< ", n = "<<n.transpose()<<", f = "<<f.transpose()<<std::endl;
    return Spectrum3d(1.0, 1.0, 1.0) * kd * n.dot(wi);
}

////Constant Reflection
ConstantReflection::ConstantReflection(double _k) : k(_k), BxDF(CONSTANT) {}

ConstantReflection::~ConstantReflection() {}

Spectrum3d ConstantReflection::eval(const Vector3d wo, const Vector3d wi) const {
    return Spectrum3d(1.0, 1.0, 1.0) * k;
}

////Blinn-Phong Specular Reflection
BlinnPhongSpecularReflection::BlinnPhongSpecularReflection(double _ks, double _sh) :
                                ks(_ks), shininess(_sh), BxDF(BFSPECULAR) {}

BlinnPhongSpecularReflection::~BlinnPhongSpecularReflection() {}

Spectrum3d BlinnPhongSpecularReflection::eval(const Vector3d wo, const Vector3d wi) const{
    Vector3d H = wo + wi / (wo + wi).norm();
    return Spectrum3d(1.0, 1.0, 1.0) * ks * pow(H.dot(n), shininess);
}

////Blinn-Phong
//BlinnPhong::BlinnPhong(double _ka, double _kd, double _ks, double _shininess) :
//ka(_ka), kd(_kd), ks(_ks), shininess(_shininess), BSDF(PHONG){};
//
//BlinnPhong::~BlinnPhong(){}
//
//Spectrum3d BlinnPhong::eval(const Vector3d wo, const Vector3d wi) const{
//    Spectrum3d Ii = Spectrum3d(1.0, 1.0, 1.0);
//    if (wi == -wo) {
//        std::cout<<"conflict light direction !"<<std::endl;
//        return Spectrum3d(0.0, 0.0, 0.0);
//    }
//    Vector3d H = wo + wi / (wo + wi).norm();
//    Ii *= (kd * n.dot(wi) + ks * pow(H.dot(n), shininess));
//    return Ii;
//}

