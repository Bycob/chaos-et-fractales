//
// Created by louis on 07/10/16.
//

#ifndef RENDUOPENGL_RENDERABLE_H
#define RENDUOPENGL_RENDERABLE_H

#include "Material.h"

class Scene;
class Context;

class Renderable {
public :
    virtual ~Renderable();

    virtual void render(Context *context) = 0;

    void setPosition(float x, float y, float z);

    void setMaterial(Material &material);
    void addTexturePath(std::string path);

    Material & getMaterial() {
        return this->material;
    }

protected :
    Renderable();

    void deleteBuffers();
    /// Recrée les buffers de données ainsi que le VAO,
    /// puis bind le vertex Array pour préparer un envoi
    /// de données.
    void regenerateBuffers();

    /** Recharge les textures et envoie les données à
     * openGL*/
    void loadTextures();

    //Fields

    float x, y, z;
    Material material;
    std::vector<std::string> texturePaths;

    bool compiled = false;
    GLuint gVAO = 0;

    GLuint vertexBuffer = 0;
    GLuint normalBuffer = 0;
    GLuint texCoordBuffer = 0;

    int verticesCount = 0;
};

class RenderableSphere : public Renderable {
public :
    RenderableSphere(float radius = 1, int segmentCount = 24, int ringCount = 24);

    virtual void render(Context *context) override;

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

#endif //RENDUOPENGL_RENDERABLE_H
