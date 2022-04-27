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
    enum ResourceTypes : int
    {
        RESOURCE_NONE,
        RESOURCE_TEXTURE2D,
        RESOURCE_MATERIAL,
        RESOURCE_SHADER,
        RESOURCE_SCENE,
        RESOURCE_MESH,
        RESOURCE_MODEL,
    };

    class Resource;

    DECLAR_SMART_POINTER(Resource);

    class GHOST_API Resource
    {
        friend class ResourceManager;
    public:
        Resource(int type) : _type(type) { }
        Resource(int type, const std::string &name, int flags);
        virtual ~Resource();
        virtual ResourcePtr clone();

        virtual void initDefault();
        virtual void release();
        virtual bool load(DataStream& dataStream) = 0;
        virtual void unload();

        int getType() const { return _type; }
        int getFlags() const { return _flags; }
        const std::string& getName() const { return _name; }
        void setName(const std::string& name) { _name = name; }
        bool isLoaded() const { return _loaded; }

        static int getTypeStatic() { return RESOURCE_NONE; }

    protected:
        std::string          _name{0};
        int                  _type = RESOURCE_NONE;
        bool                 _loaded = false;
        int                  _flags = 0;
    };

    #define DECLAR_RESOURCE_TYPE(type) static int getTypeStatic() { return RESOURCE_##type; }

    class GHOST_API ResourceFactory
    {
    public:
        virtual ResourcePtr createResource(const std::string& name, int flags) = 0;
        virtual void destoryResource(ResourcePtr res) = 0;
        virtual int getType() = 0;
    };

    template <typename T>
    class GHOST_API ResourceFactoryIml : public ResourceFactory
    {
    public:
        virtual ResourcePtr createResource(const std::string& name, int flags) override
        {
            ResourcePtr res = GHOST_MAKE_SMART_POINTER(T);
            res->setName(name);
            return res;
        }

        virtual void destoryResource(ResourcePtr res)
        {
            res.reset();
        }

        virtual int getType()
        {
            return T::getTypeStatic();
        }
    };

    class GHOST_API ResourceManager : public SingleTon<ResourceManager>
    {
    public:
        void registerResourceFactory(ResourceFactory* factory);
        void registerResourceFactory(int type, ResourceFactory* factory);

        ResourcePtr addResource(int type, const std::string &name, int flags);
        ResourcePtr addResource(ResourcePtr& resource);
        int removeResource(Resource &resource);
        ResourcePtr findResource(int type, const std::string& name) const;

        ResourcePtr cloneResource(Resource& sourceResource, const std::string& name);

        void clear();

        void setResourcesPath(const std::string resourcesPath) { _resourcesPath = resourcesPath; }
        const std::string& getResourcesPath() const { return _resourcesPath; }

    protected:
        std::vector<ResourcePtr> _resources;
        std::unordered_map<int, ResourceFactory*> _resourceFactories;

        std::string _resourcesPath{};
    };
}

#endif
