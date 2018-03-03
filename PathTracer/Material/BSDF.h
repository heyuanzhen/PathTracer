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
public:
    BxDF* bxdfs[maxBxDFs];
    
    BSDF();
    virtual ~BSDF();
    bool isFull() const;
    void addBxDF(BxDF* bxdf);
    void setBuilt(bool ib);
    bool getBuilt() const;
    virtual void buildBSDF() = 0;
};

class BlinnPhongBSDF : public BSDF {
public:
    BlinnPhongBSDF(float _ka, float _kd, float _ks, float _sh);
    ~BlinnPhongBSDF();
    
    virtual void buildBSDF();
private:
    float ka, kd, ks;
    float shininess;
};


#endif /* BSDF_h */
