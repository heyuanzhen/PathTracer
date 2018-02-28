//
//  Camera.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/19.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Camera.h"
#include "Sampler.h"
#include <iostream>

Camera::Camera(int spC, Sampler* sp, Ray* r) : sampleCount(spC), sampler(sp) {
    rays = r;
}
Camera::~Camera() {}


PerspectiveCamera::PerspectiveCamera(float* lookAt, int* reso, float fovf, int spC, Sampler* sp, Ray* r)
                                    : Camera(spC, sp, r){
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
    isWToICalc = false;
    isIToWCalc = false;
    
    m_worldToCam.setIdentity();
    m_camToImgPlane = MatrixXf::Identity(3, 4);
    m_imgPlaneToImage = MatrixXf::Identity(2, 3);
    
    m_imageToImgPlane.setIdentity();
    m_imgPlaneToCam.setIdentity();
    m_camToWorld.setIdentity();
    
    Point3f zero(0.0, 0.0, 0.0);
}

PerspectiveCamera::~PerspectiveCamera() {}

void PerspectiveCamera::calcWToIMatrices() {
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
    float u0 = xres * 0.5;
    float v0 = yres * 0.5;
    m_imgPlaneToImage(0, 0) = invDx;
    m_imgPlaneToImage(0, 2) = u0;
    m_imgPlaneToImage(1, 1) = invDy;
    m_imgPlaneToImage(1, 2) = v0;
    
    isWToICalc = true;
}

void PerspectiveCamera::calcIToWMatrices() {
    float Dx = xLength / xres;
    float Dy = yLength / yres;
    float u0 = xres * 0.5;
    float v0 = yres * 0.5;
    m_imageToImgPlane(0, 0) = Dx;
    m_imageToImgPlane(0, 2) = -u0 * Dx;
    m_imageToImgPlane(1, 1) = Dy;
    m_imageToImgPlane(1, 2) = -v0 * Dy;
    
    float invF = 1.0 / f;
    m_imgPlaneToCam(0, 0) = invF;
    m_imgPlaneToCam(1, 1) = invF;
    
    Vector3f VPN = target - origin;
    Vector3f n = VPN.normalized();
    Vector3f Vup = up.normalized();
    Vector3f u = n.cross(Vup).normalized();
    Vector3f v = u.cross(n).normalized();
    
    float ux = u.x(), uy = u.y(), uz = u.z();
    float vx = v.x(), vy = v.y(), vz = v.z();
    float nx = n.x(), ny = n.y(), nz = n.z();
    float A = nx*uy*vz - nx*uz*vy - ny*ux*vz + ny*uz*vx + nz*ux*vy - nz*uy*vx;
    float oriDotU = origin.dot(u);
    float oriDotV = origin.dot(v);
    float oriDotN = origin.dot(n);
    m_camToWorld(0, 0) = -(ny*vz - nz*vy)/A;
    m_camToWorld(0, 1) = (ny*uz - nz*uy)/A;
    m_camToWorld(0, 2) = (uy*vz - uz*vy)/A;
    m_camToWorld(0, 3) = (ny*oriDotV*uz - nz*oriDotV*uy - ny*oriDotU*vz + nz*oriDotU*vy + oriDotN*uy*vz - oriDotN*uz*vy)/A;
    m_camToWorld(1, 0) = (nx*vz - nz*vx)/A;
    m_camToWorld(1, 1) = -(nx*uz - nz*ux)/A;
    m_camToWorld(1, 2) = -(ux*vz - uz*vx)/A;
    m_camToWorld(1, 3) = -(nx*oriDotV*uz - nz*oriDotV*ux - nx*oriDotU*vz + nz*oriDotU*vx + oriDotN*ux*vz - oriDotN*uz*vx)/A;
    m_camToWorld(2, 0) = -(nx*vy - ny*vx)/A;
    m_camToWorld(2, 1) = (nx*uy - ny*ux)/A;
    m_camToWorld(2, 2) = (ux*vy - uy*vx)/A;
    m_camToWorld(2, 3) = (nx*oriDotV*uy - ny*oriDotV*ux - nx*oriDotU*vy + ny*oriDotU*vx + oriDotN*ux*vy - oriDotN*uy*vx)/A;
    isIToWCalc = true;
}

Point2i PerspectiveCamera::worldToImg(Point3f p){
    Point4f homoP(p(0), p(1), p(2), 1.0);
    if (!isWToICalc) {
        calcWToIMatrices();
    }
    Point4f posInCam = m_worldToCam * homoP;
    posInCam = posInCam / posInCam(2);
    Point2f posInPix = m_imgPlaneToImage * m_camToImgPlane *  posInCam;
    posInPix(1) = yres - posInPix(1);
    
    return Point2i(posInPix(1), posInPix(0)); //(row, col)
}

Point3f PerspectiveCamera::imgToWorld(Point2f p) { //p:(row, col)
    if (!isIToWCalc) {
        calcIToWMatrices();
    }
    Point2f pos = Point2f(p(1), yres - p(0)); //pos:(x, y);
    Point3f homoP(pos(0), pos(1), 1.0);
    Point3f imgPlaneP = m_imageToImgPlane * homoP;
//    std::cout<<"imgPlaneP: "<<imgPlaneP.transpose()<<std::endl;
    Point3f camP = f * m_imgPlaneToCam * imgPlaneP;
//    std::cout<<"camp: "<<camP.transpose()<<std::endl;
    Point4f homoCamP = Point4f(camP(0), camP(1), camP(2), 1.0);
    Point4f homoWorldP = m_camToWorld * homoCamP;
    return Point3f(homoWorldP(0), homoWorldP(1), homoWorldP(2));
}

void PerspectiveCamera::calcRayParas(Point3f pos, Ray *ray) {
    Vector3f dd = (pos - origin).normalized();
    ray->setRay(origin, dd, 0.0);
}

void PerspectiveCamera::generateRays() {
//    #pragma omp parallel for schedule(dynamic)
    for (int rowi = 0; rowi < yres; rowi++) {
        for (int coli = 0; coli < xres; coli++) {
            for (int spi = 0; spi < sampleCount; spi++) {
                Point2f p(rowi, coli);
//                std::cout<<"p = "<<p.transpose()<<std::endl;
                p += sampler->get2D() - Point2f(0.5, 0.5);
                Point3f pW = imgToWorld(p);
//                std::cout<<"pW = "<<pW.transpose()<<std::endl;
                int offset = (rowi * xres + coli) * sampleCount + spi;
                calcRayParas(pW, &rays[offset]);
                if (!rays[offset].isInit()) {
                    std::cout<<pW.transpose()<<::std::endl;
                }
            }
        }
    }
//    for (int i = 0; i < yres; i++) {
//        for (int j = 0; j < xres; j++) {
//            std::cout<<"("<<i<<", "<<j<<"): "<<rays[i * xres + j].getDirection().transpose()<<std::endl;
//        }
//    }
}





