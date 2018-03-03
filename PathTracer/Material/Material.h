//
//  Material.h
//  PathTracer
//
//  Created by HYZ on 2018/2/8.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#ifndef Material_h
#define Material_h

#include "BSDF.h"

class Material {
public:
    enum materialType{
        PHONG = 1
    };
    
    Material(materialType mt);
    virtual ~Material();
    
    void getRotateMartix(const Vector3f nW);
    Vector3f rotateNormalToLocal(const Vector3f vW) const;
    Vector3f getGeometryNormal() const;
    Matrix3f getM() const;
    virtual void addBSDF(BxDF* bsdf);
    virtual Spectrum3f eval(const Vector3f wo, const Vector3f wi) const;
protected:
    materialType mType;
    Matrix3f M;    //M and nG should be modified !!!1
    const Vector3f nG = Vector3f(0.0, 0.0, 1.0); //geometry normal
    BSDF* bsdf;
    int bsdfCount;
};

class BlinnPhong : public Material{
public:
    BlinnPhong(float _ka, float _kd, float _ks);
    ~BlinnPhong();
    
    virtual void addBSDF(BxDF* bsdf);
private:
    float ka, kd, ks;
};

#endif /* Material_h */
