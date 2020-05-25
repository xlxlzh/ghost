#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <string>
#include <vector>
#include "SingleTon.h"

namespace ghost
{
    enum ResourceTypes
    {
        RESOURCE_NONE,
        RESOURCE_TEXTURE2D,
        RESOURCE_MATERIAL,
        RESOURCE_SCENE,
    };

    class Resource
    {
        friend class ResourceManager;
    public:
        Resource(int type, const std::string &name, int flags);
        virtual ~Resource();

        virtual void initDefault();
        virtual void release();
        virtual bool load(const char *data, int size);
        void unload();

        int getType() const { return _type; }
        int getFlags() const { return _flags; }

    protected:
        std::string          _name;
        int                  _type = RESOURCE_NONE;
        bool                 _loaded = false;
        int                  _flags = 0;

        unsigned             _refCount = 0;
    };


    using ResHandle = unsigned int;
    class ResourceManager : public SingleTon<ResourceManager>
    {
    public:
        ResHandle addResource(int type, const std::string &name, int flags);
        int removeResource(Resource &resource);

    protected:
        std::vector<Resource*> _resources;
    };
}

#endif
