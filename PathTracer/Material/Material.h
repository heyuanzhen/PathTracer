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
    
    Material(materialType mt, BSDF* bs);
    virtual ~Material();
    
    void calcRotateMartix(const Vector3d nW);
    Vector3d rotateNormalToLocal(const Vector3d vW) const;
    Vector3d getGeometryNormal() const;
    Matrix3d getM() const;
    virtual void eval(const Vector3d wo, const Vector3d wi,
                      Vector3d& woL, Vector3d& wiL,
                      Spectrum3d& f, double& pdf) const;
protected:
    materialType mType;
    Matrix3d M; //M is a matrix that transform a normal to (0, 0, 1)
    const Vector3d nG = Vector3d(0.0, 1.0, 0.0); //geometry normal in local cordinate
    BSDF* bsdf;
};

//class BlinnPhong : public Material{
//public:
//    BlinnPhong(double _ka, double _kd, double _ks);
//    ~BlinnPhong();
//    
//    virtual void addBSDF(BxDF* bsdf);
//private:
//    double ka, kd, ks;
//};

#endif /* Material_h */
