#include "Texture2D.h"

namespace ghost
{
    Texture2D::Texture2D()
    {

    }

    Texture2D::Texture2D(const Texture2DResPtr& texRes) : _rawTexture(texRes), _name("")
    {
        if (_rawTexture)
        {
            _width = _rawTexture->GetWidth();
            _height = _rawTexture->GetHeight();
            _components = _rawTexture->GetComponents();
            _format = _rawTexture->GetFormat();
            _name = _rawTexture->GetName();
        }
    }

    Texture2D::Texture2D(const std::string& name)
    {
        //TODO
    }
    
    Texture2D::~Texture2D()
    {

    }
}