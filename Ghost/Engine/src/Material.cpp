#include "Material.h"
#include "../../External/tinyxml2/tinyxml2.h"
#include "DataStream.h"

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
            SAFE_DELETE_ARRAY(data);
            return false;
        }

        tinyxml2::XMLElement* shader = root->FirstChildElement("Shader");
        if (!shader)
        {
            SAFE_DELETE_ARRAY(data);
            return false;
        }

        const tinyxml2::XMLAttribute* shaderSource = shader->FindAttribute("source");
        if (shaderSource)
        {
            ResHandle shaderHandle = ResourceManager::getInstance()->addResource(RESOURCE_SHADER, shaderSource->Value(), 0);
        }

        return true;
    }
}