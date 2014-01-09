#ifndef OCTREE_H
#define OCTREE_H

#include "figure.h"
#include <QSharedPointer>

class OctreeNode;
typedef QSharedPointer< OctreeNode > OctreeNodePtr;

class OctreeNode
{
public:

  OctreeNode();


  inline OctreeNode*& parent() { return m_parent; }
  inline QVector<OctreeNodePtr>& children() { return m_children; }
  inline QVector<polygon*> &polys() { return m_polys; }
  inline void makeLeaf() { m_isLeaf = true; }
  inline bool isLeaf() const { return m_isLeaf; }

  QVector3D maxBorder;
  QVector3D minBorder;

private:

  bool m_isLeaf;

  OctreeNode* m_parent;
  QVector<OctreeNodePtr> m_children; // for nodes
  QVector<polygon*> m_polys; // for leafs

};


class octree
{
public:

    int maxPolysPerLeaf;

    void cleanup();

    void build( QVector<figure>  &scene_objects, int object_index, QVector3D minBorder, QVector3D maxBorder, bool for_chain );
    void buildNode( OctreeNode* parent, int level );
    OctreeNode* getRoot() { return m_root; }

    octree(int maxLeafs);
    ~octree();

private:

    OctreeNode* m_root;

};




#endif // OCTREE_H
