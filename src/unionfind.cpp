#include "unionfind.h"

UnionFind::UnionFind(unsigned int nbNodes)
{
    _nodes.resize(nbNodes);

    for (unsigned int i = 0;i < nbNodes;i++)
    {
        _nodes[i].parent = i;
        _nodes[i].rang = 0;
        _nodes[i].children = 1;
    }
}

unsigned int UnionFind::Find(unsigned int node)
{
    if (_nodes[node].parent != node)
    {
        _nodes[node].parent = Find(_nodes[node].parent);
    }

    return _nodes[node].parent;
}

unsigned int UnionFind::Size(unsigned int node)
{
    unsigned int root = Find(node);

    return _nodes[root].children;
}

unsigned int UnionFind::Union(unsigned int nodeX, unsigned int nodeY)
{
    unsigned int rootX = Find(nodeX);
    unsigned int rootY = Find(nodeY);
    unsigned int newRoot = rootX;

    if (rootX != rootY)
    {
        if (_nodes[rootX].rang < _nodes[rootY].rang)
        {
            _nodes[rootX].parent = rootY;
            _nodes[rootY].children += _nodes[rootX].children;
            newRoot = rootY;
        }
        else if (_nodes[rootX].rang > _nodes[rootY].rang)
        {
            _nodes[rootY].parent = rootX;
            _nodes[rootX].children += _nodes[rootY].children;
        }
        else
        {
            _nodes[rootY].parent = rootX;
            _nodes[rootX].rang++;
            _nodes[rootX].children += _nodes[rootY].children;
        }
    }

    return newRoot;
}
