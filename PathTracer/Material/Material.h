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
    
    Vector3d rotateNormalToLocal(const Vector3d vW, const Matrix3d M) const;
    Vector3d rotateNormalToWorld(const Vector3d vL, const Matrix3d invM) const;
    Vector3d getGeometryNormal() const;
    Matrix3d getM() const;
    int decideWhichBxDFToSample() const;
    virtual void eval(const Vector3d wo, const Vector3d wi,
                      Vector3d& woL, Vector3d& wiL, const Matrix3d M,
                      Spectrum3d& f, double& pdf) const;
    virtual Spectrum3d sampleBSDF(const Vector3d woW, Vector3d& wiW, const Matrix3d M,
                                  const Matrix3d invM, double& pdf, bool& specularBounces) const;
protected:
    materialType mType;
    const Vector3d nG = Vector3d(0.0, 0.0, 1.0); //geometry normal in local cordinate
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
