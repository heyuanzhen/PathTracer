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

inline Vector3d reflect(const Vector3d wo, const Vector3d n) {
    return Vector3d(-wo.x(), -wo.y(), wo.z());
}

////BxDF
BxDF::BxDF(BxDFType tp) : type(tp) {
    n = Spectrum3d(0.0, 0.0, 1.0);
}
BxDF::~BxDF(){}

int BxDF::getType() const {
    return type;
}

double BxDF::calcPDF(const Vector3d wo, const Vector3d wi) const {
    return isSameHemisphere(wo, wi) ? absCosTheta(wi) * InvPi : 0.0;
}

Spectrum3d BxDF::sampleWiAndEval(const Vector3d wo, Vector3d &wi, Point2d u, double &pdf) const {
    wi = uniformSampleHemisphere(u);
    wi.z() = wi.z() < 0.0 ? wi.z() *= -1.0 : wi.z();
    pdf = calcPDF(wo, wi);
    return eval(wo, wi);
}


////Specular Reflection
SpecularReflection::SpecularReflection(Spectrum3d _ks) :
ks(_ks), BxDF(SPECULAR){}

SpecularReflection::~SpecularReflection(){}

Spectrum3d SpecularReflection::eval(const Vector3d wo, const Vector3d wi) const {
    return Spectrum3d(0.0, 0.0, 0.0);
}

Spectrum3d SpecularReflection::sampleWiAndEval(const Vector3d wo, Vector3d &wi, Point2d u,
                                               double &pdf) const {
    pdf = 1.0;
    wi = reflect(wo, n);
//    return ks.cwiseProduct(Spectrum3d(1.0, 1.0, 1.0)) * cosTheta(wi) / absCosTheta(wi);
//    Spectrum3d r = ks.cwiseProduct(Spectrum3d(1.0, 1.0, 1.0)) / std::max(absCosTheta(wi), 0.00001);
//    if (r.norm() > sqrt(3.001)) {
//        std::cout<<"r = "<<r.transpose()<<std::endl;
//    }
    return ks / std::max(absCosTheta(wi), 0.00001);
}

double SpecularReflection::calcPDF(const Vector3d wo, const Vector3d wi) const {
    return 0.0;
}

double SpecularReflection::getWeight() const {
    return ks.norm();
}

////Specular Transmission
SpecularTransmission::SpecularTransmission(Spectrum3d _T, double eA, double eB) :
                                            T(_T), etaA(eA), etaB(eB), BxDF(TRANSMISSION) {}

SpecularTransmission::~SpecularTransmission() {}

Spectrum3d SpecularTransmission::eval(const Vector3d wo, const Vector3d wi) const {
    return Spectrum3d(0.0, 0.0, 0.0);
}

Spectrum3d SpecularTransmission::sampleWiAndEval(const Vector3d wo, Vector3d &wi, Point2d u,
                                                 double &pdf) const {
    bool entering = cosTheta(wo) > 0.0;
    double etaI = entering ? etaA : etaB;
    double etaT = entering ? etaB : etaA;
    Vector3d nn = n;
    nn = wo.dot(nn) > 0.0 ? nn : -nn;
    if (!refract(wo, nn, etaI / etaT, wi)){
        pdf = 1.0;
        wi = reflect(wo, n);
        std::cout<<"total internal reflection!"<<std::endl;
        return T / std::max(absCosTheta(wi), 0.00001);
    }
//    if (!entering) {
//        std::cout<<"wo = "<<wo.transpose()<<", wi = "<<wi.transpose()<<std::endl;
//    }
    pdf = 1.0;
    return T / std::max(absCosTheta(wi), 0.000001);
}

double SpecularTransmission::calcPDF(const Vector3d wo, const Vector3d wi) const {
    return 0.0;
}

double SpecularTransmission::getWeight() const {
    return T.norm();
}

////Lambertian Diffuse Reflection
LambertianDiffuseReflection::LambertianDiffuseReflection(Spectrum3d _kd) : kd(_kd), BxDF(DIFFUSE) {}

LambertianDiffuseReflection::~LambertianDiffuseReflection() {}

Spectrum3d LambertianDiffuseReflection::eval(const Vector3d wo, const Vector3d wi) const {
    return kd * InvPi;
}

double LambertianDiffuseReflection::getWeight() const {
    return kd.norm();
}

////Constant Reflection
ConstantReflection::ConstantReflection(Spectrum3d _k) : k(_k), BxDF(CONSTANT) {}

ConstantReflection::~ConstantReflection() {}

Spectrum3d ConstantReflection::eval(const Vector3d wo, const Vector3d wi) const {
    return k * InvPi; //????
}

double ConstantReflection::getWeight() const {
    return k.norm();
}


////Fresnel Specular Model
FresnelSpecular::FresnelSpecular(Spectrum3d _kr, Spectrum3d _kt, double eA, double eB) :
                                kr(_kr), kt(_kt), etaA(eA), etaB(eB), BxDF(FRESNELSPECULAR) {}

FresnelSpecular::~FresnelSpecular() {}

Spectrum3d FresnelSpecular::eval(const Vector3d wo, const Vector3d wi) const {
    return Spectrum3d(0.0, 0.0, 0.0);
}

double FresnelSpecular::calcPDF(const Vector3d wo, const Vector3d wi) const {
    return 0.0;
}

double FresnelSpecular::getWeight() const {
    return 1.0;
}

Spectrum3d FresnelSpecular::sampleWiAndEval(const Vector3d wo, Vector3d &wi,
                                            Point2d u, double &pdf) const {
    double Fr = FrDielectric(cosTheta(wo), etaA, etaB);
    if (u[0] < Fr) { //reflection
        wi = reflect(wo, n);
        pdf = Fr;
        return Fr * kr / absCosTheta(wi);
    }
    else { //transmission
        bool entering = cosTheta(wo) > 0;
        double etaI = entering ? etaA : etaB;
        double etaT = entering ? etaB : etaA;
        
        Vector3d nn = n;
        nn = wo.dot(nn) > 0.0 ? nn : -nn;
        if (!refract(wo, nn, etaI / etaT, wi)){
            pdf = 1.0;
            wi = reflect(wo, n);
            std::cout<<"total internal reflection!"<<std::endl;
            return kt / std::max(absCosTheta(wi), 0.00001);
        }
        
        Spectrum3d ft = kt * (1.0 - Fr); //?
        
        // Account for non-symmetry with transmission to different medium
        ft *= (etaI * etaI) / (etaT * etaT);
        
        pdf = 1.0 - Fr;
        return ft / absCosTheta(wi);
    }
}

double FresnelSpecular::FrDielectric(double cosThetaI, double etaI, double etaT) const {
    bool entering = cosThetaI > 0.0;
    if (!entering) {
        std::swap(etaI, etaT);
        cosThetaI = std::abs(cosThetaI);
    }
    double sinThetaI = sqrt(std::max(0.0, 1.0 - cosThetaI * cosThetaI));
    double sinThetaT = sinThetaI * etaI / etaT;
    if (sinThetaT >= 1) return 1.0; //total internal reflection
    double cosThetaT = sqrt(std::max(0.0, 1.0 - sinThetaT * sinThetaT));
    
    //calculate r// and r⊥
    double r1 = ((etaT * cosThetaI) - (etaI * cosThetaT)) / ((etaT * cosThetaI) + (etaI * cosThetaT));
    double r2 = ((etaI * cosThetaI) - (etaT * cosThetaT)) / ((etaI * cosThetaI) + (etaT * cosThetaT));
    
    return 0.5 * (r1 * r1 + r2 * r2);
}



//////Blinn-Phong Specular Reflection
//BlinnPhongSpecularReflection::BlinnPhongSpecularReflection(double _ks, double _sh) :
//                                ks(_ks), shininess(_sh), BxDF(BFSPECULAR) {}
//
//BlinnPhongSpecularReflection::~BlinnPhongSpecularReflection() {}
//
//Spectrum3d BlinnPhongSpecularReflection::eval(const Vector3d wo, const Vector3d wi) const{
//    Vector3d H = (wo + wi) / (wo + wi).norm();
//    return (shininess + 2.0) * Spectrum3d(1.0, 1.0, 1.0) * pow(H.dot(n), shininess) / (2.0 * Pi);
//}
//
//Spectrum3d BlinnPhongSpecularReflection::sampleWiAndEval(const Vector3d wo, Vector3d &wi, Point2d u, double &pdf) {
//    double theta = acos(pow(u[0], 1.0 / (shininess + 1.0)));
//    double phi = 2.0 * Pi * u[1];
//    Spectrum3d wh = Vector3d(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));
//    wi = reflect(wo, wh);
//    pdf = ((shininess + 2.0) / (shininess + 1.0)) * 4.0 * ()
//}



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

