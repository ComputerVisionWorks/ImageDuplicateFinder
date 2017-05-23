#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>

using namespace std;

class UnionFind
{
public:
    UnionFind(int nbNodes);
    /**
     * @brief Find Return the root of a node.
     * @param node A node.
     * @return The root of the node.
     */
    int Find(int node);

    /**
     * @brief Size Return the size of the subset in which a node is.
     * @param node A node.
     * @return The size of the subset in which the node is.
     */
    int Size(int node);

    /**
     * @brief Union Unify two nodes.
     * @param nodeX A node X.
     * @param nodeY A node Y.
     * @return The new root of the two nodes.
     */
    int Union(int nodeX, int nodeY);

private:
    struct Node
    {
        int parent;
        int rang;
        int children;
    };

    vector<Node> _nodes;
};

#endif // UNIONFIND_H
