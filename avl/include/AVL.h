#ifndef __AVL_h
#define __AVL_h

#include <cstdint>
#include <functional>
#include <initializer_list>
#include <utility>

namespace tcii::avl { // begin namespace tcii::avl

// forward declaration da Tree
template <typename T, typename C> class Tree;

struct TreeNodeBase {
  TreeNodeBase *_parent;
  TreeNodeBase *_childL{};
  TreeNodeBase *_childR{};
  int _height{};

  TreeNodeBase(TreeNodeBase *parent) : _parent{parent} {
    // do nothing
  }

  TreeNodeBase(int height) : _height{height}, _parent{nullptr} {
    // do nothing
  }
  ~TreeNodeBase() = default;

}; // TreeNodeBase

struct AVLHelper {
  static auto height(const TreeNodeBase *node) {
    return node ? node->_height : 0;
  }

  static auto balanceFactor(const TreeNodeBase *node) {
    return node ? height(node->_childR) - height(node->_childL) : 0;
  }

  static void updateHeight(TreeNodeBase *node) {
    if (node) {
      auto hl = height(node->_childL);
      auto hr = height(node->_childR);

      node->_height = (hl > hr ? hl : hr) + 1;
    }
  }

  static void rotateL(TreeNodeBase *&);
  static void rotateR(TreeNodeBase *&);
  static void balance(TreeNodeBase *&);

}; // AVLHelper

struct TreeIteratorBase {
  bool operator==(const TreeIteratorBase &other) const {
    return _node == other._node;
  }

  bool operator!=(const TreeIteratorBase &other) const {
    return !operator==(other);
  }

protected:
  TreeNodeBase *_node;

  TreeIteratorBase(TreeNodeBase *node) : _node{node} {
    // do nothing
  }

  void increment();
  void decrement();

}; // TreeIteratorBase

template <typename T> class TreeNode : public TreeNodeBase {
public:
  T _value;

  TreeNode(const T &value, TreeNodeBase *parent)
      : TreeNodeBase{parent}, _value{value} {
    // do nothing
  }

  ~TreeNode() {
    delete (TreeNode *)_childL;
    delete (TreeNode *)_childR;
  }

}; // TreeNode

template <typename T> class TreeIterator : public TreeIteratorBase {
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;
  using pointer = T *;
  using reference = T &;
  using difference_type = std::ptrdiff_t;

  TreeIterator(TreeNode<T> *node) : TreeIteratorBase{node} {
    // do nothing
  }

  auto &operator*() const { return ((TreeNode<T> *)_node)->_value; }

  auto operator->() const { return &operator*(); }

  auto &operator++() {
    increment();
    return *this;
  }

  auto operator++(int) {
    auto temp = *this;

    increment();
    return temp;
  }

  auto operator--() {
    decrement();
    return *this;
  }

  auto operator--(int) {
    auto temp = *this;

    decrement();
    return temp;
  }

  template <typename U, typename C> friend class Tree;
}; // TreeIterator

template <typename T, typename C = std::less<T>> class Tree {
public:
  using iterator = TreeIterator<T>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using IterFunc = void(const T &);

  Tree(C comp = C{}) : _comp{comp} {
    _header._childL = _header._childR = &_header;
  }

  ~Tree() { clear(); }

  void clear() {
    if (_header._parent) {
      delete static_cast<Node *>(_header._parent);
      _header._parent = nullptr;
      _header._childL = _header._childR = &_header;
      _nodeCount = 0;
    }
  }

  std::pair<iterator, bool> insert(const T &);

  void insert(std::initializer_list<T> list) {
    for (const auto &value : list)
      insert(value);
  }

  bool erase(const T &value);

  auto size() const { return _nodeCount; }

  auto empty() const { return !_header._parent; }

  void iterate(IterFunc func) const {
    iterate(static_cast<Node *>(_header._parent), func);
  }

  auto height() const { return AVLHelper::height(_header._parent); }

  iterator begin() const { return (Node *)_header._childL; }

  iterator end() const { return (Node *)&_header; }

  iterator find(const T &) const;

  auto contains(const T &value) const { return find(value) != end(); }

  auto rbegin() const {
    return reverse_iterator(iterator(end()->_node->_childR));
  }
  auto rend() const { return reverse_iterator(begin()); }

private:
  using Node = TreeNode<T>;

  TreeNodeBase _header{-1};
  unsigned _nodeCount{};
  C _comp;

  static Node *insert(Node *, TreeNodeBase *, const T &, Node *&, C &);
  static void iterate(Node *, IterFunc);

}; // Tree

template <typename T, typename C>
auto Tree<T, C>::insert(const T &value)
    -> std::pair<Tree<T, C>::iterator, bool> {
  Node *valueNode;
  auto root = insert(static_cast<Node *>(_header._parent), &_header, value,
                     valueNode, _comp);
  auto success = bool(root);

  if (success) {
    _header._parent = root;
    _nodeCount++;

    if (_header._childL == &_header ||
        _comp(value, static_cast<Node *>(_header._childL)->_value))
      _header._childL = valueNode;

    if (_header._childR == &_header ||
        _comp(static_cast<Node *>(_header._childR)->_value, value))
      _header._childR = valueNode;
  }
  return {valueNode, success};
}

template <typename T, typename C>
auto Tree<T, C>::find(const T &value) const -> Tree<T, C>::iterator {
  auto node = static_cast<Node *>(_header._parent);

  while (node) {
    if (_comp(value, node->_value))
      node = static_cast<Node *>(node->_childL);
    else if (_comp(node->_value, value))
      node = static_cast<Node *>(node->_childR);
    else
      return iterator{node};
  }
  return end();
}

template <typename T, typename C>
TreeNode<T> *Tree<T, C>::insert(Tree<T, C>::Node *node, TreeNodeBase *parent,
                                const T &value, Tree<T, C>::Node *&valueNode,
                                C &comp) {
  if (!node)
    return valueNode = new Node{value, parent};
  if (comp(value, node->_value)) {
    if (auto l = insert((Node *)node->_childL, node, value, valueNode, comp))
      node->_childL = l;
    else
      return nullptr;
  } else if (comp(node->_value, value)) {
    if (auto r = insert((Node *)node->_childR, node, value, valueNode, comp))
      node->_childR = r;
    else
      return nullptr;
  } else
    return (valueNode = node), nullptr;

  TreeNodeBase *temp{node};

  AVLHelper::balance(temp);
  return (Node *)temp;
}

template <typename T, typename C> bool Tree<T, C>::erase(const T &value) {
  auto target_iterator = find(value);

  if (target_iterator == end())
    return false;

  auto node = static_cast<Node *>(target_iterator._node);

  TreeNodeBase *rebalanceStartNode = nullptr;

  if (_header._childL == node) {
    auto it = iterator{node};
    ++it;
    _header._childL = it != end() ? it._node : &_header;
  }
  if (_header._childR == node) {
    auto it = iterator{node};
    --it;
    _header._childR = it != end() ? it._node : &_header;
  }

  if (node->_childL && node->_childR) {
    auto successor = node->_childR;
    while (successor->_childL)
      successor = static_cast<Node *>(successor->_childL);

    // Swap
    if (successor->_parent != node) {
      rebalanceStartNode = successor->_parent;

      auto successorParent = successor->_parent;
      successorParent->_childL = successor->_childR;

      if (successor->_childR)
        successor->_childR->_parent = successorParent;

      successor->_childR = node->_childR;
      successor->_childR->_parent = successor;
    } else
      rebalanceStartNode = successor;

    successor->_childL = node->_childL;
    if (successor->_childL)
      successor->_childL->_parent = successor;
    successor->_parent = node->_parent;

    auto &ref = (node == _header._parent) // alteração
                    ? _header._parent
                    : (node->_parent->_childL == node ? node->_parent->_childL
                                                      : node->_parent->_childR);
    ref = successor;
  } else {
    rebalanceStartNode = node->_parent;

    auto child = node->_childL ? node->_childL : node->_childR;
    auto &ref = (node == _header._parent) // alteração
                    ? _header._parent
                    : (node->_parent->_childL == node ? node->_parent->_childL
                                                      : node->_parent->_childR);
    ref = child;
    if (child)
      child->_parent = node->_parent;
  }

  node->_parent = node->_childL = node->_childR = nullptr;
  delete node;
  _nodeCount--;

  // Rebalance
  auto current = rebalanceStartNode;
  while (current && current->_height != -1) // alteração
  {
    auto next = static_cast<Node *>(current->_parent);

    auto &link =
        (current == _header._parent) // alteração
            ? _header._parent
            : (next->_childL == current ? next->_childL : next->_childR);
    AVLHelper::balance(link);

    current = next;
  }

  return true;
}

template <typename T, typename C>
void Tree<T, C>::iterate(Node *node, IterFunc func) {
  if (node) {
    iterate((Node *)node->_childL, func);
    func(node->_value);
    iterate((Node *)node->_childR, func);
  }
}

} // end namespace tcii::avl

#endif // __AVL_h