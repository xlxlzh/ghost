#include "Model.h"

namespace ghost
{
    bool Model::load(DataStream& dataStream)
    {
        if (dataStream.getSize() <= 0)
            return false;

        return true;
    }
}