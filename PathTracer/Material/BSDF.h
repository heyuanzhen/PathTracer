//
//  BSDF.h
//  PathTracer
//
//  Created by HYZ on 2018/2/9.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#ifndef BSDF_h
#define BSDF_h

#include "BxDF.h"


class BSDF {
protected:
    static const int maxBxDFs = 5;
    int BxDFCount;
    bool isBuilt;
    double weightSum;
public:
    BxDF* bxdfs[maxBxDFs];
    
    BSDF();
    virtual ~BSDF();
    bool isFull() const;
    void addBxDF(BxDF* bxdf);
    void setBuilt(bool ib);
    bool getBuilt() const;
    int getBxDFCount() const;
    double getWeightSum() const;
    virtual void buildBSDF() = 0;
};

class PhongBSDF : public BSDF {
public:
    PhongBSDF();
    PhongBSDF(Spectrum3d _ka, Spectrum3d _kd, Spectrum3d _ks);
    PhongBSDF(Spectrum3d _ka, Spectrum3d _kd, Spectrum3d _ks, Spectrum3d _T, double e);
    ~PhongBSDF();
    
    virtual void buildBSDF();
    void buildBSDF(Spectrum3d _ka, Spectrum3d _kd, Spectrum3d _ks);
private:
    Spectrum3d ka, kd, ks, T;
    double eta;
};

class BlinnPhongBSDF : public BSDF {
private:
    Spectrum3d ka, kd, ks, T;
    double shininess, eta;
public:
    BlinnPhongBSDF(Spectrum3d _ka, Spectrum3d _kd, Spectrum3d _ks, double sh);
    BlinnPhongBSDF(Spectrum3d _ka, Spectrum3d _kd, Spectrum3d _ks, Spectrum3d _T, double sh, double e);
    ~BlinnPhongBSDF();
    
    virtual void buildBSDF();
};


class FresnelBSDF : public BSDF {
private:
    Spectrum3d kr, kt;
    double etaA, etaB;
public:
    FresnelBSDF(Spectrum3d _kr, Spectrum3d _kt, double _eA, double _eB);
    ~FresnelBSDF();
    
    virtual void buildBSDF();
};

#endif /* BSDF_h */
