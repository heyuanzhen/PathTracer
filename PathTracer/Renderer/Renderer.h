//
//  Renderer.h
//  PathTracer
//
//  Created by HYZ on 2018/2/26.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#ifndef Renderer_h
#define Renderer_h

#include "Scene.h"
#include "Camera.h"
#include "Sampler.h"
#include "PathIntegrator.h"


inline double clamp01(double value) {
    double newValue = value < 0.0 ? 0.0 : value;
    return newValue = newValue > 1.0 ? 1.0 : newValue;
}

//inline Spectrum3d clampSpec(Spectrum3d spec) {
//    double maxV = spec[0] > spec[1] ? spec[0] : spec[1];
//    maxV = maxV > spec[2] ? maxV : spec[2];
//    return spec / maxV;
//}

inline double gammaCorrect(double value) {
    return pow(value, gammaCoefficient);
}

inline int toRBGUint8(double value) {
    return (int)(clamp01(gammaCorrect(value)) * 255.0 + 0.5);
}


class Renderer {
protected:
    int xres, yres;
    int sampleCount;
    int maxDepth;
    double** pixels;
//    int** pixels_uint8;
    Scene* scene;
    Camera* camera;
    Sampler* pixelSampler;
    Sampler* normalSampler;
    Ray* rays;
public:
    Renderer(int* reso, int spc, int mD, Scene* sc, Sampler* psp,
             Sampler* nsp, double* lookAt, double fov);
    ~Renderer();
    void printPixels() const;
    void modifyPixels();
//    void showImage() const;
    void showImage_uint8() const;
    void writePPM() const;
    void startRendering();
    void test();
};

#endif /* Renderer_h */
