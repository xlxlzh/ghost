#include "SceneNode.h"
#include "SceneManager.h"

namespace ghost
{
    SceneNode::SceneNode(SceneManager* owner) : _owner(owner), _dirty(true), _parent(nullptr)
    {
        _absTrans.identify();
        _relTrans.identify();
    }

    void SceneNode::setTransform(const Vector3f& pos, const Vector3f& rotation, const Vector3f& scale)
    {
        _relTrans = Matrix4x4f::scaleMatrix(scale._x, scale._y, scale._z);
        _relTrans.rotate(rotation._x, rotation._y, rotation._z);
        _relTrans.translate(pos._x, pos._y, pos._z);

        markDirty();
    }

    void SceneNode::setTransform(const Matrix4x4f& mat)
    {
        _relTrans = mat;

        markDirty();
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
            _absTrans = _parent->_absTrans * _relTrans;
        else
            _absTrans = _relTrans;

        
        _owner->updateNode(this);

        onPostUpdate();

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