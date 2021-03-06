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
    Camera(int spC, Sampler* sp, Ray* r);
    virtual ~Camera();
    
    virtual Ray* generateRays(int rowi, int coli) = 0;
};



class PerspectiveCamera : public Camera{
    int xres, yres;
    double f, fov; // fov is the view angle along x axis
    double xLength, yLength; // image-plane's size in world cordinate
    Point3d target;
    Point3d origin;
    Vector3d up;
    bool isWToICalc;
    bool isIToWCalc;
    
    Matrix4d m_worldToCam;
    MatrixXd m_camToImgPlane;
    MatrixXd m_imgPlaneToImage;
    
    Matrix3d m_imageToImgPlane;
    Matrix3d m_imgPlaneToCam;
    Matrix4d m_camToWorld;
public:
    //reso[2] should be rows * cols
    PerspectiveCamera(double* lookAt, int* reso, double fovf, int spC, Sampler* sp, Ray* r);
    ~PerspectiveCamera();
    
    void calcWToIMatrices();
    void calcIToWMatrices();
    Point2i worldToImg(Point3d p);
    Point3d imgToWorld(Point2d p);
    void calcRayParas(Point3d pos, Ray* ray);
    virtual Ray* generateRays(int rowi, int coli);
};

#endif /* Camera_h */
