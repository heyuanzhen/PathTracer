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

class BlinnPhongBSDF : public BSDF {
public:
    BlinnPhongBSDF(Spectrum3d _ka, Spectrum3d _kd, Spectrum3d _ks);
    BlinnPhongBSDF(Spectrum3d _ka, Spectrum3d _kd, Spectrum3d _ks, Spectrum3d _T, double e);
    ~BlinnPhongBSDF();
    
    virtual void buildBSDF();
private:
    Spectrum3d ka, kd, ks, T;
    double eta;
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
