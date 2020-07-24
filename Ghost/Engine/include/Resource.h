#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>
#include "SingleTon.h"
#include "Ghost.h"
#include "DataStream.h"

namespace ghost
{
    enum ResourceTypes
    {
        RESOURCE_NONE,
        RESOURCE_TEXTURE2D,
        RESOURCE_MATERIAL,
        RESOURCE_SCENE,
    };

    using ResHandle = unsigned int;
    class GHOST_API Resource
    {
        friend class ResourceManager;
    public:
        Resource(int type) : _type(type) { }
        Resource(int type, const std::string &name, int flags);
        virtual ~Resource();
        virtual Resource* clone();

        virtual void initDefault();
        virtual void release();
        virtual bool load(DataStream& dataStream) = 0;
        virtual void unload();

        int getType() const { return _type; }
        int getFlags() const { return _flags; }
        const std::string& getName() const { return _name; }
        void setName(std::string& name) { _name = name; }
        bool isLoaded() const { return _loaded; }
        ResHandle getHandle() const { return _handle; }

        void addRef() { ++_refCount; }
        void subRef() { --_refCount; assert(_refCount >= 0); }

    protected:
        std::string          _name{0};
        int                  _type = RESOURCE_NONE;
        bool                 _loaded = false;
        int                  _flags = 0;

        ResHandle            _handle = 0;

        unsigned             _refCount = 0;
    };

    DECLAR_SMART_POINTER(Resource);

    class GHOST_API ResourceFactory
    {
    public:
        virtual Resource* createResource(const std::string& name, int flags) = 0;
        virtual int getType() = 0;
    };

    class GHOST_API ResourceManager : public SingleTon<ResourceManager>
    {
    public:
        void registerResourceFactory(ResourceFactory* factory);
        void registerResourceFactory(int type, ResourceFactory* factory);

        ResHandle addResource(int type, const std::string &name, int flags);
        ResHandle addResource(Resource& resource);
        int removeResource(Resource &resource);
        Resource* findResource(int type, const std::string& name) const;

        ResHandle cloneResource(Resource& sourceResource, const std::string& name);

        void clear();

    protected:
        std::vector<Resource*> _resources;
        std::unordered_map<int, ResourceFactory*> _resourceFactories;
    };
}

#endif
