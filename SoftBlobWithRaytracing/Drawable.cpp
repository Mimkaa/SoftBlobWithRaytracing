#include "Drawable.h"
#include "Graphics.h"
#include "BIndablesHeader.h"
#include "Drawable.h"



void Drawable::BindAdditional(Graphics& gfx)
{

}

void Drawable::Bind(Graphics& gfx)
{
  
    for (auto& b : bindables)
    {
        b->Bind(gfx);
    }
}



void Drawable::Draw(Graphics& gfx)
{
    for (auto& b : bindables)
    {
        if (auto p = std::dynamic_pointer_cast<Drawer>(b))
        {
            if (num_indicies == 0)
            {
                throw ("not enough indices(");
            }
            p->Draw(gfx, num_indicies);
        }
    }

}