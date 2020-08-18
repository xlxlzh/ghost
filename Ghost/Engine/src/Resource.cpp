#include "Resource.h"
#include <sstream>
#include "DataStream.h"

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

    Resource* Resource::clone()
    {
        return nullptr;
    }

    void Resource::initDefault()
    {

    }

    void Resource::release()
    {

    }

    void Resource::unload()
    {
        release();
        initDefault();
        _loaded = false;
    }


    /*
    @Brief: ResourceManager
    */
    void ResourceManager::registerResourceFactory(int type, ResourceFactory* factory)
    {
        _resourceFactories[type] = factory;
    }

    void ResourceManager::registerResourceFactory(ResourceFactory* factory)
    {
        if (factory == nullptr)
            return;

        _resourceFactories[factory->getType()] = factory;
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

        Resource* newRes = nullptr;
        auto factory = _resourceFactories.find(type);
        if (factory == _resourceFactories.end())
            return 0;

        newRes = factory->second->createResource(name, flags);

        if (newRes == nullptr)
            return 0;

        std::string realPath = _resourcesPath + name;
        DataStream* dataStream = new FileStream(realPath);
        if (dataStream && !dataStream->isOpened())
        {
            dataStream->close();
            SAFE_DELETE(dataStream);
            factory->second->destoryResource(newRes);
            return 0;
        }

        if (!newRes->load(*dataStream))
        {
            dataStream->close();
            SAFE_DELETE(dataStream);
            factory->second->destoryResource(newRes);
            return 0;
        }

        return addResource(*newRes);
    }

    ResHandle ResourceManager::addResource(Resource& resource)
    {
        for (unsigned i = 0; i < _resources.size(); ++i)
        {
            if (_resources[i] == nullptr)
            {
                resource._handle = i + 1;
                _resources[i] = &resource;
                return resource._handle;
            }
        }

        resource._handle = _resources.size() + 1;
        _resources.push_back(&resource);
        return resource._handle;
    }

    int ResourceManager::removeResource(Resource &resource)
    {
        return 0;
    }

    Resource* ResourceManager::findResource(int type, const std::string& name) const
    {
        for (auto res : _resources)
        {
            if (res && res->_type == type && res->_name == name)
            {
                return res;
            }
        }

        return nullptr;
    }

    void ResourceManager::clear()
    {
        for (unsigned i = 0; i < _resources.size(); ++i)
        {
            if (_resources[i])
            {
                _resources[i]->release();
                delete _resources[i];
                _resources[i] = nullptr;
            }
        }
    }

    ResHandle ResourceManager::cloneResource(Resource& sourceResource, const std::string& name)
    {
        if (name != "")
        {
            for (unsigned i = 0; i < _resources.size(); ++i)
            {
                if (_resources[i] != nullptr && _resources[i]->_name == name)
                {
                    return 0;
                }
            }
        }

        Resource* newResource = sourceResource.clone();
        if (newResource == nullptr)
            return 0;

        newResource->_name = name != "" ? name : "tmpResource";
        newResource->_refCount = 0;
        int handle = addResource(*newResource);

        if (name == "")
        {
            std::stringstream ss;
            ss << sourceResource._name << "|" << handle;
            newResource->_name = ss.str();
        }

        return handle;
    }
}