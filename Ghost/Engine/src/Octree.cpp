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

    void Octree::addNode(SceneNode* node)
    {
        _nodes.push_back(node);
        node->setTree(this);

        _increaseNumNodes();
    }

    void Octree::removeNode(SceneNode* node)
    {
        _nodes.erase(std::find(_nodes.begin(), _nodes.end(), node));
        node->setTree(nullptr);

        _decreaseNumNodes();
    }

    void Octree::_increaseNumNodes()
    {
        ++_numNodes;

        if (_parent != nullptr)
            _parent->_increaseNumNodes();
    }

    void Octree::_decreaseNumNodes()
    {
        --_numNodes;

        if (_parent != nullptr)
            _parent->_increaseNumNodes();
    }

    bool Octree::isTwiceSize(const BoundingBox& box) const
    {
        Vector3f halfSize = _boundingBox.getHalfSize();
        Vector3f boxSize = box.getSize();

        return (boxSize._x <= halfSize._x) && (boxSize._y <= halfSize._y) && (boxSize._z <= halfSize._z);
    }

    void Octree::_getChildIndexes(const BoundingBox& box, int& x, int& y, int& z) const
    {
        Vector3f treeCenter = _boundingBox.getCenter();
        Vector3f boxCenter = box.getCenter();

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