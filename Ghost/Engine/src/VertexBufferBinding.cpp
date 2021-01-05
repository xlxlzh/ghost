#include "VertexBufferBinding.h"
#include "LogManager.h"
#include <algorithm>

namespace ghost
{
    VertexBufferBinding::VertexBufferBinding() : _highIndex(0)
    {

    }

    VertexBufferBinding::~VertexBufferBinding()
    {
        unsetAllBinding();
    }

    void VertexBufferBinding::setBinding(unsigned short index, const VertexBufferPtr& buffer)
    {
        _bindingMap[index] = buffer;
        _highIndex = std::max(_highIndex, (unsigned short)(_highIndex + 1));
    }

    void VertexBufferBinding::unsetBinding(unsigned short index)
    {
        auto it = _bindingMap.find(index);
        if (it == _bindingMap.end())
        {
            GHOST_LOG_FORMAT_ERROR("Can not find buffer binding for index.");
            return;
        }

        _bindingMap.erase(it);
    }

    void VertexBufferBinding::unsetAllBinding()
    {
        _bindingMap.clear();
        _highIndex = 0;
    }

    const VertexBufferBindingMap& VertexBufferBinding::getBindings() const
    {
        return _bindingMap;
    }

    const VertexBufferPtr& VertexBufferBinding::getBuffer(unsigned short index) const
    {
        auto it = _bindingMap.find(index);
        if (it == _bindingMap.end())
        {
            GHOST_LOG_FORMAT_ERROR("No buffer is bound to that index");
            return nullptr;
        }

        return it->second;
    }

    bool VertexBufferBinding::isBufferBound(unsigned short index) const
    {
        return _bindingMap.find(index) != _bindingMap.end();
    }
}