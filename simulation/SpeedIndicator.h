//
// Created by louis on 21/11/16.
//

#ifndef RENDUOPENGL_SPEEDINDICATOR_H
#define RENDUOPENGL_SPEEDINDICATOR_H

#include "rendering/Renderable.h"
#include "rendering/Sprite.h"

#define INDICATOR_WIDTH 20
#define INDICATOR_HEIGHT 20
#define OFFSET_LEFT 25
#define OFFSET_TOP 25

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
