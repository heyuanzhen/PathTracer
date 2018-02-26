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

class Renderer {
    int xres, yres;
    int sampleCount;
    float** pixels;
    Scene* scene;
    Camera* camera;
    Sampler* sampler;
    Ray* rays;
    bool isInit;
public:
    Renderer(int* reso, int spc);
    ~Renderer();
    void printPixels() const;
    void showImage() const;
    void initRenderer(Scene* scene, Camera* camera, Sampler* sampler);
    void startRendering();
    void test();
};

#endif /* Renderer_h */
