#include "VertexDeclaration.h"

namespace ghost
{
    VertexElement::VertexElement(unsigned short slot, unsigned offset, VertexElementType type, 
        VertexElementSemantic semantic, unsigned short index /* = 0 */) :
        _slot(slot), _offset(offset), _type(type), _semantic(semantic), _index(index)
    {

    }

    unsigned VertexElement::getSize() const
    {
        return getTypeSize(_type);
    }

    unsigned VertexElement::getTypeSize(VertexElementType type)
    {
        switch (type)
        {
        case VET_FLOAT_1:
            return sizeof(float);
        case VET_FLOAT_2:
            return sizeof(float) * 2;
        case VET_FLOAT_3:
            return sizeof(float) * 3;
        case VET_FLOAT_4:
            return sizeof(float) * 4;
        }

        return 0;
    }

    unsigned short VertexElement::getTypeCount(VertexElementType type)
    {
        switch (type)
        {
        case VET_FLOAT_1:
            return 1;
        case VET_FLOAT_2:
            return 2;
        case VET_FLOAT_3:
            return 3;
        case VET_FLOAT_4:
            return 4;
        }

        return 0;
    }



    /****************************************** VertexDeclaration **********************************/

    VertexDeclaration::VertexDeclaration()
    {

    }

    VertexDeclaration::~VertexDeclaration()
    {

    }

    const VertexElement& VertexDeclaration::addElement(unsigned short slot, unsigned offset, VertexElementType type, VertexElementSemantic semantic, unsigned short index /* = 0 */)
    {
        _elementList.push_back(VertexElement(slot, offset, type, semantic, index));

        return _elementList.back();
    }

    const VertexElement& VertexDeclaration::insertElement(unsigned short slot, unsigned short pos, unsigned offset, VertexElementType type, VertexElementSemantic semantic, unsigned short index /* = 0 */)
    {
        if (pos >= _elementList.size())
            return addElement(slot, offset, type, semantic, index);

        auto it = _elementList.begin();
        for (unsigned short i = 0; i < pos; ++i)
            ++it;

        it = _elementList.insert(it, VertexElement(slot, offset, type, semantic, index));

        return *it;
    }

    const VertexElement* VertexDeclaration::getElement(unsigned short index) const
    {
        assert(index < _elementList.size());

        auto it = _elementList.begin();
        for (unsigned short i = 0; i < index; ++i)
            ++it;

        return &(*it);
    }

    const std::list<VertexElement>& VertexDeclaration::getElements() const
    {
        return _elementList;
    }

    void VertexDeclaration::removeElement(unsigned short pos)
    {
        assert(pos < _elementList.size());

        auto it = _elementList.begin();
        for (unsigned short i = 0; i < pos; ++i)
            ++it;

        _elementList.erase(it);
    }

    void VertexDeclaration::removeElement(VertexElementSemantic semantic, unsigned short index /* = 0 */)
    {
        for (auto it = _elementList.begin(); it != _elementList.end(); ++it)
        {
            if (it->getSemantic() == semantic && it->getIndex() == index)
            {
                _elementList.erase(it);
                break;
            }
        }
    }

    void VertexDeclaration::removeAllElements()
    {
        _elementList.clear();
    }

    const VertexElement* VertexDeclaration::findElementBySemantic(VertexElementSemantic semantic, unsigned short index) const
    {
        for (auto it = _elementList.begin(); it != _elementList.end(); ++it)
        {
            if (it->getSemantic() == semantic && it->getIndex() == index)
            {
                return &(*it);
            }
        }

        return nullptr;
    }
}