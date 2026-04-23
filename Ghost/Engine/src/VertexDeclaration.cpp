#include "VertexDeclaration.h"

namespace ghost
{
    VertexElement::VertexElement(unsigned short slot, unsigned offset, VertexElementType type, 
        VertexElementSemantic semantic, unsigned short index /* = 0 */) :
        _slot(slot), _offset(offset), _type(type), _semantic(semantic), _index(index)
    {

    }

    unsigned VertexElement::GetSize() const
    {
        return GetTypeSize(_type);
    }

    unsigned VertexElement::GetTypeSize(VertexElementType type)
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

    unsigned short VertexElement::GetTypeCount(VertexElementType type)
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

    const VertexElement& VertexDeclaration::AddElement(unsigned short slot, unsigned offset, VertexElementType type, VertexElementSemantic semantic, unsigned short index /* = 0 */)
    {
        _elementList.push_back(VertexElement(slot, offset, type, semantic, index));

        return _elementList.back();
    }

    const VertexElement& VertexDeclaration::InsertElement(unsigned short slot, unsigned short pos, unsigned offset, VertexElementType type, VertexElementSemantic semantic, unsigned short index /* = 0 */)
    {
        if (pos >= _elementList.size())
            return AddElement(slot, offset, type, semantic, index);

        auto it = _elementList.begin();
        for (unsigned short i = 0; i < pos; ++i)
            ++it;

        it = _elementList.insert(it, VertexElement(slot, offset, type, semantic, index));

        return *it;
    }

    const VertexElement* VertexDeclaration::GetElement(unsigned short index) const
    {
        assert(index < _elementList.size());

        auto it = _elementList.begin();
        for (unsigned short i = 0; i < index; ++i)
            ++it;

        return &(*it);
    }

    const std::list<VertexElement>& VertexDeclaration::GetElements() const
    {
        return _elementList;
    }

    void VertexDeclaration::RemoveElement(unsigned short pos)
    {
        assert(pos < _elementList.size());

        auto it = _elementList.begin();
        for (unsigned short i = 0; i < pos; ++i)
            ++it;

        _elementList.erase(it);
    }

    void VertexDeclaration::RemoveElement(VertexElementSemantic semantic, unsigned short index /* = 0 */)
    {
        for (auto it = _elementList.begin(); it != _elementList.end(); ++it)
        {
            if (it->GetSemantic() == semantic && it->GetIndex() == index)
            {
                _elementList.erase(it);
                break;
            }
        }
    }

    void VertexDeclaration::RemoveAllElements()
    {
        _elementList.clear();
    }

    const VertexElement* VertexDeclaration::FindElementBySemantic(VertexElementSemantic semantic, unsigned short index) const
    {
        for (auto it = _elementList.begin(); it != _elementList.end(); ++it)
        {
            if (it->GetSemantic() == semantic && it->GetIndex() == index)
            {
                return &(*it);
            }
        }

        return nullptr;
    }
}