//
//  Camera.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/19.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Camera.h"
#include <iostream>

Camera::Camera() {}
Camera::~Camera() {}


PerspectiveCamera::PerspectiveCamera(float* lookAt, int* reso, float fovf){
    fov = fovf;
    xres = reso[1];
    yres = reso[0];
    target = Point3f(lookAt[0], lookAt[1], lookAt[2]);
    origin = Point3f(lookAt[3], lookAt[4], lookAt[5]);
    up = Vector3f(lookAt[6], lookAt[7], lookAt[8]);
    f = (target - origin).norm();
    float theta = fov * 0.5;
    xLength = 2.0 * f * tan(theta);
    float yox = yres * 1.0 / (xres * 1.0);
    yLength = xLength * yox;
    isCalc = false;
    
    m_worldToCam.setIdentity();
    m_camToImgPlane = MatrixXf::Identity(3, 4);
    m_imgPlaneToImage = MatrixXf::Identity(2, 3);
}

PerspectiveCamera::~PerspectiveCamera() {}

void PerspectiveCamera::calcCameraMatrices() {
    Vector3f VPN = target - origin;
    Vector3f n = VPN.normalized();
    Vector3f Vup = up.normalized();
    Vector3f u = n.cross(Vup).normalized();
    Vector3f v = u.cross(n).normalized();
    
    m_worldToCam(0, 0) = u(0);
    m_worldToCam(0, 1) = u(1);
    m_worldToCam(0, 2) = u(2);
    m_worldToCam(0, 3) = -origin.dot(u);
    m_worldToCam(1, 0) = v(0);
    m_worldToCam(1, 1) = v(1);
    m_worldToCam(1, 2) = v(2);
    m_worldToCam(1, 3) = -origin.dot(v);
    m_worldToCam(2, 0) = n(0);
    m_worldToCam(2, 1) = n(1);
    m_worldToCam(2, 2) = n(2);
    m_worldToCam(2, 3) = -origin.dot(n);
    
    m_camToImgPlane(0, 0) = f;
    m_camToImgPlane(1, 1) = f;
    
    float invDx = xres / xLength;
    float invDy = yres / yLength;
    int u0 = xres * 0.5;
    int v0 = yres * 0.5;
    m_imgPlaneToImage(0, 0) = invDx;
    m_imgPlaneToImage(0, 2) = u0;
    m_imgPlaneToImage(1, 1) = invDy;
    m_imgPlaneToImage(1, 2) = v0;
    
    isCalc = true;
}

Point2i PerspectiveCamera::transToImg(Point3f p){
    Point4f homoP(p(0), p(1), p(2), 1.0);
    if (!isCalc) {
        calcCameraMatrices();
    }
    Point4f posInCam = m_worldToCam * homoP;
    posInCam = posInCam / posInCam(2);
    Point2f posInPix = m_imgPlaneToImage * m_camToImgPlane *  posInCam;
    posInPix(1) = yres - posInPix(1);
    
    return Point2i(posInPix(1), posInPix(0)); //(row, col)
}






