//
// Created by GaGa-PC on 2024/5/19.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_LINE_H
#define INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_LINE_H
#include <allegro5/allegro_font.h>
#include <allegro5/color.h>
#include <memory>
#include <string>

#include "Engine/IObject.hpp"

namespace Engine {
    class Line : public IObject {
    protected:
        // Smart pointer to font.
        float x1,y1,x2,y2,width;
    public:
        ALLEGRO_COLOR Color;
        explicit Line(float x1, float y1, float x2, float y2, unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, float width=0 , float anchorX = 0, float anchorY = 0);
        void setX1(float x1){
            this->x1 = x1;
        }
        void setY1(float y1){
            this->y1 = y1;
        }
        void setX2(float x2){
            this->x2 = x2;
        }
        void setY2(float y2){
            this->y2 = y2;
        }
        float getX2(){
            return this->x2;
        }
        void Draw() const override;
    };
}
#endif //INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_LINE_H
