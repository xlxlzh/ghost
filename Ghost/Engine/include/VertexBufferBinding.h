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

        void setBinding(unsigned short index, const VertexBufferPtr& buffer);
        void unsetBinding(unsigned short index);
        void unsetAllBinding();

        const VertexBufferBindingMap& getBindings() const;

        const VertexBufferPtr& getBuffer(unsigned short index) const;

        bool isBufferBound(unsigned short index) const;

        unsigned getBufferCount() const { return _bindingMap.size(); }

    protected:
        VertexBufferBindingMap _bindingMap;
        mutable unsigned short _highIndex;
    };

    DECLAR_SMART_POINTER(VertexBufferBinding);
}

#endif
