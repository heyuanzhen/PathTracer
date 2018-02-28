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


class Renderer {
protected:
    int xres, yres;
    int sampleCount;
    int maxDepth;
    float** pixels;
    Scene* scene;
    Camera* camera;
    Sampler* sampler;
    Ray* rays;
public:
    Renderer(int* reso, int spc, int mD, Scene* scene, Camera* camera, Sampler* sampler, Ray* rays);
    ~Renderer();
    void printPixels() const;
    void showImage() const;
    void startRendering();
    void test();
};

#endif /* Renderer_h */
