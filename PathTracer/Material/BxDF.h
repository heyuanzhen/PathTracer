//
//  BxDF.h
//  PathTracer
//
//  Created by HYZ on 2018/2/4.
//  Copyright © 2018年 HYZ. All rights reserved.
//

//This file contains BxDF classes and inline functions

#ifndef BxDF_h
#define BxDF_h

#include "typeAlias.h"
#include "globalConstants.h"
#include <iostream>

//pbrt: page 510
inline double cosTheta(Point3d w){
    return w[2];
}

inline double cos2theta(Point3d w){
    return w[2] * w[2];
}

inline double absCosTheta(Point3d w){
    return abs(w[2]);
}

inline double sin2Theta(Point3d w){
    return 1.0 - cos2theta(w) > 0.0 ? 1.0 - cos2theta(w) : 0.0;
}

inline double sinTheta(Point3d w){
    return sqrt(sin2Theta(w));
}

inline double cosPhi(Point3d w){
    return (sinTheta(w) - 0.0 > eps) ? 1.0 : w[0] / sinTheta(w);
}

inline double sinPhi(Point3d w){
    return (sinTheta(w) - 0.0 > eps) ? 1.0 : w[1] / sinTheta(w);
}

inline double sin2Phi(Point3d w){
    return sinPhi(w) * sinPhi(w);
}

inline double cos2Phi(Point3d w){
    return cosPhi(w) * cosPhi(w);
}

inline double clamp(double val, double low, double up){
    val = val > low ? val : low;
    val = val < up ? val : up;
    return val;
}

inline bool isSameHemisphere(const Vector3d wo, const Vector3d wi){
    return wo[2] * wi[2] > 0 ? true : false;
}

inline bool refract(const Vector3d &wi, const Vector3d n, double eta, Vector3d& wt) {
    double cosThetaI = n.dot(wi);
    double sin2ThetaI = std::max(0.0, 1.0 - cosThetaI * cosThetaI);
    double sin2ThetaT = eta * eta * sin2ThetaI;
    
//    if (eta > 1.0 && sin2ThetaI > 0.308) {
//        std::cout<<"sin2(thetaI) = "<<sin2ThetaI<<", 1/n2 = "<<1.0 / (eta * eta)<<std::endl;
//    }
    if (sin2ThetaI >= (1.0 / (eta * eta))){
        return false;
    }
    double cosThetaT = sqrt(1.0 - sin2ThetaT);
    wt = eta * (-wi) + (eta * cosThetaI - cosThetaT) * n;
    wt = wt.normalized();
//    if(eta > 1.0) {
//        std::cout<<"eta = "<<eta<<", wi = "<<wi.transpose()<<", cosThetaI = "<<cosThetaI<<", cosThetaT = "<<cosThetaT<<", wt = "<<wt.transpose()<<std::endl;
//    }
    return true;
}


class BxDF {
public:
    enum BxDFType{
        SPECULAR    = 1,
        DIFFUSE     = 2,
        CONSTANT    = 3,
        BFSPECULAR  = 4,
        TRANSMISSION = 5,
        FRESNELSPECULAR = 6
    };
    
    BxDF(BxDFType tp);
    virtual ~BxDF();
    
    int getType() const;
    
    //calculate BxDF function value (pure virtual)
    virtual Spectrum3d eval(const Vector3d wo, const Vector3d wi) const = 0;
    
    //default sample method to sample wi
    virtual Spectrum3d sampleWiAndEval(const Vector3d wo, Vector3d& wi, Point2d u, double& pdf) const;
    
    //default pdf calculate method
    virtual double calcPDF(const Vector3d wo, const Vector3d wi) const;
    
    //every BxDF needs to give a weight when sampled
    virtual double getWeight() const = 0;
    
private:
    const BxDFType type;
protected:
    //geometry normal
    Spectrum3d n;
};

////calcPDF() should be redefined according to the type of reflection model!!!
class SpecularReflection : public BxDF {
    Spectrum3d ks;
public:
    SpecularReflection(Spectrum3d _ks);
    ~SpecularReflection();
    
    double shininess;
    virtual Spectrum3d eval(const Vector3d wo, const Vector3d wi) const;
    virtual Spectrum3d sampleWiAndEval(const Vector3d wo, Vector3d& wi, Point2d u, double& pdf) const;
    virtual double calcPDF(const Vector3d wo, const Vector3d wi) const;
    virtual double getWeight() const;
};

class SpecularTransmission : public BxDF {
    const Spectrum3d T;
    const double etaA, etaB;
public:
    SpecularTransmission(Spectrum3d _T, double eA, double eB);
    ~SpecularTransmission();
    
    virtual Spectrum3d eval(const Vector3d wo, const Vector3d wi) const;
    virtual Spectrum3d sampleWiAndEval(const Vector3d wo, Vector3d& wi, Point2d u, double& pdf) const;
    virtual double calcPDF(const Vector3d wo, const Vector3d wi) const;
    virtual double getWeight() const;
};

class LambertianDiffuseReflection : public BxDF {
    Spectrum3d kd;
public:
    LambertianDiffuseReflection(Spectrum3d _kd);
    ~LambertianDiffuseReflection();
    
    virtual Spectrum3d eval(const Vector3d wo, const Vector3d wi) const;
    virtual double getWeight() const;
};

class ConstantReflection : public BxDF {
    Spectrum3d k;
public:
    ConstantReflection(Spectrum3d _k);
    ~ConstantReflection();
    
    virtual Spectrum3d eval(const Vector3d wo, const Vector3d wi) const;
    virtual double getWeight() const;
};

class FresnelSpecular : public BxDF {
    Spectrum3d kr, kt;
    double etaA, etaB;
    
public:
    FresnelSpecular(Spectrum3d _kr, Spectrum3d _kt, double eA, double eB);
    ~FresnelSpecular();
    
    virtual Spectrum3d eval(const Vector3d wo, const Vector3d wi) const;
    virtual Spectrum3d sampleWiAndEval(const Vector3d wo, Vector3d& wi, Point2d u, double& pdf) const;
    virtual double calcPDF(const Vector3d wo, const Vector3d wi) const;
    virtual double getWeight() const;
    double FrDielectric(double cosThetaI, double etaI, double etaT) const;
};

class BlinnPhongSpecularReflection : public BxDF{
    double ks;
    double shininess;
public:
    BlinnPhongSpecularReflection(double _ks, double _sh);
    ~BlinnPhongSpecularReflection();
    virtual Spectrum3d eval(const Vector3d wo, const Vector3d wi) const;
    virtual Spectrum3d sampleWiAndEval(const Vector3d wo, Vector3d& wi, Point2d u, double& pdf) const;
    virtual double calcPDF(const Vector3d wo, const Vector3d wi) const;
    virtual double getWeight() const;
};

//class BlinnPhong : public BSDF{
//private:
//    // I = ka * Ia + Ii * [kd * (N * L) + ks * (H * N)^shininess]
//    double ka, kd, ks, shininess;
//public:
//    BlinnPhong(double _ka, double _kd, double _ks, double _shininess);
//    ~BlinnPhong();
//
//    virtual Spectrum3d eval(const Vector3d wo, const Vector3d wi) const;
//};

#endif /* BxDF_h */
