//
// Created by louis on 21/11/16.
//

#ifndef RENDUOPENGL_SPEEDINDICATOR_H
#define RENDUOPENGL_SPEEDINDICATOR_H

#include "rendering/Renderable.h"
#include "rendering/Sprite.h"

#define INDICATOR_WIDTH 30
#define INDICATOR_HEIGHT 30
#define OFFSET_LEFT 30
#define OFFSET_TOP 30

class SpeedIndicator : public Renderable {
public :
    SpeedIndicator(const std::string & texture);

    void setSpeed(uint speed);
    void setSignum(int signum);

    virtual void render(Context * context, Scene *scene);
private:
    Sprite sprite;

    uint speed = 1;
    int signum;
};


#endif //RENDUOPENGL_SPEEDINDICATOR_H
