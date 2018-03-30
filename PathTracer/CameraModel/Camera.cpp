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


PerspectiveCamera::PerspectiveCamera(double* lookAt, int* reso, double fovf, int spC, Sampler* sp, Ray* r)
                                    : Camera(spC, sp, r){
    fov = fovf;
    xres = reso[1];
    yres = reso[0];
    origin = Point3d(lookAt[0], lookAt[1], lookAt[2]);
    target = Point3d(lookAt[3], lookAt[4], lookAt[5]);
    up = Vector3d(lookAt[6], lookAt[7], lookAt[8]);
    f = (target - origin).norm();
    double theta = fov * 0.5;
    xLength = 2.0 * f * tan(theta);
    double yox = yres * 1.0 / (xres * 1.0);
    yLength = xLength * yox;
    isWToICalc = false;
    isIToWCalc = false;
    
    m_worldToCam.setIdentity();
    m_camToImgPlane = MatrixXd::Identity(3, 4);
    m_imgPlaneToImage = MatrixXd::Identity(2, 3);
    
    m_imageToImgPlane.setIdentity();
    m_imgPlaneToCam.setIdentity();
    m_camToWorld.setIdentity();
    
    Point3d zero(0.0, 0.0, 0.0);
    printf("Camera position: (%lf, %lf, %lf)\n", origin[0], origin[1], origin[2]);
    printf("Camera target: (%lf, %lf, %lf)\n", target[0], target[1], target[2]);
}

PerspectiveCamera::~PerspectiveCamera() {}

void PerspectiveCamera::calcWToIMatrices() {
    Vector3d VPN = target - origin;
    Vector3d n = VPN.normalized();
    Vector3d Vup = up.normalized();
    Vector3d u = n.cross(Vup).normalized();
    Vector3d v = u.cross(n).normalized();
    
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
    
    double invDx = xres / xLength;
    double invDy = yres / yLength;
    double u0 = xres * 0.5;
    double v0 = yres * 0.5;
    m_imgPlaneToImage(0, 0) = invDx;
    m_imgPlaneToImage(0, 2) = u0;
    m_imgPlaneToImage(1, 1) = invDy;
    m_imgPlaneToImage(1, 2) = v0;
    
    isWToICalc = true;
}

void PerspectiveCamera::calcIToWMatrices() {
    double Dx = xLength / xres;
    double Dy = yLength / yres;
    double u0 = xres * 0.5;
    double v0 = yres * 0.5;
    m_imageToImgPlane(0, 0) = Dx;
    m_imageToImgPlane(0, 2) = -u0 * Dx;
    m_imageToImgPlane(1, 1) = Dy;
    m_imageToImgPlane(1, 2) = -v0 * Dy;
    
    double invF = 1.0 / f;
    m_imgPlaneToCam(0, 0) = invF;
    m_imgPlaneToCam(1, 1) = invF;
    
    Vector3d VPN = target - origin;
    Vector3d n = VPN.normalized();
    Vector3d Vup = up.normalized();
    Vector3d u = n.cross(Vup).normalized();
    Vector3d v = u.cross(n).normalized();
    
    double ux = u.x(), uy = u.y(), uz = u.z();
    double vx = v.x(), vy = v.y(), vz = v.z();
    double nx = n.x(), ny = n.y(), nz = n.z();
    double A = nx*uy*vz - nx*uz*vy - ny*ux*vz + ny*uz*vx + nz*ux*vy - nz*uy*vx;
    double oriDotU = origin.dot(u);
    double oriDotV = origin.dot(v);
    double oriDotN = origin.dot(n);
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

Point2i PerspectiveCamera::worldToImg(Point3d p){
    Point4d homoP(p(0), p(1), p(2), 1.0);
    if (!isWToICalc) {
        calcWToIMatrices();
    }
    Point4d posInCam = m_worldToCam * homoP;
    posInCam = posInCam / posInCam(2);
    Point2d posInPix = m_imgPlaneToImage * m_camToImgPlane *  posInCam;
    posInPix(1) = yres - posInPix(1);
    
    return Point2i(posInPix(1), posInPix(0)); //(row, col)
}

Point3d PerspectiveCamera::imgToWorld(Point2d p) { //p:(row, col)
    if (!isIToWCalc) {
        calcIToWMatrices();
    }
    Point2d pos = Point2d(p(1), yres - p(0)); //pos:(x, y);
    Point3d homoP(pos(0), pos(1), 1.0);
    Point3d imgPlaneP = m_imageToImgPlane * homoP;
//    std::cout<<"imgPlaneP: "<<imgPlaneP.transpose()<<std::endl;
    Point3d camP = f * m_imgPlaneToCam * imgPlaneP;
//    std::cout<<"camp: "<<camP.transpose()<<std::endl;
    Point4d homoCamP = Point4d(camP(0), camP(1), camP(2), 1.0);
    Point4d homoWorldP = m_camToWorld * homoCamP;
    return Point3d(homoWorldP(0), homoWorldP(1), homoWorldP(2));
}

void PerspectiveCamera::calcRayParas(Point3d pos, Ray *ray) {
    Vector3d dd = (pos - origin).normalized();
    ray->setRay(origin, dd, 0.0);
}

Ray* PerspectiveCamera::generateRays(int rowi, int coli) {
//    for (int rowi = 0; rowi < yres; rowi++) {
//        for (int coli = 0; coli < xres; coli++) {
    
            if (rays == nullptr) {
                rays = new Ray[sampleCount]();
            }
            else{
                delete[] rays;
               rays = new Ray[sampleCount]();
            }
            #pragma omp parallel for schedule(dynamic)
            for (int spi = 0; spi < sampleCount; spi++) {
                Point2d p(rowi, coli);
//                std::cout<<"p = "<<p.transpose()<<std::endl;
                p += sampler->get2D() - Point2d(0.5, 0.5);
                Point3d pW = imgToWorld(p);
//                std::cout<<"pW = "<<pW.transpose()<<std::endl;
                int offset = spi;
                calcRayParas(pW, &rays[offset]);
                if (!rays[offset].isInit()) {
                    std::cout<<pW.transpose()<<::std::endl;
                }
            }
            return rays;
//        }
//    }
//    for (int i = 0; i < yres; i++) {
//        for (int j = 0; j < xres; j++) {
//            std::cout<<"("<<i<<", "<<j<<"): "<<rays[i * xres + j].getDirection().transpose()<<std::endl;
//        }
//    }
}





