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
        virtual ResourcePtr Clone();

        virtual void InitDefault();
        virtual void Release();
        virtual bool Load(DataStream& dataStream) = 0;
        virtual void Save(DataStream& dataStream) = 0;
        virtual void Unload();

        int GetType() const { return _type; }
        int GetFlags() const { return _flags; }
        const std::string& GetName() const { return _name; }
        void SetName(const std::string& name) { _name = name; }
        bool IsLoaded() const { return _loaded; }

        static int GetTypeStatic() { return RESOURCE_NONE; }

    protected:
        std::string          _name{0};
        int                  _type = RESOURCE_NONE;
        bool                 _loaded = false;
        int                  _flags = 0;
    };

    #define DECLAR_RESOURCE_TYPE(type) static int GetTypeStatic() { return RESOURCE_##type; }

    class GHOST_API ResourceFactory
    {
    public:
        virtual ResourcePtr CreateResource(const std::string& name, int flags) = 0;
        virtual void DestoryResource(ResourcePtr res) = 0;
        virtual int GetType() = 0;
    };

    template <typename T>
    class GHOST_API ResourceFactoryIml : public ResourceFactory
    {
    public:
        virtual ResourcePtr CreateResource(const std::string& name, int flags) override
        {
            ResourcePtr res = GHOST_MAKE_SMART_POINTER(T);
            res->SetName(name);
            return res;
        }

        virtual void DestoryResource(ResourcePtr res)
        {
            res.reset();
        }

        virtual int GetType()
        {
            return T::GetTypeStatic();
        }
    };

    class GHOST_API ResourceManager : public SingleTon<ResourceManager>
    {
    public:
        void RegisterResourceFactory(ResourceFactory* factory);
        void RegisterResourceFactory(int type, ResourceFactory* factory);

        ResourcePtr AddResource(int type, const std::string &name, int flags);
        ResourcePtr AddResource(ResourcePtr& resource);
        int RemoveResource(Resource &resource);
        ResourcePtr FindResource(int type, const std::string& name) const;

        ResourcePtr CloneResource(Resource& sourceResource, const std::string& name);

        void Clear();

        void SetResourcesPath(const std::string resourcesPath) { _resourcesPath = resourcesPath; }
        const std::string& GetResourcesPath() const { return _resourcesPath; }

    protected:
        std::vector<ResourcePtr> _resources;
        std::unordered_map<int, ResourceFactory*> _resourceFactories;

        std::string _resourcesPath{};
    };
}

#endif
