//
// Created by louis on 06/11/16.
//

#ifndef RENDUOPENGL_RENDERABLESPHERE_H
#define RENDUOPENGL_RENDERABLESPHERE_H

#include "Renderable.h"

class RenderableSphere : public RenderableModel {
public :
    RenderableSphere(float radius = 1, int segmentCount = 24, int ringCount = 24);

    virtual void render(Context *context, Scene *scene) override;

private :
    /// Calcule les points du modèle de la sphère et crée un
    /// buffer openGL qui accueille les données.
    void buildModelData(Context *context);

    float radius = 1;

    /// nombre de méridiens de la sphère
    int segmentCount = 24;
    /// nombre de parralèles de la sphère
    int ringCount = 24;
};

#endif //RENDUOPENGL_RENDERABLESPHERE_H
