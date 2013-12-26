#include "octree.h"


OctreeNode::OctreeNode()
{

}

octree::octree(int maxLeafs)
{
    maxPolysPerLeaf = maxLeafs;
    m_root = 0;
}

octree::~octree()
{
  if( m_root != 0 ) delete m_root;

}

void octree::cleanup()
{
  if( m_root != 0 ) delete m_root;
  m_root = 0;

}

void octree::build( QVector<figure > &scene_objects, int object_index,QVector3D minBorder, QVector3D maxBorder, bool for_chain )
{
  cleanup();

  m_root = new OctreeNode();

  m_root->minBorder = minBorder;
  m_root->maxBorder = maxBorder;

  int start, end;

  if(for_chain)
  {
      start = 0;
      end = object_index;
  }
  else
  {
      start = object_index;
      end = scene_objects.size();
  }

  for(int i = start; i < end; i++)
  {
      for(int j = 0; j < scene_objects[i].polys.size(); j++)
      {
          m_root->polys().push_back(&scene_objects[i].polys[j]);
      }
  }

  //min max

  buildNode( m_root, 0 );


}

void octree::buildNode( OctreeNode* parent, int level )
{
  if( parent->polys().size() < maxPolysPerLeaf )
  {
    // leaf
    parent->makeLeaf();
  } else
  {
    // split into up to 8 nodes
    // (skip node if has no polys)


     /* int halfDim = ( parent->maxBorder.x() - parent->minBorder.x() ) / 2;

          for( size_t x = 0; x < 2; x++ )
            for( size_t y = 0; y < 2; y++ )
              for( size_t z = 0; z < 2; z++ )
              {
                // TODO: убрать лишние динамические выделения памяти
                OctreeNode* node = new OctreeNode();
                node->minBorder = parent->minBorder +
                    Vector3i( x * ( 1 + halfDim ), y * ( 1 + halfDim ), z * ( 1 + halfDim ) );
                node->maxBorder = parent->minBorder +
                    Vector3i( x + ( 1 + x ) * halfDim, y + ( 1 + y ) * halfDim, z + ( 1 + z ) * halfDim );

                // TODO: вынести из цикла
                for( size_t u = 0; u < parent->polys().size(); u++ )
                {
                  int& i = parent->polys()[ u ].i;
                  int& j = parent->polys()[ u ].j;
                  int& k = parent->polys()[ u ].k;

                  if( i <= node->maxBorder.x() && i >= node->minBorder.x() &&
                      j <= node->maxBorder.y() && j >= node->minBorder.y() &&
                      k <= node->maxBorder.z() && k >= node->minBorder.z() )
                    node->polys().push_back( parent->polys()[ u ] );
                }

                if( node->polys().size() > 0 )
                {
                  parent->children().push_back( OctreeNodePtr( node ) );
                  buildNode( node, level + 1 );
                } else
                {
                  delete node;
                }
              }

          if( parent != m_root )
            parent->polys().clear();*/

      double halfDim = ( parent->maxBorder.x() - parent->minBorder.x() ) / 2.0;

      for( int x = 0; x < 2; x++ )
        for( int y = 0; y < 2; y++ )
          for( int z = 0; z < 2; z++ )
          {
              OctreeNode* node = new OctreeNode();
              node->minBorder = parent->minBorder +
                  QVector3D( x * ( halfDim ), y * ( halfDim ), z * ( halfDim ) );
              node->maxBorder = parent->minBorder +
                  QVector3D( ( 1 + x ) * halfDim, ( 1 + y ) * halfDim, ( 1 + z ) * halfDim );

              //проверка вершин



              for( int u = parent->polys().size() - 1; u >= 0; u-- )
              {

                int hit_flag = 0;

                for( int z = 0; z < parent->polys()[u]->vertices.size(); z++)
                 {

                    double i = parent->polys()[ u ]->vertices[z].x();
                    double j = parent->polys()[ u ]->vertices[z].y();
                    double k = parent->polys()[ u ]->vertices[z].z();

                    if( i <= node->maxBorder.x() && i >= node->minBorder.x() &&
                        j <= node->maxBorder.y() && j >= node->minBorder.y() &&
                        k <= node->maxBorder.z() && k >= node->minBorder.z() )
                      hit_flag++ ;
                 }

                 if(hit_flag == 3)
                 {
                     node->polys().push_back( parent->polys()[ u ] );

                     node->parent() = parent;

                     parent->polys().remove(u);
                 }
              }


              //сторим ноду

              if( node->polys().size() > 0 )
              {
                parent->children().push_back( OctreeNodePtr( node ) );
                buildNode( node, level + 1 );
              } else
              {
                delete node;
              }

          }


    //if( parent != m_root )
     // parent->polys().clear();
  }


}
