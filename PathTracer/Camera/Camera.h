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

class Camera {
    
public:
    Camera();
    ~Camera();
};



class PerspectiveCamera : public Camera{
    int xres, yres;
    float f, fov; // fov is the view angle along x axis
    float xLength, yLength; // image-plane's size in world cordinate
    Point3f target;
    Point3f origin;
    Vector3f up;
    bool isCalc;
    
    Matrix4f m_worldToCam;
    MatrixXf m_camToImgPlane;
    MatrixXf m_imgPlaneToImage;
public:
    //reso[2] should be rows * cols
    PerspectiveCamera(float* lookAt, int* reso, float fovf);
    ~PerspectiveCamera();
    
    void calcCameraMatrices();
    Point2i transToImg(Point3f p);
};

#endif /* Camera_h */
