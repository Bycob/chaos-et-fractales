//
// Created by louis on 21/11/16.
//

#include "SpeedIndicator.h"

SpeedIndicator::SpeedIndicator(const std::string & texture) :
        sprite(texture, INDICATOR_WIDTH, INDICATOR_HEIGHT) {

}

void SpeedIndicator::setSpeed(uint speed) {
    this->speed = speed;
}

void SpeedIndicator::setSignum(int signum) {
    this->signum = signum;
}

void SpeedIndicator::render(Context *context, Scene *scene) {
    //L'indicateur est en haut à gauche de l'écran.

    for (int i = 0 ; i < speed ; i++) {
        sprite.drawAt(context, OFFSET_LEFT + i * INDICATOR_WIDTH, OFFSET_TOP);
    }
}