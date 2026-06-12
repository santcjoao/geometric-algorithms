#ifndef __Topology_h
#define __Topology_h

namespace tcii::cg
{ // begin namespace tcii::cg

// meta programming to define what is a topological element?

struct Vertex;
struct HalfEdge;
struct Edge;
struct Face;
struct Boundary;

class TopologyIteratorBase
{};

class VertexIterator: public TopologyIteratorBase
{};

class HalfEdgeIterator: public TopologyIteratorBase
{};

class EdgeIterator: public TopologyIteratorBase
{};

class FaceIterator: public TopologyIteratorBase
{};

class BoundaryIterator: public TopologyIteratorBase
{};

template<typename A>
class Topology
{
public:

  Topology(TriangleMesh& mesh)
  {

  }

private:
  
};

} // end namespace tcii::cg

#endif // __Topology_h