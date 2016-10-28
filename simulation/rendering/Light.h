//
// Created by louis on 19/10/16.
//

#ifndef RENDUOPENGL_LIGHT_H
#define RENDUOPENGL_LIGHT_H

class Context;

enum LightType {
    LIGHT_POINT = 0,
    LIGHT_SUN = 1
};

class Light {
public :
    Light(LightType type = LIGHT_POINT, float x = 0, float y = 0, float z = 0);

    void pushLight(Context *context);
private :
    LightType type;
    float x, y, z;
};


#endif //RENDUOPENGL_LIGHT_H
