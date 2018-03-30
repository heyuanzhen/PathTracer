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

void Scene::readObjFile(std::string fileName, Shape **triangles, bool isE, double* cen, double* len) {
    objl::Loader loader;
    bool loadout = loader.LoadFile(fileName);
    int meshCount = (int)loader.LoadedMeshes.size();
    triangles = new Shape* [meshCount]();
    if (loadout) {
        int tri = 0;
        double maxX = -MAX_DOUBLE, maxY = -MAX_DOUBLE, maxZ = -MAX_DOUBLE;
        double minX = MAX_DOUBLE, minY = MAX_DOUBLE, minZ = MAX_DOUBLE;
        for (int i = 0; i < meshCount; i++) {
            objl::Mesh curMesh = loader.LoadedMeshes[i];
            int surfaceCount = (int)curMesh.Indices.size();
            for (int j = 0; j < surfaceCount; j+= 3) {
                int id1 = curMesh.Indices[j], id2 = curMesh.Indices[j + 1], id3 = curMesh.Indices[j + 2];
                Point3d p1(curMesh.Vertices[id1].Position.X, curMesh.Vertices[id1].Position.Y, curMesh.Vertices[id1].Position.Z);
                maxX = curMesh.Vertices[id1].Position.X > maxX ? curMesh.Vertices[id1].Position.X : maxX;
                minX = curMesh.Vertices[id1].Position.X < minX ? curMesh.Vertices[id1].Position.X : minX;
                maxY = curMesh.Vertices[id1].Position.Y > maxY ? curMesh.Vertices[id1].Position.Y : maxY;
                minY = curMesh.Vertices[id1].Position.Y < minY ? curMesh.Vertices[id1].Position.Y : minY;
                maxZ = curMesh.Vertices[id1].Position.Z > maxZ ? curMesh.Vertices[id1].Position.Z : maxZ;
                minZ = curMesh.Vertices[id1].Position.Z < minZ ? curMesh.Vertices[id1].Position.Z : minZ;
                Point3d p2(curMesh.Vertices[id2].Position.X, curMesh.Vertices[id2].Position.Y, curMesh.Vertices[id2].Position.Z);
                maxX = curMesh.Vertices[id2].Position.X > maxX ? curMesh.Vertices[id2].Position.X : maxX;
                minX = curMesh.Vertices[id2].Position.X < minX ? curMesh.Vertices[id2].Position.X : minX;
                maxY = curMesh.Vertices[id2].Position.Y > maxY ? curMesh.Vertices[id2].Position.Y : maxY;
                minY = curMesh.Vertices[id2].Position.Y < minY ? curMesh.Vertices[id2].Position.Y : minY;
                maxZ = curMesh.Vertices[id2].Position.Z > maxZ ? curMesh.Vertices[id2].Position.Z : maxZ;
                minZ = curMesh.Vertices[id2].Position.Z < minZ ? curMesh.Vertices[id2].Position.Z : minZ;
                Point3d p3(curMesh.Vertices[id3].Position.X, curMesh.Vertices[id3].Position.Y, curMesh.Vertices[id3].Position.Z);
                maxX = curMesh.Vertices[id3].Position.X > maxX ? curMesh.Vertices[id3].Position.X : maxX;
                minX = curMesh.Vertices[id3].Position.X < minX ? curMesh.Vertices[id3].Position.X : minX;
                maxY = curMesh.Vertices[id3].Position.Y > maxY ? curMesh.Vertices[id3].Position.Y : maxY;
                minY = curMesh.Vertices[id3].Position.Y < minY ? curMesh.Vertices[id3].Position.Y : minY;
                maxZ = curMesh.Vertices[id3].Position.Z > maxZ ? curMesh.Vertices[id3].Position.Z : maxZ;
                minZ = curMesh.Vertices[id3].Position.Z < minZ ? curMesh.Vertices[id3].Position.Z : minZ;
                triangles[tri] = new Triangle(p1, p2, p3, isE);
                addShape(triangles[tri++]);

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
        printf("minX = %lf, minY = %lf, minZ = %lf\n", minX, minY, minZ);
        printf("maxX = %lf, maxY = %lf, maxZ = %lf\n", maxX, maxY, maxZ);
        printf("cenX = %lf, cenY = %lf, cenZ = %lf\n", cenX, cenY, cenZ);
        printf("lenX = %lf, lenY = %lf, lenZ = %lf\n", lenX, lenY, lenZ);
    }
}








