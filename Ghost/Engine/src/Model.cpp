#include "Model.h"

namespace ghost
{
    Model::Model(int type, const std::string& name, int flag) : Resource(type, name, flag)
    {

    }

    bool Model::load(DataStream& dataStream)
    {
        if (dataStream.getSize() <= 0)
            return false;

        return true;
    }
}