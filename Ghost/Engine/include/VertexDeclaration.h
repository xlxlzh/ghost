#ifndef _VERTEXDECLARATION_H_
#define _VERTEXDECLARATION_H_

#include "Ghost.h"
#include <list>

namespace ghost
{
    enum VertexElementSemantic
    {
        VES_POSITION        = 1,
        VES_BLEND_WEIGHTS   = 2,
        VES_BLEND_INDICES   = 3,
        VES_NORMAL          = 4,
        VES_DIFFUSE         = 5,
        VES_SPECULAR        = 6,
        VES_TEXTURE_COORDINATES = 7,
        VES_BINORMAL        = 8,
        VES_TANGENT         = 9,
        VES_COUNT           = 10,
    };

    enum VertexElementType
    {
        VET_FLOAT_1         = 0,
        VET_FLOAT_2         = 1,
        VET_FLOAT_3         = 2,
        VET_FLOAT_4         = 3,

        //TO BE ADDED
    };

    class GHOST_API VertexElement
    {
    public:
        VertexElement() { }
        VertexElement(unsigned short slot, unsigned offset, VertexElementType type, VertexElementSemantic semantic, unsigned short index = 0);

        unsigned short getIndex() const { return _index; }
        unsigned getOffset() const { return _offset; }
        unsigned short getStreamSlot() const { return _slot; }
        VertexElementType getType() const { return _type; }
        VertexElementSemantic getSemantic() const { return _semantic; }

        unsigned getSize() const;

    public:
        static unsigned getTypeSize(VertexElementType type);
        static unsigned short getTypeCount(VertexElementType type);

    protected:
        unsigned short _index;
        unsigned int   _offset;
        unsigned short _slot;

        VertexElementType _type;
        VertexElementSemantic _semantic;
    };

    class GHOST_API VertexDeclaration
    {
    public:
        VertexDeclaration();
        virtual ~VertexDeclaration();

        unsigned getElementCount() const { return _elementList.size(); }
        const VertexElement* getElement(unsigned short index) const;
        const std::list<VertexElement>& getElements() const;

        const VertexElement& addElement(unsigned short slot, unsigned offset, VertexElementType type, VertexElementSemantic semantic, unsigned short index = 0);
        const VertexElement& insertElement(unsigned short slot, unsigned short pos, unsigned offset, VertexElementType type, VertexElementSemantic semantic, unsigned short index = 0);

        void removeElement(unsigned short pos);
        void removeElement(VertexElementSemantic semantic, unsigned short index = 0);
        void removeAllElements();

        const VertexElement* findElementBySemantic(VertexElementSemantic semantic, unsigned short index) const;

    protected:
        std::list<VertexElement> _elementList;
    };

    DECLAR_SMART_POINTER(VertexDeclaration);
}

#endif
