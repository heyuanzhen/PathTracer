//
//  Transform.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/19.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Transform.h"


Transform::Transform() {
    M.setIdentity();
    invM.setIdentity();
    MRot.setIdentity();
    invMRot.setIdentity();
    isInvMCalc = true;
    isInvMRotCalc = true;
}

Transform::~Transform() {}

void Transform::calcInvM() {
    invM = M.inverse();
    isInvMCalc = true;
}

void Transform::calcInvMRot() {
    invMRot = MRot.inverse();
    isInvMRotCalc = true;
}

void Transform::setTranslation(float *ds) {
    M(0, 3) += ds[0];
    M(1, 3) += ds[1];
    M(2, 3) += ds[2];
    isInvMCalc = false;
}

void Transform::setScale(float* s) {
    Matrix4f Mt = Matrix4f::Identity(4, 4);
    Mt(0, 0) = s[0];
    Mt(1, 1) = s[1];
    Mt(2, 2) = s[2];
    M = Mt * M;
    isInvMCalc = false;
}

void Transform::setRotateX(float rad){
    Matrix4f Mt = Matrix4f::Identity(4, 4);
    Mt(1, 1) = cos(rad);
    Mt(1, 2) = -sin(rad);
    Mt(2, 1) = sin(rad);
    Mt(2, 2) = cos(rad);
    M = Mt * M;
    MRot = Mt * MRot;
    isInvMCalc = false;
    isInvMRotCalc = false;
}

void Transform::setRotateY(float rad){
    Matrix4f Mt = Matrix4f::Identity(4, 4);
    Mt(0, 0) = cos(rad);
    Mt(0, 2) = sin(rad);
    Mt(2, 0) = -sin(rad);
    Mt(2, 2) = cos(rad);
    M = Mt * M;
    MRot = Mt * MRot;
    isInvMCalc = false;
    isInvMRotCalc = false;
}

void Transform::setRotateZ(float rad){
    Matrix4f Mt = Matrix4f::Identity(4, 4);
    Mt(0, 0) = cos(rad);
    Mt(0, 1) = -sin(rad);
    Mt(1, 0) = sin(rad);
    Mt(1, 1) = cos(rad);
    M = Mt * M;
    MRot = Mt * MRot;
    isInvMCalc = false;
    isInvMRotCalc = false;
}

void Transform::setIdentityM() {
    M.setIdentity();
    isInvMCalc = false;
}

Matrix4f Transform::getM() const {
    return M;
}

Matrix4f Transform::getMRot() const {
    return MRot;
}

Matrix4f Transform::getInvM() {
    if (!isInvMCalc) {
        calcInvM();
    }
    return invM;
}

Matrix4f Transform::getInvMRot() {
    if (!isInvMRotCalc) {
        calcInvMRot();
    }
    return invMRot;
}











