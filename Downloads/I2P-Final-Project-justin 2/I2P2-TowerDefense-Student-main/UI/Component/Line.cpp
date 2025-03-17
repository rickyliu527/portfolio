//
// Created by GaGa-PC on 2024/5/19.
//

#include "Line.h"
#include <allegro5/allegro_font.h>
#include <memory>

#include "Engine/IObject.hpp"
#include "Label.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include <allegro5/allegro_primitives.h>

namespace Engine {
    Line::Line(float x1, float y1, float x2, float y2, unsigned char r, unsigned char g, unsigned char b,float width, float anchorX, float anchorY) :
            IObject(x1, y1, 0, 0, anchorX, anchorY), Color(al_map_rgb(r, g, b)), x1(x1), y1(y1), x2(x2), y2(y2), width(width){
    }
    void Line::Draw() const {
        al_draw_line(x1, y1, x2, y2, Color, width);
    }
}
