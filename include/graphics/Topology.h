#ifndef __Topology_h
#define __Topology_h

// OVERVIEW: Topology.h
// ========
// Class definition for general topology.
//
// Authors: Arthur de Andrade, João de Carvalho e Nathan Rezende
// Last revision: 17/06/2026

#include "FreeListVector.h"
#include "Mesh.h"
#include "TriangleMesh.h"
#include <algorithm>
#include <concepts>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <type_traits>

/// @brief This is an example of a Topology, more specifically the given topology of P2.
namespace tcii::cg
{ // begin namespace tcii::cg

using index_t = unsigned;
using ProcFunc = std::function<bool(size_t)>;

enum Elements { VertexData = 0, HalfEdgeData, EdgeData, FaceData, BoundaryData };

// classe para topological element base? e ai os caba herda

struct Vertex;
struct HalfEdge;
struct Edge;
struct Face;
struct Boundary;

template <typename E>
struct is_topo_element: std::false_type {};

template <>
struct is_topo_element<Vertex>: std::true_type {};

template <>
struct is_topo_element<HalfEdge>: std::true_type {};

template <>
struct is_topo_element<Edge>: std::true_type {};

template <>
struct is_topo_element<Face>: std::true_type {};

template <>
struct is_topo_element<Boundary>: std::true_type {};

template <typename E>
constexpr bool is_topo_element_v = is_topo_element<E>::value;

template <typename E>
concept IsTopoElement = is_topo_element_v<E>;

template<typename A>
concept is_topological_collection = requires(A collection, index_t i, A::value_type val)
{
  typename A::value_type;
  // é indexável -> tem uma ordem
  collection[i];

  // tem operação size()
  { collection.size() } -> std::integral;
  
  // Exige o método reserve
  collection.reserve(i);

  requires std::constructible_from<A, size_t>;
  // next pro iterador se pa? mas entra na brisa de ter

  // não pode construtor a partir de tais elementos pq 
  // insert ACHO QUE PRECISA ? Só que pra elementos topológicos
  collection.push_back(val);

  // Exige o método is_active para checagem de índices reciclados no iterador
  { collection.is_active(i) } -> std::same_as<bool>;

  // remove ACHO QUE PRECISA ?
  
};

struct Vertex
{
  index_t id;
  index_t heID;
  Vec3f pos;
};

struct HalfEdge
{
  index_t vID;
  index_t twin;
  index_t eID;
  index_t fID;
  index_t next;
  index_t prev;
};

struct Edge
{
  index_t id;
  index_t twins[2];
};

struct Face
{
  index_t id;
  index_t heID;
};

struct Boundary
{
  index_t id;
  index_t heID;
};

template<typename A>
requires IsTopoElement<typename A::value_type> && is_topological_collection<A>
class TopologyIteratorBase
{
public:
  using element_type = A::value_type;// tirar
  using collection_type = A;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename A::value_type;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;

  TopologyIteratorBase(const A& collection, index_t pos = 0u):
    _value{collection},
    _pos{0} // pos
  {
    if (_pos < _value.size() && !_value.is_active(_pos))
      increment();
  }

  bool operator ==(const TopologyIteratorBase<A>& other)
  {
    return _value[_pos] == other._value[other._pos];
  }

  bool operator !=(const TopologyIteratorBase<A>& other)
  {
    return !operator==(other);
  }

  auto& operator*() const { return _value[_pos]; }

  auto operator->() const { return &operator*(); }

  auto& operator ++()
  {
    increment();
    return *this;
  }

  auto operator ++(int)
  {
    auto temp = *this;

    increment();
    return temp;
  }

  auto& operator --()
  {
    decrement();
    return *this;
  }

  auto operator --(int)
  {
    auto temp = *this;

    decrement();
    return temp;
  }

private:
  A& _value;
  index_t _pos;

  void increment()
  {
    assert(_pos < _value.size());
    do
    {
      _pos++;
    } while (_pos < _value.size() && !_value.is_active(_pos));
  }

  void decrement()
  {
    assert(_pos > 0);
    do
    {
      _pos--;
    } while (_pos > 0 && !_value.is_active(_pos));
  }
};

template
<
  template <typename...> class Collection = FreeListVector, 
  typename V = Vertex,
  typename HE = HalfEdge,
  typename E = Edge,
  typename F = Face,
  typename B = Boundary
>
class Topology
{
public:
  using collection = Collection<V>;

  using vert   = V;
  using he     = HE;
  using edge   = E;
  using face   = F;
  using bounds = B;

  using vert_collection   = Collection<vert  >;
  using he_collection     = Collection<he    >;
  using edge_collection   = Collection<edge  >;
  using face_collection   = Collection<face  >;
  using bounds_collection = Collection<bounds>;

  static_assert(is_topological_collection<vert_collection>, 
    "A coleção fornecida não satisfaz os requisitos topológicos."); // requires não?

  Topology(TriangleMesh& mesh):
    _vertices{},
    _halfEdges{},
    _edges{},
    _faces{},
    _boundaries{}
  {
    auto numVerts = mesh.data().vertexCount();
    auto numFaces = mesh.data().triangleCount();

    _vertices.reserve(numVerts);
    _halfEdges.reserve(3 * numVerts * 2);
    _edges.reserve(3 * numVerts);
    _faces.reserve(2 * numFaces);
    _boundaries.reserve(0);

    std::cerr << "[TOPO] Malha: " << numVerts << "v, " << numFaces << "f. Reservando..." << std::endl;

    for (auto i = 0u; i < numVerts; ++i)
    {
      vert v;
      v.id = i;
      v.heID = std::numeric_limits<index_t>::max();
      v.pos = mesh.data().vertices()[i];
      _vertices.push_back(v);
    }

    std::cerr << "[TOPO] Vertices inseridos: " << _vertices.size() << std::endl;

    // DS to process twins
    std::map<std::pair<index_t, index_t>, index_t> edgeMap;

    for (auto i = 0u; i < numFaces; ++i)
    {
      auto tri = mesh.data().triangles()[i];

      index_t he0_id = _halfEdges.size();
      index_t he1_id = he0_id + 1;
      index_t he2_id = he1_id + 1;
      
      he he0, he1, he2;

      he0.vID = tri[0];
      he0.fID = i;
      he0.next = he1_id;
      he0.prev = he2_id;

      he1.vID = tri[1];
      he1.fID = i;
      he1.next = he2_id;
      he1.prev = he0_id;

      he2.vID = tri[2];
      he2.fID = i;
      he2.next = he0_id;
      he2.prev = he1_id;

      face f;
      f.id = i;
      f.heID = he0_id;
      _faces.push_back(f);

      // updating he_ids
      _vertices[tri[0]].heID = he0_id;
      _vertices[tri[1]].heID = he1_id;
      _vertices[tri[2]].heID = he2_id;

      _halfEdges.push_back(he0);
      _halfEdges.push_back(he1);
      _halfEdges.push_back(he2);

      // verifying twins to each one of the hes
      index_t current_he_ids[3] = {he0_id, he1_id, he2_id};
      index_t v_origins[3] = {tri[0], tri[1], tri[2]};
      index_t v_dests[3] = {tri[1], tri[2], tri[0]};

      for (int j = 0; j < 3; ++j)
      {
        index_t current_id = current_he_ids[j];
        auto key = std::minmax(v_origins[j], v_dests[j]);

        if (edgeMap.find(key) == edgeMap.end())
        {
          // key was not found: no twin yet
          edgeMap[key] = current_id;
        }
        else
        {
          // found twin
          index_t twin_id = edgeMap[key];

          // "sew" twins
          _halfEdges[current_id].twin = twin_id;
          _halfEdges[twin_id].twin = current_id;

          // make edge
          edge e;
          e.id = _edges.size();
          e.twins[0] = current_id;
          e.twins[1] = twin_id;
          _edges.push_back(e);

          // already solved pair
          edgeMap.erase(key);
        }
      }
    }

    std::cerr << "[TOPO] Faces e twins: " << _halfEdges.size() << " HEs, "
              << _edges.size() << " edges. Bordas restantes no mapa: "
              << edgeMap.size() << std::endl;

    // making boundaries: the half edges left on the edgeMap are the twins of the bounds' hes
    std::vector<index_t> boundaryHalfEdges;
    std::map<index_t, index_t> boundaryStartsAt;
    
    for (auto const& [chave, id_h_in] : edgeMap)
    {
      // A semi-aresta interna (h_in) vai do vértice A para o vértice B
      index_t v_A = _halfEdges[id_h_in].vID;
      index_t v_B = _halfEdges[_halfEdges[id_h_in].next].vID;

      index_t id_h_out = _halfEdges.size();
        
      he h_out;
      h_out.vID = v_B; // Nasce em B e vai para A
      h_out.fID = std::numeric_limits<index_t>::max(); // Sem face (é borda)
        
      // Costura os Twins
      h_out.twin = id_h_in;
      _halfEdges[id_h_in].twin = id_h_out;

      // Cria a Aresta (Edge)
      edge e;
      e.id = _edges.size();
      e.twins[0] = id_h_in;
      e.twins[1] = id_h_out;
        
      h_out.eID = e.id;
      _halfEdges[id_h_in].eID = e.id;
        
      _edges.push_back(e);
      _halfEdges.push_back(h_out);
      boundaryHalfEdges.push_back(id_h_out);

      // Mapeamento Vital: A semi-aresta de borda que nasce em B é a h_out
      boundaryStartsAt[v_B] = id_h_out;
    }

    std::cerr << "[TOPO] Boundary HEs criadas: " << boundaryHalfEdges.size() << std::endl;

    // edge map is no need

    // echaining the loops
    for (index_t id_h_out : boundaryHalfEdges)
    {
      // Precisamos saber onde essa h_out termina (que é o vértice A)
      // Para descobrir o vértice A, basta olhar a origem da sua gêmea interna!
      index_t v_A = _halfEdges[_halfEdges[id_h_out].twin].vID;

      // O próximo passo lógico é a semi-aresta de borda que nasce em A
      index_t next_h_out = boundaryStartsAt[v_A];

      _halfEdges[id_h_out].next = next_h_out;
      _halfEdges[next_h_out].prev = id_h_out;
    }

    std::cerr << "[TOPO] Encadeamento de boundaries concluido" << std::endl;

    // --- PASSO 3: Extração dos Boundaries ---
    std::vector<bool> visited(boundaryHalfEdges.size(), false);

    for (size_t i = 0; i < boundaryHalfEdges.size(); ++i)
    {
      if (visited[i]) continue;

      bounds b;
      b.id = _boundaries.size();
      b.heID = boundaryHalfEdges[i];
      _boundaries.push_back(b);

      // Dar a volta no buraco para marcar todas como visitadas
      index_t current_he = boundaryHalfEdges[i];
      size_t guard = 0;
      const size_t maxIter = _halfEdges.size() * 2;
      do
      {
        if (++guard > maxIter)
        {
          std::cerr << "[TOPO][ERRO] Loop infinito detectado na extracao de boundary "
                    << _boundaries.size() - 1
                    << "! HE atual: " << current_he
                    << ", HE inicio: " << boundaryHalfEdges[i]
                    << ", iteracoes: " << guard << std::endl;
          throw std::runtime_error("Loop infinito na extracao de boundaries.");
        }

        // Encontra qual é o índice real dessa current_he dentro do vetor boundaryHalfEdges
        // (Usamos std::find para atualizar o array de visitados corretamente)
        auto it = std::find(boundaryHalfEdges.begin(), boundaryHalfEdges.end(), current_he);
        if (it != boundaryHalfEdges.end())
          visited[std::distance(boundaryHalfEdges.begin(), it)] = true;

        current_he = _halfEdges[current_he].next;
      } while(current_he != boundaryHalfEdges[i]);
    }

    std::cerr << "[TOPO] Boundaries extraidos: " << _boundaries.size() << std::endl;
    std::cerr << "[TOPO] Construcao completa." << std::endl;
  }

  template<typename TopoElement>
  auto begin()
  {
    if constexpr (std::is_same_v<TopoElement, vert>)
      return TopologyIteratorBase<vert_collection>(_vertices, 0);
    else if constexpr (std::is_same_v<TopoElement, he>)
      return TopologyIteratorBase<he_collection>(_halfEdges, 0);
    else if constexpr (std::is_same_v<TopoElement, edge>)
      return TopologyIteratorBase<edge_collection>(_edges, 0);
    else if constexpr (std::is_same_v<TopoElement, face>)
      return TopologyIteratorBase<face_collection>(_faces, 0);
    else if constexpr (std::is_same_v<TopoElement, bounds>)
      return TopologyIteratorBase<bounds_collection>(_boundaries, 0);
  }

  template<typename TopoElement>
  auto end()
  {
    if constexpr (std::is_same_v<TopoElement, vert>)
      return TopologyIteratorBase<vert_collection>(_vertices, _vertices.size());
    else if constexpr (std::is_same_v<TopoElement, he>)
      return TopologyIteratorBase<he_collection>(_halfEdges, _halfEdges.size());
    else if constexpr (std::is_same_v<TopoElement, edge>)
      return TopologyIteratorBase<edge_collection>(_edges, _edges.size());
    else if constexpr (std::is_same_v<TopoElement, face>)
      return TopologyIteratorBase<face_collection>(_faces, _faces.size());
    else if constexpr (std::is_same_v<TopoElement, bounds>)
      return TopologyIteratorBase<bounds_collection>(_boundaries, _boundaries.size());
  }

  template<typename TopoElement>
  auto halfEdge(const index_t i) const
  {
    if constexpr (std::is_same_v<TopoElement, vert>)
      return _vertices[i].heID;
    else if constexpr (std::is_same_v<TopoElement, face>)
      return _faces[i].heID;
    else if constexpr (std::is_same_v<TopoElement, bounds>)
      return _boundaries[i].heID;
    else
      return std::numeric_limits<index_t>::max();
  }

  // Alterar no Data depois
  auto vertexCount() const { return _vertices.size(); }
  auto halfEdgeCount() const { return _halfEdges.size(); }
  auto edgeCount() const { return _edges.size(); }
  auto faceCount() const { return _faces.size(); }
  auto boundaryCount() const { return _boundaries.size(); }

  // next (half edge)
  auto next(const index_t& he) const
  {
    return _halfEdges[he].next;
  }
  
  auto twin(const index_t& he) const
  {
    return _halfEdges[he].twin;
  }
  
  bool hasFace(const index_t& he) const
  {
    return _halfEdges[he].fID != std::numeric_limits<index_t>::max();
  }

  auto getVertex(const index_t& he) const
  {
    return _halfEdges[he].vID;
  }

  auto getEdge(const index_t& he) const
  {
    return _halfEdges[he].eID;
  }

  auto getFace(const index_t& he) const
  {
    return _halfEdges[he].fID;
  }

  auto distance(const index_t& e1, const index_t& e2) const
  {
    auto p1 = _vertices[e1].pos;
    auto p2 = _vertices[e2].pos;
    auto diff = Vec3f{p1.x - p2.x, p1.y - p2.y, p1.z - p2.z};
    return std::sqrt(diff.x*diff.x + diff.y*diff.y + diff.z*diff.z);
  }

  template<typename TopoElement>
  void incidentElement(const index_t& e, ProcFunc f)
  {
    if constexpr (std::is_same_v<TopoElement, edge>)
    {
      auto he0 = halfEdge<vert>(e);

      do
      {
        auto ed = getEdge(he0);
        if (!f(ed)) return; 

        he0 = next(twin(he0));
      } while(he0 != halfEdge<vert>(e));
    }
    else if constexpr (std::is_same_v<TopoElement, face>)
    {
      auto he0 = halfEdge<vert>(e);

      do
      {
        if (hasFace(he0))
        {
          auto fc = getFace(he0);
          if (!f(fc)) return;
        }

        he0 = next(twin(he0));
      } while(he0 != halfEdge<vert>(e));
    }
    else
      return;
  }

  // TODO
  template<typename TopoElement>
  void kRing(const index_t& e, size_t k, ProcFunc f)
  {
    if (k == 0) return; // Um 0-anel não possui faces adjacentes a processar

    // Vetor para evitar ciclos e não reprocessar a mesma face no 2-anel, 3-anel, etc.
    // Inicializado com false para todas as faces.
    std::vector<bool> visited(_faces.size(), false);
      
    // Fila que armazena pares: {ID da Face, Nível Atual do Anel}
    std::queue<std::pair<index_t, size_t>> q; 

    // --- PASSO 1: Semeando a Fila (Nível 1) ---

    if constexpr (std::is_same_v<TopoElement, vert>)
    {
      // Para um vértice, o 1-anel são todas as faces que incidem nele.
      auto he0 = halfEdge<vert>(e);
      auto curr_he = he0;

      do
      {
        if (hasFace(curr_he))
        {
          index_t fID = getFace(curr_he);
          if (!visited[fID])
          {
            visited[fID] = true;
            q.push({fID, 1});
            
            // O ProcFunc retorna um bool. É padrão usá-lo para early-exit.
            // Se f() retornar false, interrompemos a travessia.
            if (!f(fID)) return; 
          }
        }
        // Circula pelo vértice (funciona perfeitamente mesmo com buracos 
        // graças às suas semi-arestas de borda explícitas)
        curr_he = next(twin(curr_he));
      } while (curr_he != he0);
    } 
    else if constexpr (std::is_same_v<TopoElement, face>)
    {
      // Para uma face, o 1-anel são as faces que compartilham suas arestas.
      // Marcamos a própria face como visitada para não adicioná-la aos anéis subsequentes.
      visited[e] = true; 

      auto he0 = halfEdge<face>(e);
      auto curr_he = he0;

      do
      {
        auto t = twin(curr_he); // A gêmea pertence à face vizinha
        if (hasFace(t))
        {
          index_t fID = getFace(t);
          if (!visited[fID])
          {
            visited[fID] = true;
            q.push({fID, 1});
            
            if (!f(fID)) return;
          }
        }
        curr_he = next(curr_he); // Circula pelas arestas da face original
      } while (curr_he != he0);
    }
    else
      return;

    // --- PASSO 2: Expandindo para k > 1 (BFS) ---
    
    while (!q.empty())
    {
      auto [current_face, current_level] = q.front();
      q.pop();

      // Se a face que tiramos da fila já atingiu o limite k, 
      // não precisamos olhar para os vizinhos dela.
      if (current_level >= k) continue;

      // Obtém as faces adjacentes à face atual (compartilham aresta)
      auto he0 = halfEdge<face>(current_face);
      auto curr_he = he0;

      do
      {
        auto t = twin(curr_he);
        if (hasFace(t)) // Ignora buracos (fID == max)
        {
          index_t fID = getFace(t);
          if (!visited[fID])
          {
            visited[fID] = true;
            q.push({fID, current_level + 1});
            
            if (!f(fID)) return;
          }
        }
        curr_he = next(curr_he);
      } while (curr_he != he0);
    }
  }

private:
  vert_collection _vertices;
  he_collection _halfEdges;
  edge_collection _edges;
  face_collection _faces;
  bounds_collection _boundaries;
};

} // end namespace tcii::cg

#endif // __Topology_h