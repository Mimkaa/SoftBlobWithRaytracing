#pragma once
#include <vector>
#include <memory>

class Graphics;
class Bindable;


class Drawable
{
public:
    
    void Bind(Graphics& gfx);
    void Draw(Graphics& gfx); 
    virtual void BindAdditional(Graphics& gfx);
protected:
    std::vector<std::shared_ptr<Bindable>> bindables;
    int num_indicies;
    
};
