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
    objShapesCount = 0;
    shapes = new Shape*[spT]();
    lights = new Light*[liT]();
    objShapes = nullptr;
}

Scene::~Scene(){
    delete[] shapes;
    delete[] lights;
    cleanObj();
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

void Scene::readObjFile(std::string fileName, bool isE, double* cen, double* len) {
    objl::Loader loader;
    bool loadout = loader.LoadFile(fileName);
    int meshCount = (int)loader.LoadedMeshes.size();
    int totalSurface = 0;
    for (int i = 0; i < meshCount; i++) {
        totalSurface += loader.LoadedMeshes[i].Indices.size();
    }
    objShapes = new Shape* [totalSurface]();
    if (loadout) {
//        int tri = 0;
        double maxX = -MAX_DOUBLE, maxY = -MAX_DOUBLE, maxZ = -MAX_DOUBLE;
        double minX = MAX_DOUBLE, minY = MAX_DOUBLE, minZ = MAX_DOUBLE;
        for (int i = 0; i < meshCount; i++) {
            objl::Mesh curMesh = loader.LoadedMeshes[i];
            int surfaceCount = (int)curMesh.Indices.size();
            for (int j = 0; j < surfaceCount; j+= 3) {
                int id1 = curMesh.Indices[j], id2 = curMesh.Indices[j + 1], id3 = curMesh.Indices[j + 2];
                Point3d p1(curMesh.Vertices[id1].Position.X, curMesh.Vertices[id1].Position.Y, curMesh.Vertices[id1].Position.Z);
                Point3d p2(curMesh.Vertices[id2].Position.X, curMesh.Vertices[id2].Position.Y, curMesh.Vertices[id2].Position.Z);
                Point3d p3(curMesh.Vertices[id3].Position.X, curMesh.Vertices[id3].Position.Y, curMesh.Vertices[id3].Position.Z);
                
                Vector3d n1 = Vector3d(curMesh.Vertices[id1].Normal.X, curMesh.Vertices[id1].Normal.Y, curMesh.Vertices[id1].Normal.Z);
                Vector3d n2 = Vector3d(curMesh.Vertices[id2].Normal.X, curMesh.Vertices[id2].Normal.Y, curMesh.Vertices[id2].Normal.Z);
                Vector3d n3 = Vector3d(curMesh.Vertices[id3].Normal.X, curMesh.Vertices[id3].Normal.Y, curMesh.Vertices[id3].Normal.Z);
                objShapes[objShapesCount] = new Triangle(p1, p2, p3, n1, n2, n3, isE);
                addShape(objShapes[objShapesCount++]);
                
                maxX = curMesh.Vertices[id1].Position.X > maxX ? curMesh.Vertices[id1].Position.X : maxX;
                minX = curMesh.Vertices[id1].Position.X < minX ? curMesh.Vertices[id1].Position.X : minX;
                maxY = curMesh.Vertices[id1].Position.Y > maxY ? curMesh.Vertices[id1].Position.Y : maxY;
                minY = curMesh.Vertices[id1].Position.Y < minY ? curMesh.Vertices[id1].Position.Y : minY;
                maxZ = curMesh.Vertices[id1].Position.Z > maxZ ? curMesh.Vertices[id1].Position.Z : maxZ;
                minZ = curMesh.Vertices[id1].Position.Z < minZ ? curMesh.Vertices[id1].Position.Z : minZ;
                maxX = curMesh.Vertices[id2].Position.X > maxX ? curMesh.Vertices[id2].Position.X : maxX;
                minX = curMesh.Vertices[id2].Position.X < minX ? curMesh.Vertices[id2].Position.X : minX;
                maxY = curMesh.Vertices[id2].Position.Y > maxY ? curMesh.Vertices[id2].Position.Y : maxY;
                minY = curMesh.Vertices[id2].Position.Y < minY ? curMesh.Vertices[id2].Position.Y : minY;
                maxZ = curMesh.Vertices[id2].Position.Z > maxZ ? curMesh.Vertices[id2].Position.Z : maxZ;
                minZ = curMesh.Vertices[id2].Position.Z < minZ ? curMesh.Vertices[id2].Position.Z : minZ;
                maxX = curMesh.Vertices[id3].Position.X > maxX ? curMesh.Vertices[id3].Position.X : maxX;
                minX = curMesh.Vertices[id3].Position.X < minX ? curMesh.Vertices[id3].Position.X : minX;
                maxY = curMesh.Vertices[id3].Position.Y > maxY ? curMesh.Vertices[id3].Position.Y : maxY;
                minY = curMesh.Vertices[id3].Position.Y < minY ? curMesh.Vertices[id3].Position.Y : minY;
                maxZ = curMesh.Vertices[id3].Position.Z > maxZ ? curMesh.Vertices[id3].Position.Z : maxZ;
                minZ = curMesh.Vertices[id3].Position.Z < minZ ? curMesh.Vertices[id3].Position.Z : minZ;
            }
        }
        double cenX = (maxX + minX) * 0.5;
        double cenY = (maxY + minY) * 0.5;
        double cenZ = (maxZ + minZ) * 0.5;
        double lenX = maxX - minX;
        double lenY = maxY - minY;
        double lenZ = maxZ - minZ;
        cen[0] = cenX; cen[1] = cenY; cen[2] = cenZ;
        len[0] = lenX; len[1] = lenY; len[2] = lenZ;
        printf("OBJ file infomation: \n");
        printf("minX = %lf, minY = %lf, minZ = %lf\n", minX, minY, minZ);
        printf("maxX = %lf, maxY = %lf, maxZ = %lf\n", maxX, maxY, maxZ);
        printf("cenX = %lf, cenY = %lf, cenZ = %lf\n", cenX, cenY, cenZ);
        printf("lenX = %lf, lenY = %lf, lenZ = %lf\n", lenX, lenY, lenZ);
        printf("\n");
    }
}


void Scene::cleanObj() {
    for (int i = 0; i < objShapesCount; i++) {
        delete objShapes[i];
    }
    delete[] objShapes;
}





