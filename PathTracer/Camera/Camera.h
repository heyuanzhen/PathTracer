//
//  Camera.h
//  PathTracer
//
//  Created by HYZ on 2018/2/19.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

#include "typeAlias.h"
#include "Sampler.h"
#include "Ray.h"

class Camera {
protected:
    const int sampleCount;
    Sampler* sampler;
    Ray* rays;
public:
    Camera(int spC, Sampler* sp);
    ~Camera();
    
    virtual void generateRays() = 0;
};



class PerspectiveCamera : public Camera{
    int xres, yres;
    float f, fov; // fov is the view angle along x axis
    float xLength, yLength; // image-plane's size in world cordinate
    Point3f target;
    Point3f origin;
    Vector3f up;
    bool isWToICalc;
    bool isIToWCalc;
    
    Matrix4f m_worldToCam;
    MatrixXf m_camToImgPlane;
    MatrixXf m_imgPlaneToImage;
    
    Matrix3f m_imageToImgPlane;
    Matrix3f m_imgPlaneToCam;
    Matrix4f m_camToWorld;
public:
    //reso[2] should be rows * cols
    PerspectiveCamera(float* lookAt, int* reso, float fovf, int spC, Sampler* sp);
    ~PerspectiveCamera();
    
    void calcWToIMatrices();
    void calcIToWMatrices();
    Point2i worldToImg(Point3f p);
    Point3f imgToWorld(Point2f p);
    void calcRayParas(Point3f pos, Ray* ray);
    virtual void generateRays();
};

#endif /* Camera_h */
