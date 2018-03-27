//
//  Scene.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/20.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Scene.h"
#include "Light.h"
#include "OBJ_Loader.h"
#include "Shape.h"
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

Shape* Scene::getShape(int spi) const {
    if (spi >= shapeTotal || spi >= shapeCount) {
        std::cout<<"Shape index "<<spi<<" out of range !"<<std::endl;
        return nullptr;
    }
    return shapes[spi];
}

Light* Scene::getLight(int li) const {
    if (li >= lightTotal || li >= lightCount) {
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
    Light::powerSum += li->getPower();
}

void Scene::readObjFile(std::string fileName, Shape **triangles, bool isE) {
    objl::Loader loader;
    bool loadout = loader.LoadFile(fileName);
    int meshCount = (int)loader.LoadedMeshes.size();
    triangles = new Shape* [meshCount]();
    if (loadout) {
        int tri = 0;
        for (int i = 0; i < meshCount; i++) {
            objl::Mesh curMesh = loader.LoadedMeshes[i];
            int surfaceCount = (int)curMesh.Indices.size();
            for (int j = 0; j < surfaceCount; j+= 3) {
                int id1 = curMesh.Indices[j], id2 = curMesh.Indices[j + 1], id3 = curMesh.Indices[j + 2];
                Point3d p1(curMesh.Vertices[id1].Position.X, curMesh.Vertices[id1].Position.Y, curMesh.Vertices[id1].Position.Z);
                Point3d p2(curMesh.Vertices[id2].Position.X, curMesh.Vertices[id2].Position.Y, curMesh.Vertices[id2].Position.Z);
                Point3d p3(curMesh.Vertices[id3].Position.X, curMesh.Vertices[id3].Position.Y, curMesh.Vertices[id3].Position.Z);
                triangles[tri] = new Triangle(p1, p2, p3, isE);
                addShape(triangles[tri++]);
            }
        }
    }
}











