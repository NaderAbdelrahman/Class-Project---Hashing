#ifndef LINEAR_PROBING_H
#define LINEAR_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>


namespace {

// Internal method to test if a positive number is prime.
bool LinIsPrime(size_t n) {
  if( n == 2 || n == 3 )
    return true;
  
  if( n == 1 || n % 2 == 0 )
    return false;
  
  for( unsigned int i = 3; i * i <= n; i += 2 )
    if( n % i == 0 )
      return false;
  
  return true;
}


// Internal method to return a prime number at least as large as n.
int LinNextPrime(size_t n) {
  if (n % 2 == 0)
    ++n;  
  while (!LinIsPrime(n)) n += 2;  
  return n;
}

}  // namespace


// Quadratic probing implementation.
template <typename HashedObj>
class LinearTable {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit LinearTable(size_t size = 101) : array_(LinNextPrime(size))
    { MakeEmpty(); }
  
  // bool Contains(const HashedObj & x) const {
  //   return IsActive(FindPos(x));
  // }
  bool Contains(const HashedObj & x) const {
    bool btmp;
    unsigned int tmp = number_of_collisions_;
    btmp = IsActive(FindPos(x));
    number_of_probes_ = number_of_collisions_ - tmp + 1;
    return btmp;
  }
  
  void MakeEmpty() {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  bool Insert(const HashedObj & x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos)){

      return false;
    }
    
    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;
    
    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();    
    return true;
  }
    
  bool Insert(HashedObj && x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }
  unsigned int NumberOfItems(){
    return current_size_;
  }
  unsigned int TableSize(){
    return array_.size();
  }
  float LoadFactor(){
    return static_cast<float>(current_size_) / static_cast<float>(array_.size());
  }
  unsigned int NumberOfCollisions(){
    return number_of_collisions_;  
  }
  float AverageNumberOfCollisions(){
    return static_cast<float>(number_of_collisions_) / static_cast<float>(current_size_);
  }
  unsigned int NumberOfProbes(){
    return number_of_probes_;
  }

 private:        
  struct HashEntry {
    HashedObj element_;
    EntryType info_;
    
    HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
    :element_{e}, info_{i} { }
    
    HashEntry(HashedObj && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{ i } {}
  };
    
  std::vector<HashEntry> array_;
  size_t current_size_;
  mutable unsigned int number_of_collisions_ = 0;
  mutable unsigned int number_of_probes_ = 0;

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }

  size_t FindPos(const HashedObj & x) const {
    // size_t offset = 1;
    size_t current_pos = InternalHash(x);
    while (array_[current_pos].info_ != EMPTY &&
	   array_[current_pos].element_ != x) {
      current_pos++;
      ++number_of_collisions_;
      if (current_pos >= array_.size())         
	     current_pos -= array_.size();
    }
        return current_pos;
  }

  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(LinNextPrime(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	Insert(std::move(entry.element_));
  }

  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size( );
  }
};

#endif  // LINEAR_PROBING_H
