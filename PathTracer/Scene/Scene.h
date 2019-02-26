//
//  Scene.h
//  PathTracer
//
//  Created by HYZ on 2018/2/20.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#ifndef Scene_h
#define Scene_h

#include <string>

class Shape;
class Light;

class Scene {
private:
    Shape** shapes;
    Light** lights;
    int shapeTotal; //number of all shapes in scene
    int lightTotal;
    int shapeCount; //number of shapes that has been added to shape list now
    int lightCount;
public:
    int objShapesCount;
    Shape** objShapes;
    
    Scene(int spT, int liT);
    ~Scene();
    
    void parseSceneFile(std::string filePath);
    Shape* getShape(int spi) const;
    Light* getLight(int li) const;
    int getShapeTotal() const;
    int getLightTotal() const;
    int getShapeCount() const;
    int getLightCount() const;
    void addShape(Shape* sp);
    void addLight(Light* li);
    void readObjFile(std::string fileName, bool isE, double* cen, double* len);
    void cleanObj();
};


#endif /* Scene_h */
