#ifndef __ParticleBuffer_h
#define __ParticleBuffer_h

#include "graphics/Bounds3.h"
#include "physx/Renderer.h"
#include "util/SharedObject.h"
#include "util/SoA.h"
#include <tuple>

namespace tcii::physx
{ // begin namespace tcii::physx

using namespace cg;

struct DefaultAllocator
{
  template <typename T>
  static T *allocate(size_t n)
  {
    return new T[n];
  }

  template <typename T>
  static void free(T *ptr)
  {
    delete[] ptr;
  }
};

// Forward declaration
template <typename... Fields> class ParticleSystem;
template <typename... Fields> class ParticleBuffer;

/// @brief Proxy para uma partícula individual (iterador)
///        Fornece acesso referencial às propriedades da partícula.
/// @tparam ...Fields
template <typename... Fields> class ParticleRef {
public:
  using Buffer = ParticleBuffer<Fields...>;

  ParticleRef(Buffer *buffer, unsigned idx) : _buffer(buffer), _idx(idx) {}

  const Vec3f &position() const { return _buffer->position(_idx); }

  // Acesso genérico a qualquer campo por índice
  // (0 = posição, 1 = primeiro campo, ...)
  template <size_t I> auto &get() const
  {
    return _buffer->template get<I>(_idx);
  }

  bool operator!=(const ParticleRef &other) const
  {
    return _buffer != other._buffer || _idx != other._idx;
  }

  ParticleRef &operator++()
  {
    ++_idx;
    return *this;
  }

  ParticleRef operator++(int)
  {
    ParticleRef tmp = *this;
    ++_idx;
    return tmp;
  }

private:
  Buffer *_buffer;
  unsigned _idx;
};

/// @brief Armazena partículas em SoA e gerencia os atributos
/// @tparam ...Fields
template <typename... Fields> class ParticleBuffer : public SharedObject {
public:
  using TupleType = std::tuple<Vec3f, Fields...>;
  using SoAType = SoA<DefaultAllocator, unsigned, Vec3f, Fields...>;
  using iterator = ParticleRef<Fields...>;
  using const_iterator = iterator; // Iteradores const = não const (pra não
                                   // precisar implementar um ConstParticleRef)

  ParticleBuffer(unsigned capacity) : _data(capacity), _particleCount(0) {}

  auto particleCount() const { return _particleCount; }
  auto capacity() const { return _data.size(); }

  void add(const Vec3f &p, const Fields &...fields)
  {
    if (_particleCount >= capacity())
      return;
    _data.set(_particleCount, p, fields...);
    ++_particleCount;
  }

  void clear() { _particleCount = 0; }

  Vec3f &position(unsigned i) { return _data.template get<0>(i); }
  const Vec3f &position(unsigned i) const { return _data.template get<0>(i); }

  template <size_t I>
  auto &get(unsigned i)
  {
    return _data.template get<I>(i);
  }

  template <size_t I>
  const auto &get(unsigned i) const
  {
    return _data.template get<I>(i);
  }

  Bounds3f bounds() const;

  // Renderização não utilizada
  void render(Renderer &) const { /* Do nothing */ };

  // Iteradores
  iterator begin() { return iterator(this, 0); }
  iterator end() { return iterator(this, _particleCount); }
  const_iterator begin() const
  {
    return const_iterator(const_cast<ParticleBuffer *>(this), 0);
  }
  const_iterator end() const
  {
    return const_iterator(const_cast<ParticleBuffer *>(this), _particleCount);
  }

private:
  SoAType _data;
  unsigned _particleCount;

  friend class ParticleSystem<Fields...>;
};

template <typename... Fields>
Bounds3f ParticleBuffer<Fields...>::bounds() const
{
  Bounds3f b;
  for (unsigned i = 0; i < _particleCount; ++i)
    b.inflate(position(i));
  return b;
}

} // end namespace tcii::physx

#endif // __ParticleBuffer_h