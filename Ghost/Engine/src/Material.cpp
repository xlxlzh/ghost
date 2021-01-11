#include "Material.h"
#include "../../External/tinyxml2/tinyxml2.h"
#include "DataStream.h"
#include "Engine.h"
#include "LogManager.h"

namespace ghost
{
    Material::Material(int type, const std::string& name, int flag) :
        Resource(type, name, flag)
    {

    }

    bool Material::load(DataStream& dataStream)
    {
        int size = dataStream.getSize();
        char* data = new char[size];
        dataStream.read(data, size);

        tinyxml2::XMLDocument doc;
        doc.Parse(data, size);

        tinyxml2::XMLElement* root = doc.RootElement();
        if (strcmp(root->Name(), "Material") != 0)
        {
            GHOST_LOG_FORMAT_ERROR("Can not find <Material> element in material %s.", this->getName().c_str());
            SAFE_DELETE_ARRAY(data);
            return false;
        }

        _defines.clear();
        tinyxml2::XMLElement* shaderDefines = root->FirstChildElement("Defines");
        if (shaderDefines)
        {
            tinyxml2::XMLElement* definesElement = shaderDefines->FirstChildElement();
            while (definesElement)
            {
                const char* definesName = definesElement->Name();
                const char* definesValue = definesElement->Attribute("value");
                _defines[definesName] = definesValue;

                definesElement = definesElement->NextSiblingElement();
            }
        }

        tinyxml2::XMLElement* shader = root->FirstChildElement("Shader");
        if (!shader)
        {
            GHOST_LOG_FORMAT_ERROR("Can not find <Shader> element in material %s.", this->getName().c_str());
            SAFE_DELETE_ARRAY(data);
            return false;
        }

        const tinyxml2::XMLAttribute* shaderSource = shader->FindAttribute("source");
        if (shaderSource)
        {
            _shaderResource = GHOST_SMARTPOINTER_CAST(ShaderResource, ResourceManager::getInstance()->addResource(RESOURCE_SHADER, shaderSource->Value(), 0));

            auto renderDevice = Engine::getInstance()->getRenderDevice();

            const tinyxml2::XMLElement* shaderEntry = shader->FirstChildElement();
            while (shaderEntry)
            {
                const char* name = shaderEntry->Name();
                ShaderType type = GetShaderTypeByName(name);

                const tinyxml2::XMLAttribute* entry = shaderEntry->FindAttribute("entry");
                if (entry)
                {
                    const char* entryName = entry->Value();
                    renderDevice->compileShader(type, entryName, _defines, *_shaderResource);
                }

                shaderEntry = shaderEntry->NextSiblingElement();
            }

            //Shader reflection
            const ShaderByteCode* byteCode = _shaderResource->getByteCodeByType(SHADER_VS);
            if (byteCode)
            {
                renderDevice->reflectShader(_shaderResource, _params);
            }
        }

        const tinyxml2::XMLElement* uniforms = root->FirstChildElement("Uniforms");
        if (uniforms)
        {
            //TODO
        }

        const tinyxml2::XMLElement* textures = root->FirstChildElement("Textures");
        if (textures)
        {

        }

        const tinyxml2::XMLElement* samplers = root->FirstChildElement("Samplers");
        if (samplers)
        {
            //TODO
        }

        return true;
    }

    void Material::apply()
    {
        if (_shaderResource == nullptr)
            return;

        if (!_handwareShader)
        {
            auto renderDevice = Engine::getInstance()->getRenderDevice();
            _handwareShader = renderDevice->createShader(_shaderResource);
        }

        auto rendersystem = Engine::getInstance()->getRenderSystem();

        //Setup shader
        if (_handwareShader && _handwareShader->isValid())
        {
            rendersystem->setShader(_handwareShader);
        }
    }

    unsigned Material::getConstBufferSlot(ShaderType type, const std::string& name)
    {
        ShaderParams param = _params[type];
        auto& buffers = param._constBuffers;
        for (auto& cb : buffers)
        {
            if (cb._name == name)
                return cb._bindPoint;
        }

        return 0;
    }
}