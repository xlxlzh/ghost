#include "Material.h"
#include "../../External/tinyxml2/tinyxml2.h"
#include "DataStream.h"
#include "Engine.h"
#include "LogManager.h"

namespace ghost
{
    const static std::map<std::string, FilterOptions> FilterOptionMappings =
    {
        {"FILTER_POINT", FO_POINT},
        {"FILTER_LINEAR", FO_LINEAR},
        {"FILTER_ANISOTROPIC", FO_ANISOTROPIC},
    };

    const static std::map<std::string, FilterType> FilterTypeMappings =
    {
        {"MIN", FT_MIN},
        {"MAG", FT_MAG},
        {"MIP", FT_MIP},
    };


    ShaderPass::ShaderPass()
    {

    }

    ShaderPass::ShaderPass(RenderPass pass) : _passType(pass)
    {

    }

    ShaderPass::~ShaderPass()
    {

    }

    unsigned ShaderPass::getConstBufferSlot(ShaderType type, const std::string& name)
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

    void ShaderPass::applyTextureToSlot(const std::string& name, Texture2DPtr ptr)
    {
        for (ShaderParams& param : _params)
        {
            auto it = std::find_if(param._textures.begin(), param._textures.end(), 
                [&name](const TextureVariableInfo& info)->bool 
                {
                    if (info._name == name)
                        return true;

                    return false;
                });

            if (it != param._textures.end())
                it->_texture = ptr;
        }
    }

    /*void ShaderPass::applySamplerToSlot(const std::string& name, SamplerFilter filter)
    {
        for (ShaderParams& param : _params)
        {
            auto it = std::find_if(param._samplers.begin(), param._samplers.end(),
                [&name](const SamplerInfo& info)->bool
            {
                if (info._name == name)
                    return true;

                return false;
            });

            if (it != param._samplers.end())
                it->_filter = filter;
        }
    }*/


    Material::Material(int type, const std::string& name, int flag) :
        Resource(type, name, flag)
    {

    }

    RenderPass GetPassByName(const char* name)
    {
        if (strcmp(name, "forward") == 0)
            return RENDER_PASS_FORWARD;
        else if (strcmp(name, "shadow") == 0)
            return RENDER_PASS_SHADOW;
        else if (strcmp(name, "deferred") == 0)
            return RENDER_PASS_DEFERRED;
        else if (strcmp(name, "depth") == 0)
            return RENDER_PASS_DEPTH;
        else
            return RENDER_PASS_FORWARD;
    }

    constexpr static const char* TexturesNames[] =
    {
        "Abldo",
        "Normal",
    };


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

        _passes.clear();
        tinyxml2::XMLElement* passElement = root->FirstChildElement();
        while (passElement)
        {
            if (strcmp(passElement->Name(), "Pass") == 0)
            {
                RenderPass passType = GetPassByName(passElement->Attribute("name"));

                _passes.push_back(RenderPass(passType));
                ShaderPass& currentPass = _passes.back();

                currentPass._defines.clear();
                tinyxml2::XMLElement* shaderDefines = passElement->FirstChildElement("Defines");
                if (shaderDefines)
                {
                    tinyxml2::XMLElement* definesElement = shaderDefines->FirstChildElement();
                    while (definesElement)
                    {
                        const char* definesName = definesElement->Name();
                        const char* definesValue = definesElement->Attribute("value");
                        currentPass._defines[definesName] = definesValue;

                        definesElement = definesElement->NextSiblingElement();
                    }
                }

                tinyxml2::XMLElement* shader = passElement->FirstChildElement("Shader");
                if (!shader)
                {
                    GHOST_LOG_FORMAT_ERROR("Can not find <Shader> element in material %s.", this->getName().c_str());
                    SAFE_DELETE_ARRAY(data);
                    return false;
                }

                const tinyxml2::XMLAttribute* shaderSource = shader->FindAttribute("source");
                if (shaderSource)
                {
                    currentPass._linkedShader = GHOST_SMARTPOINTER_CAST(ShaderResource, ResourceManager::getInstance()->addResource(RESOURCE_SHADER, shaderSource->Value(), 0));

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
                            renderDevice->compileShader(type, entryName, currentPass._defines, *currentPass._linkedShader);
                        }

                        shaderEntry = shaderEntry->NextSiblingElement();
                    }

                    //Shader reflection
                    const ShaderByteCode* byteCode = currentPass._linkedShader->getByteCodeByType(SHADER_VS);
                    if (byteCode)
                    {
                        renderDevice->reflectShader(currentPass._linkedShader, currentPass._params);
                    }
                }

                const tinyxml2::XMLElement* uniforms = passElement->FirstChildElement("Uniforms");
                if (uniforms)
                {
                    //TODO
                }

                const tinyxml2::XMLElement* textures = passElement->FirstChildElement("Textures");
                if (textures)
                {
                    const tinyxml2::XMLElement* textureElement = textures->FirstChildElement();
                    while (textureElement)
                    {
                        const char* textureName = textureElement->Name();
                        const char* texturePath = textureElement->Attribute("path");
                        if (texturePath)
                        {
                            Texture2DPtr tex2D = GHOST_SMARTPOINTER_CAST(Texture2D, ResourceManager::getInstance()->addResource(RESOURCE_TEXTURE2D, texturePath, 0));
                            currentPass.applyTextureToSlot(textureName, tex2D);
                        }

                        textureElement = textureElement->NextSiblingElement();
                    }
                }

                const tinyxml2::XMLElement* samplers = passElement->FirstChildElement("Samplers");
                if (samplers)
                {
                    const tinyxml2::XMLElement* sampler = samplers->FirstChildElement();
                    while (sampler)
                    {
                        const tinyxml2::XMLElement* filter = sampler->FirstChildElement();
                        while (filter)
                        {
                            const char* elementName = filter->Name();
                            //TODO

                            filter = filter->NextSiblingElement();
                        }

                        sampler = samplers->NextSiblingElement();
                    }
                }
            }

            passElement = passElement->NextSiblingElement();
        }

        return true;
    }

    void Material::applyToRenderPass(RenderPass pass)
    {
        ShaderPass* currentPass = getShaderPass(pass);
        if (currentPass == nullptr || currentPass->_linkedShader == nullptr)
            return;

        if (!currentPass->_handwareShader)
        {
            auto renderDevice = Engine::getInstance()->getRenderDevice();
            currentPass->_handwareShader = renderDevice->createShader(currentPass->_linkedShader);
        }

        auto rendersystem = Engine::getInstance()->getRenderSystem();

        //Setup shader
        Shader* hardwareShader = currentPass->_handwareShader;
        if (hardwareShader && hardwareShader->isValid())
        {
            rendersystem->setShader(hardwareShader);

            const ShaderParamsList& params = currentPass->_params;
            for (unsigned i = 0; i < SHADER_TYPE_NUM; ++i)
            {
                const auto& param = params[i];
                for (const TextureVariableInfo& texInfo : param._textures)
                {
                    rendersystem->setTexture((ShaderType)i, texInfo._bindPoint, texInfo._texture);
                }
            }
        }
    }

    ShaderPass* Material::getShaderPass(RenderPass pass)
    {
        for (unsigned i = 0; i < _passes.size(); ++i)
        {
            if (_passes[i]._passType == pass)
                return &_passes[i];
        }

        return nullptr;
    }
}