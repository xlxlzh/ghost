#include "Resource.h"

namespace ghost
{
    Resource::Resource(int type, const std::string &name, int flags)
        : _type(type),
        _name(name),
        _flags(flags)
    {

    }

    Resource::~Resource()
    {

    }

    void Resource::initDefault()
    {

    }

    void Resource::release()
    {

    }

    bool Resource::load(const char *data, int size)
    {
        if (_loaded)
            return false;

        if (data == 0x0 || size <= 0)
        {
            return false;
        }

        _loaded = true;

        return true;
    }

    void Resource::unload()
    {
        release();
        initDefault();
        _loaded = false;
    }


    ResHandle ResourceManager::addResource(int type, const std::string &name, int flags)
    {
        if (name == "")
            return 0;

        for (unsigned i = 0; i < _resources.size(); ++i)
        {
            if (_resources[i] != nullptr && _resources[i]->_name == name)
            {
                if (_resources[i]->_type == type)
                {
                    return i + 1;
                }
            }
        }

        return 0;
    }

    int ResourceManager::removeResource(Resource &resource)
    {
        return 0;
    }
}