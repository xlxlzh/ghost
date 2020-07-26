#include "Texture2D.h"

namespace ghost
{
    Texture2D::Texture2D() : Resource(RESOURCE_TEXTURE2D, "", 0)
    {

    }

    Texture2D::Texture2D(const std::string& name, int flag) : Resource(RESOURCE_TEXTURE2D, name, flag)
    {

    }

    bool Texture2D::load(DataStream& dataStream)
    {

        return true;
    }

    Texture2D::~Texture2D()
    {

    }
}