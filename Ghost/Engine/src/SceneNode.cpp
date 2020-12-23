#include "SceneNode.h"
#include "SceneManager.h"

namespace ghost
{
    SceneNode::SceneNode(SceneManager* owner) : _owner(owner), _dirty(true), _parent(nullptr)
    {

    }

    void SceneNode::setTransform(const Vector3f& pos, const Vector3f& rotation, const Vector3f& scale)
    {
        //TODO
    }

    void SceneNode::getTransform(Vector3f& pos, Vector3f& rotation, Vector3f& scale)
    {
        //TODO
    }

    void SceneNode::update()
    {
        if (!_dirty)
            return;

        if (_parent != nullptr)
            ;
        else
            _absTrans = _relTrans;

        
        _owner->updateNode(this);

        _dirty = false;

        for (auto& child : _children)
        {
            child->update();
        }
    }

    void SceneNode::markDirty()
    {
        _dirty = true;

        SceneNode* node = _parent;
        while (node != nullptr)
        {
            node->_dirty = true;
            node = node->_parent;
        }

        markChildrenDirty();
    }

    void SceneNode::markChildrenDirty()
    {
        for (auto& nodeIt : _children)
        {
            if (!nodeIt->_dirty)
            {
                nodeIt->_dirty = true;
                markChildrenDirty();
            }
        }
    }
}