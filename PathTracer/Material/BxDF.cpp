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
    n = Spectrum3f(0.0, 0.0, 1.0);
}
BxDF::~BxDF(){}

float BxDF::calcPDF(const Vector3f wo, const Vector3f wi) const {
    return isSameHemisphere(wo, wi) ? absCosTheta(wi) * InvPi : 0.0;
}

Spectrum3f BxDF::sampleWiAndEval(const Vector3f wo, Vector3f &wi, Point2f u, float &pdf) const {
    wi = uniformSampleHemisphere(u);
    wi.z() = wi.z() < 0.0 ? wi.z() *= -1.0 : wi.z();
    pdf = calcPDF(wo, wi);
    return eval(wo, wi);
}


////Specular Reflection
SpecularReflection::SpecularReflection(float _ks, float _shiniess) :
ks(_ks), shininess(_shiniess), BxDF(SPECULAR){}

SpecularReflection::~SpecularReflection(){}

Spectrum3f SpecularReflection::eval(const Vector3f wo, const Vector3f wi) const {
    Vector3f H = wo + wi / (wo + wi).norm();
    return Spectrum3f(1.0, 1.0, 1.0) * ks * pow(H.dot(n), shininess);
}


////Lambertian Diffuse Reflection
LambertianDiffuseReflection::LambertianDiffuseReflection(float _kd) : kd(_kd), BxDF(DIFFUSE) {}

LambertianDiffuseReflection::~LambertianDiffuseReflection() {}

Spectrum3f LambertianDiffuseReflection::eval(const Vector3f wo, const Vector3f wi) const {
    return Spectrum3f(1.0, 1.0, 1.0) * kd * n.dot(wi);
}

////Constant Reflection
ConstantReflection::ConstantReflection(float _k) : k(_k), BxDF(CONSTANT) {}

ConstantReflection::~ConstantReflection() {}

Spectrum3f ConstantReflection::eval(const Vector3f wo, const Vector3f wi) const {
    return Spectrum3f(1.0, 1.0, 1.0) * k;
}

////Blinn-Phong Specular Reflection
BlinnPhongSpecularReflection::BlinnPhongSpecularReflection(float _ks, float _sh) :
                                ks(_ks), shininess(_sh), BxDF(BFSPECULAR) {}

BlinnPhongSpecularReflection::~BlinnPhongSpecularReflection() {}

Spectrum3f BlinnPhongSpecularReflection::eval(const Vector3f wo, const Vector3f wi) const{
    Vector3f H = wo + wi / (wo + wi).norm();
    return Spectrum3f(1.0, 1.0, 1.0) * ks * pow(H.dot(n), shininess);
}

////Blinn-Phong
//BlinnPhong::BlinnPhong(float _ka, float _kd, float _ks, float _shininess) :
//ka(_ka), kd(_kd), ks(_ks), shininess(_shininess), BSDF(PHONG){};
//
//BlinnPhong::~BlinnPhong(){}
//
//Spectrum3f BlinnPhong::eval(const Vector3f wo, const Vector3f wi) const{
//    Spectrum3f Ii = Spectrum3f(1.0, 1.0, 1.0);
//    if (wi == -wo) {
//        std::cout<<"conflict light direction !"<<std::endl;
//        return Spectrum3f(0.0, 0.0, 0.0);
//    }
//    Vector3f H = wo + wi / (wo + wi).norm();
//    Ii *= (kd * n.dot(wi) + ks * pow(H.dot(n), shininess));
//    return Ii;
//}

