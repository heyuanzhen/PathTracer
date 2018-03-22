//
//  sampling.h
//  PathTracer
//
//  Created by HYZ on 2018/2/4.
//  Copyright © 2018年 HYZ. All rights reserved.
//

//This file contains inline sampling functions

#ifndef sampling_h
#define sampling_h

#include "typeAlias.h"
#include "globalConstants.h"
#include "Sampler.h"

//RandomSampler rsp;

//pbrt: page: 775
inline Point3d uniformSampleHemisphere(const Point2d u){
    double z = u[0];
    double r = sqrt(1.0 - z * z);
    double phi = 2 * Pi * u[1];
    return Point3d(r * cos(phi), r * sin(phi), z);
}
inline double uniformSampleHemispherePDF(){
    return Inv2Pi;
}


//pbrt: page776
inline Point3d uniformSampleSphere(const Point2d u){
    double z = 1.0 - 2.0 * u[0];
    double r = sqrt(1.0 - z * z);
    double phi = 2 * Pi * u[1];
    return Point3d(r * cos(phi), r * sin(phi), z);
}
inline double uniformSampleSpherePDF(){
    return Inv4Pi;
}

//pbrt: page 777. This function is not used
inline Point2d uniformSampleDisk(const Point2d u) {
    double r = sqrt(u[0]);
    double theta = 2 * M_PI * u[1];
    Point2d p = Point2d(r * cos(theta), r * sin(theta));
    return p;
}

//pbrt: page 778
inline Point2d concentricSampleDisk(const Point2d u) {
    // Map uniform random numbers to $[-1,1]^2$
    Point2d uMapped = 2.0 * u - Vector2d(1.0, 1.0);
    
    // Handle degeneracy at the origin
    if (uMapped[0] == 0 && uMapped[1] == 0) {
        return Point2d(0.0, 0.0);
    }
    
    double r, theta;
    if(abs(uMapped[0]) > abs(uMapped[1])) {
        r = uMapped[0];
        theta = PiOver4 * (uMapped[0] / uMapped[1]);
    }
    else{
        r = uMapped[1];
        theta = PiOver2 - PiOver4 * (uMapped[0] / uMapped[1]);
    }
    Point2d p = Point2d(r * cos(theta), r * sin(theta));
    return p;
}

//pbrt: page 780
inline Point3d cosineSampleHemisphere(const Point2d u){
    Point2d p_disk = concentricSampleDisk(u);
    double z = sqrt(1.0 - p_disk.x() * p_disk.x() - p_disk.y() - p_disk.y());
    return Point3d(p_disk.x(), p_disk.y(), z);
}
inline double cosinSampleHemisphere(double cosTheta){
    return cosTheta * InvPi;
}

//pbrt: page 781
inline Point3d uniformSampleCone(const Point2d u, const double cosThetaMax){
    double cosTheta = (1.0 - u[0]) + u[0] * cosThetaMax;
    double sinTheta = sqrt(1.0 - cosTheta * cosTheta);
    double phi = u[1] * 2 * Pi;
    return Point3d(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
}
inline double uniformSampleConePDF(const double cosThetaMax){
    return 1.0 / (2.0 * Pi * (1.0 - cosThetaMax));
}

//what's this ????
inline double powerHeuristic(int nf, double fPdf, int ng, double gPdf) {
    double f = nf * fPdf, g = ng * gPdf;
    return (f * f) / (f * f + g * g);
}




#endif /* sampling_h */
