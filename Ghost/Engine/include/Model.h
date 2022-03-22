#ifndef _MODEL_H_
#define _MODEL_H_

#include "Ghost.h"
#include "Resource.h"
#include "Material.h"

namespace ghost
{
    class GHOST_API SubMesh
    {
    public:
        
    };

    class GHOST_API Model : public Resource
    {
    public:
        Model() : Model(RESOURCE_MODEL, "", 0) { }
        Model(int type, const std::string& name, int flag);

        virtual bool load(DataStream& dataStream) override;

        DECLAR_RESOURCE_TYPE(MODEL);

    protected:
        std::vector<SubMesh> _meshes;
        std::vector<Material> _materials;
    };
}

#endif