#ifndef _VERTEXBUFFERBINDING_H_
#define _VERTEXBUFFERBINDING_H_

#include <map>
#include "VertexBuffer.h"

namespace ghost
{
    using  VertexBufferBindingMap = std::map<unsigned short, VertexBufferPtr>;
    class GHOST_API VertexBufferBinding
    {
    public:
        VertexBufferBinding();
        ~VertexBufferBinding();

        void SetBinding(unsigned short index, const VertexBufferPtr& buffer);
        void UnsetBinding(unsigned short index);
        void UnsetAllBinding();

        const VertexBufferBindingMap& GetBindings() const;

        const VertexBufferPtr& GetBuffer(unsigned short index) const;

        bool IsBufferBound(unsigned short index) const;

        unsigned GetBufferCount() const { return _bindingMap.size(); }

    protected:
        VertexBufferBindingMap _bindingMap;
        mutable unsigned short _highIndex;
    };

    DECLAR_SMART_POINTER(VertexBufferBinding);
}

#endif
