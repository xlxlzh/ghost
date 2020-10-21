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

        return false;
    }
}