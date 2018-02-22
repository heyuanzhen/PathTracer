//
//  Scene.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/20.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Scene.h"
#include <iostream>

Scene::Scene(int spT, int liT) : shapeTotal(spT), lightTotal(liT) {
    shapeCount = 0;
    lightCount = 0;
    shapes = new Shape*[spT]();
    lights = new Light*[liT]();
}

Scene::~Scene(){
    delete[] shapes;
    delete[] lights;
}

Shape* Scene::getShape(int spi) {
    if (spi >= shapeTotal) {
        std::cout<<"Shape index out of range !"<<std::endl;
        return nullptr;
    }
    return shapes[spi];
}

Light* Scene::getLight(int li) {
    if (li >= lightTotal) {
        std::cout<<"Light index out of range !"<<std::endl;
        return nullptr;
    }
    return lights[li];
}

int Scene::getShapeTotal() const {
    return shapeTotal;
}

int Scene::getLightTotal() const {
    return lightTotal;
}

int Scene::getShapeCount() const {
    return shapeCount;
}

int Scene::getLightCount() const {
    return lightCount;
}

void Scene::addShape(Shape *sp) {
    shapes[shapeCount] = sp;
    shapeCount++;
}

void Scene::addLight(Light *li) {
    lights[lightCount] = li;
    lightCount++;
}
