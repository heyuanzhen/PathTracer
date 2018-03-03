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

//pbrt: page 510
inline float cosTheta(Point3f w){
    return w[2];
}

inline float cos2theta(Point3f w){
    return w[2] * w[2];
}

inline float absCosTheta(Point3f w){
    return abs(w[2]);
}

inline float sin2Theta(Point3f w){
    return 1.0 - cos2theta(w) > 0.0 ? 1.0 - cos2theta(w) : 0.0;
}

inline float sinTheta(Point3f w){
    return sqrt(sin2Theta(w));
}

inline float cosPhi(Point3f w){
    return (sinTheta(w) - 0.0 > eps) ? 1.0 : w[0] / sinTheta(w);
}

inline float sinPhi(Point3f w){
    return (sinTheta(w) - 0.0 > eps) ? 1.0 : w[1] / sinTheta(w);
}

inline float sin2Phi(Point3f w){
    return sinPhi(w) * sinPhi(w);
}

inline float cos2Phi(Point3f w){
    return cosPhi(w) * cosPhi(w);
}

inline float clamp(float val, float low, float up){
    val = val > low ? val : low;
    val = val < up ? val : up;
    return val;
}

inline bool isSameHemisphere(const Vector3f wo, const Vector3f wi){
    return wo[2] * wi[2] > 0 ? true : false;
}


class BxDF {
public:
    enum BxDFType{
        SPECULAR    = 1,
        DIFFUSE     = 2,
        CONSTANT    = 3,
        BFSPECULAR  = 4
    };
    
    BxDF(BxDFType tp);
    virtual ~BxDF();
    
    //calculate BxDF function value (pure virtual)
    virtual Spectrum3f eval(const Vector3f wo, const Vector3f wi) const = 0;
    
    //default sample method to sample wi
    virtual Spectrum3f sampleWiAndEval(const Vector3f wo, Vector3f& wi, Point2f u, float& pdf) const;
    
    //default pdf calculate method
    virtual float calcPDF(const Vector3f wo, const Vector3f wi) const;
private:
    const BxDFType type;
protected:
    //geometry normal
    Spectrum3f n;
};

////calcPDF() should be redefined according to the type of reflection model!!!
class SpecularReflection : public BxDF {
    float ks;
public:
    SpecularReflection(float _ks, float _shiniess);
    ~SpecularReflection();
    
    float shininess;
    virtual Spectrum3f eval(const Vector3f wo, const Vector3f wi) const;
};

class LambertianDiffuseReflection : public BxDF {
    float kd;
public:
    LambertianDiffuseReflection(float _kd);
    ~LambertianDiffuseReflection();
    
    virtual Spectrum3f eval(const Vector3f wo, const Vector3f wi) const;
};

class ConstantReflection : public BxDF {
    float k;
public:
    ConstantReflection(float _k);
    ~ConstantReflection();
    
    virtual Spectrum3f eval(const Vector3f wo, const Vector3f wi) const;
};

class BlinnPhongSpecularReflection : public BxDF{
    float ks;
    float shininess;
public:
    BlinnPhongSpecularReflection(float _ks, float _sh);
    ~BlinnPhongSpecularReflection();
    virtual Spectrum3f eval(const Vector3f wo, const Vector3f wi) const;
};

//class BlinnPhong : public BSDF{
//private:
//    // I = ka * Ia + Ii * [kd * (N * L) + ks * (H * N)^shininess]
//    float ka, kd, ks, shininess;
//public:
//    BlinnPhong(float _ka, float _kd, float _ks, float _shininess);
//    ~BlinnPhong();
//
//    virtual Spectrum3f eval(const Vector3f wo, const Vector3f wi) const;
//};

#endif /* BxDF_h */
