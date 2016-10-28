//
// Created by louis on 19/10/16.
//

#include <glad/glad.h>

#include "Light.h"
#include "Context.h"
#include "Program.h"

Light::Light(LightType type, float x, float y, float z) :
    type(type), x(x), y(y), z(z) {

}

void Light::pushLight(Context *context) {

    context->program().setUniform1i("light.type", (int) this->type);
    context->program().setUniform3f("light.position", this->x, this->y, this->z);
}