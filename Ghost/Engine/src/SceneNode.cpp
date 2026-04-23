#include "SceneNode.h"
#include "SceneManager.h"

namespace ghost
{
    SceneNode::SceneNode(SceneManager* owner) : _owner(owner), _dirty(true), _parent(nullptr)
    {
        _absTrans.Identify();
        _relTrans.Identify();
    }

    void SceneNode::SetTransform(const Vector3f& pos, const Vector3f& rotation, const Vector3f& scale)
    {
        _pos = pos;
        _rotation = rotation;
        _scale = scale;

        _relTrans = Matrix4x4f::ScaleMatrix(scale._x, scale._y, scale._z);
        _relTrans.Rotate(rotation._x, rotation._y, rotation._z);
        _relTrans.Translate(pos._x, pos._y, pos._z);

        MarkDirty();
    }

    void SceneNode::SetTransform(const Matrix4x4f& mat)
    {
        _relTrans = mat;

        MarkDirty();
    }

    void SceneNode::SetPosition(const Vector3f& pos)
    {
        _pos = pos;
        _relTrans.Translate(pos._x, pos._y, pos._z);

        MarkDirty();
    }

    void SceneNode::SetScale(const Vector3f& scale)
    {
        _scale = scale;

        SetTransform(_pos, _rotation, _scale);
    }

    void SceneNode::SetRotation(const Vector3f& rotation)
    {
        _rotation = rotation;

        SetTransform(_pos, _rotation, _scale);
    }

    void SceneNode::GetTransform(Vector3f& pos, Vector3f& rotation, Vector3f& scale)
    {
        pos = _pos;
        rotation = _rotation;
        scale = _scale;
    }

    void SceneNode::Update()
    {
        if (!_dirty)
            return;

        if (_parent != nullptr)
            _absTrans = _parent->_absTrans * _relTrans;
        else
            _absTrans = _relTrans;

        
        _owner->UpdateNode(this);

        OnPostUpdate();

        _dirty = false;

        for (auto& child : _children)
        {
            child->Update();
        }
    }

    void SceneNode::MarkDirty()
    {
        _dirty = true;

        SceneNode* node = _parent;
        while (node != nullptr)
        {
            node->_dirty = true;
            node = node->_parent;
        }

        MarkChildrenDirty();
    }

    void SceneNode::MarkChildrenDirty()
    {
        for (auto& nodeIt : _children)
        {
            if (!nodeIt->_dirty)
            {
                nodeIt->_dirty = true;
                MarkChildrenDirty();
            }
        }
    }
}