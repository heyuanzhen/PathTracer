//
//  Light.h
//  PathTracer
//
//  Created by HYZ on 2018/2/20.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#ifndef Light_h
#define Light_h

class Light {
public:
    enum LightType{
        POINT = 1
    };
    
    const LightType type;
    Light(LightType tp);
    ~Light();
};


#endif /* Light_h */
