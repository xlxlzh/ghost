#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Resource.h"

namespace ghost
{
    class GHOST_API Material : public Resource
    {
    public:
        Material() : Material(RESOURCE_MATERIAL, "", 0) { }
        Material(int type, const std::string& name, int flag);

        virtual bool load(DataStream& dataStream) override;

        static int getTypeStatic() { return RESOURCE_MATERIAL; }

    private:

    };
}

#endif
