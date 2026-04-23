#include "Octree.h"

namespace ghost
{
    Octree::Octree(Octree* parent) : _parent(parent)
    {
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                for (int k = 0; k < 2; ++k)
                {
                    _children[i][j][k] = nullptr;
                }
            }
        }

        _numNodes = 0;
    }

    Octree::~Octree()
    {
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                for (int k = 0; k < 2; ++k)
                {
                    SAFE_DELETE(_children[i][j][k]);
                }
            }
        }

        _parent = nullptr;
        _numNodes = 0;
    }

    void Octree::AddNode(SceneNode* node)
    {
        _nodes.push_back(node);
        node->SetTree(this);

        IncreaseNumNodes();
    }

    void Octree::RemoveNode(SceneNode* node)
    {
        _nodes.erase(std::find(_nodes.begin(), _nodes.end(), node));
        node->SetTree(nullptr);

        DecreaseNumNodes();
    }

    void Octree::IncreaseNumNodes()
    {
        ++_numNodes;

        if (_parent != nullptr)
            _parent->IncreaseNumNodes();
    }

    void Octree::DecreaseNumNodes()
    {
        --_numNodes;

        if (_parent != nullptr)
            _parent->DecreaseNumNodes();
    }

    bool Octree::IsTwiceSize(const BoundingBox& box) const
    {
        Vector3f halfSize = _boundingBox.GetHalfSize();
        Vector3f boxSize = box.GetSize();

        return (boxSize._x <= halfSize._x) && (boxSize._y <= halfSize._y) && (boxSize._z <= halfSize._z);
    }

    void Octree::GetChildIndexes(const BoundingBox& box, int& x, int& y, int& z) const
    {
        Vector3f treeCenter = _boundingBox.GetCenter();
        Vector3f boxCenter = box.GetCenter();

        if (boxCenter._x > treeCenter._x)
            x = 1;
        else
            x = 0;

        if (boxCenter._y > treeCenter._y)
            y = 1;
        else
            y = 0;

        if (boxCenter._z > treeCenter._z)
            z = 1;
        else
            z = 0;
    }
}