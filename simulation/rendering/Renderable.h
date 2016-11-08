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


protected :
    Renderable();

    virtual void deleteBuffers();
    /// Recrée les buffers de données ainsi que le VAO,
    /// puis bind le vertex Array pour préparer un envoi
    /// de données.
    virtual void regenerateBuffers();

    bool compiled = false;
    GLuint gVAO = 0;
};

class RenderableModel : public Renderable {

public :
    RenderableModel();
    ~RenderableModel();

    void setPosition(float x, float y, float z);
    glm::vec3 getPosition();

    void setMaterial(Material &material);
    void addTexturePath(std::string path);

    Material & getMaterial() {
        return this->material;
    }
protected :
    virtual void deleteBuffers();

    virtual void regenerateBuffers();

    /** Recharge les textures et envoie les données à
     * openGL*/
    virtual void loadTextures();

    //Fields

    float x, y, z;
    Material material;
    std::vector<std::string> texturePaths;

    GLuint vertexBuffer = 0;
    GLuint normalBuffer = 0;
    GLuint texCoordBuffer = 0;

    int verticesCount = 0;
};

#endif //RENDUOPENGL_RENDERABLE_H
