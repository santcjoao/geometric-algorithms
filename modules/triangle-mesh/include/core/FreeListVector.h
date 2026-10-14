#ifndef __FreeListVector_h
#define __FreeListVector_h

// OVERVIEW: MeshDecoration.h
// ========
// Class definition for free list collection.
//
// Authors: Arthur de Andrade, João de Carvalho e Nathan Rezende
// Last revision: 17/06/2026

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <vector>

namespace tcii::cg
{ // begin namespace tcii::cg

/// @brief A collection that manages its own free list.
///        Configured to be used by default in the Topology class, but can be used in other contexts as well.
template <typename T>
class FreeListVector
{
public:
  using value_type = T;
  using index_t = unsigned int;

  FreeListVector() = default;

  explicit FreeListVector(size_t reserve_size)
  {
    reserve(reserve_size);
  }

  T& operator[](index_t index)
  {
    assert(index < _data.size());
    return _data[index];
  }

  const T& operator[](index_t index) const
  {
    assert(index < _data.size());
    return _data[index];
  }

  void reserve(size_t n) { _data.reserve(n); }
  
  size_t size() const { return _data.size(); }

  index_t push_back(const T& value)
  {
    if (!_free_indices.empty())
    {
      index_t free_idx = _free_indices.back();
      _free_indices.pop_back();
      
      _data[free_idx] = value;
      return free_idx;
    }
    else
    {
      _data.push_back(value);
      return static_cast<index_t>(_data.size() - 1);
    }
  }

  void remove(index_t index)
  {
    assert(index < _data.size());
    _free_indices.push_back(index);
  }

  bool is_active(index_t index) const
  {
    for(auto i : _free_indices)
      if(i == index) return false;
    return true;
  }

private:
  std::vector<T> _data;
  std::vector<index_t> _free_indices;
};

} // end namespace tcii::cg

#endif // __FreeListVector_h