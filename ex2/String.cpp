#include "String.h"

namespace tcii::ex
{ // begin namespace tcii::ex


//////////////////////////////////////////////////////////
//
// String implementation
// ======
void
String::copy(const char* const s, unsigned size)
{
  if ((_size = size) > 0)
  {
    if (isShort())
      _data = _buffer;
    else
    {
      _capacity = (_size / maxBuffer + 1) * maxBuffer;
      _data = new char[_capacity + 1];
    }
    strncpy(_data, s, _size + 1);
  }
}

void
String::move(String& other) noexcept
{
  _size = other._size;
  if (isShort())
  {
    _data = _buffer;
    strncpy(_data, other._data, _size + 1);
  }
  else
  {
    _capacity = other._capacity;
    _data = other._data;
  }
  other._size = 0;
}

// insert your code here

} // end namespace tcii::ex
