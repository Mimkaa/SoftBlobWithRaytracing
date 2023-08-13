#include "Drawable.h"
#include "Graphics.h"
#include "BIndablesHeader.h"

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
            p->Draw(gfx, num_indicies);
        }
    }

}