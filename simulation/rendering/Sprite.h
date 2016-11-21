//
// Created by louis on 21/11/16.
//

#ifndef RENDUOPENGL_SPRITE_H
#define RENDUOPENGL_SPRITE_H

#include <glm/glm.hpp>

#include "Renderable.h"

class Sprite : public Renderable {
public :
    /** Constructeur de Sprite
     * @param width La largeur du sprite, en pixels
     * @param height La hauteur du sprite en pixels*/
    Sprite(const std::string & texture, int width, int height);

    /**
     * Dessine le sprite tel que son coin en haut à gauche
     * soit situé au point (x, y)
     * @param x L'abscisse du coin en haut à gauche du dessin.
     * En pixels.
     * @param y L'ordonnée du coin en haut à gauche du dessin.
     * En pixel
     */
    void drawAt(Context* context, int x, int y);
    virtual void render(Context * context, Scene * scene);

private :
    virtual void buildRenderData(Context * context);

    std::string _textureName;
    std::unique_ptr<Texture> _texture;
    int _width;
    int _height;
};


#endif //RENDUOPENGL_SPRITE_H
