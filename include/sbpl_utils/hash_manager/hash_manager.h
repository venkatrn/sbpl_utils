#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

namespace sbpl_utils {
// A generic hash manager for maintaining a two-way mapping between states and unique IDs.
// The template type HashableState must implement the following methods:
//      size_t GetHash() const;
//      bool operator==(const HashableState& other_state) const;
//
// Typical use by an SBPL environment will look as follows:
//      HashManager<HashableState> hash_manager;
//      ....
//      ....
//      std::vector<unsigned int> GetSuccs(unsigned int source_state_id) {
//        const HashableState &source_state = hash_manager.GetState(source_state_id);
//        for (const auto &successor_state : Successors(source_state))
//          successor_ids.push_back(hash_manager.GetStateIDForceful(successor_state));
//        return successor_ids;
//      }
//
// Optionally, if HashableState implementes an ostream<< operator, useful debug information will be printed if a state
// is not found in the hash manager.
//
// Other features include 'updating' a state while preserving its state ID, like in scenarios where we might update the
// continuous coordinates while keeping the discrete coordinates fixed.
//
// Passive consumers (that won't add new states to the hash manager) can use a std::shared_ptr<const HashManager>
// to access the mappings.

// Utility to check if the ostream << operator exists for a given template type.
template <typename T>
class HasOstreamLShift {
  struct no {};

  template <typename T2>
  static decltype(std::declval<std::ostream &>() << std::declval<T2>()) test(
    int);

  template <typename T2>
  static no test(...);

 public:
  enum { value = ! std::is_same<no, decltype(test<T>(0))>::value};
};


// Define a dummy ostream operator for HashableState if one doesn't exist.
template < class HashableState,
           typename std::enable_if < !HasOstreamLShift<HashableState>::value,
                                     int >::type = 0 >
std::ostream &operator<< (std::ostream &stream, const HashableState &state) {
  stream << "<Hash: " << state.GetHash() << ">";
  return stream;
}

template<class HashableState>
class HashManager {
 public:
  HashManager();

  // Return the number of states in the hash manager.
  size_t Size() const;

  bool Exists(const HashableState &hashable_state) const;
  bool Exists(unsigned int state_id) const;

  // Throws error if state does not exist.
  const HashableState &GetState(unsigned int state_id) const;

  // Throws error if state does not exist.
  unsigned int GetStateID(const HashableState &hashable_state) const;

  // Adds a new entry to hash table if one does not exist and returns the state ID.
  // If state already exists, returns existing state ID.
  unsigned int GetStateIDForceful(const HashableState &hashable_state);

  // If state does not already exist in the hash table, run time error is thrown.
  // Preserves the old state ID.
  void UpdateState(const HashableState &hashable_state);

  // Clear the hash manager.
  void Reset();


 private:
  struct HashFunction {
    size_t operator()(const HashableState &hashable_state) const {
      return hashable_state.GetHash();
    }
  };

  std::unordered_map<HashableState, unsigned int, HashFunction>
  state_to_state_id_;
  std::unordered_map<unsigned int, HashableState> state_id_to_state_;
};

///////////////////////////////////////////////////////////////////////////////
// Template Inline Implementation
///////////////////////////////////////////////////////////////////////////////

template<class HashableState>
HashManager<HashableState>::HashManager() {};

template<class HashableState>
size_t HashManager<HashableState>::Size() const {
  return state_to_state_id_.size();
}

template<class HashableState>
bool HashManager<HashableState>::Exists(const HashableState &hashable_state)
const {
  return state_to_state_id_.find(hashable_state) != state_to_state_id_.end();
}

template<class HashableState>
bool HashManager<HashableState>::Exists(unsigned int state_id) const {
  return state_id_to_state_.find(state_id) != state_id_to_state_.end();
}

template<class HashableState>
unsigned int HashManager<HashableState>::GetStateID(const HashableState
                                                    &hashable_state) const {
  const auto it = state_to_state_id_.find(hashable_state);

  if (it == state_to_state_id_.end()) {
    std::ostringstream ss;
    ss << "Asked for non-existent state " << hashable_state <<
       std::endl;
    std::cerr << ss.str();
    throw std::runtime_error(ss.str());
  }

  return it->second;
}

template<class HashableState>
const HashableState &HashManager<HashableState>::GetState(
  unsigned int state_id) const {
  const auto it = state_id_to_state_.find(state_id);

  if (it == state_id_to_state_.end()) {
    std::ostringstream ss;
    ss << "Asked for non-existent state ID: " <<  state_id << std::endl;
    std::cerr << ss.str();
    throw std::runtime_error(ss.str());
  }

  return it->second;
}

// Non-const methods
template<class HashableState>
unsigned int HashManager<HashableState>::GetStateIDForceful(
  const HashableState &hashable_state) {
  const auto it = state_to_state_id_.find(hashable_state);

  if (it != state_to_state_id_.end()) {
    return it->second;
  }

  const int new_state_id = Size();
  state_to_state_id_[hashable_state] = new_state_id;
  state_id_to_state_[new_state_id] = hashable_state;
  return new_state_id;
}

template<class HashableState>
void HashManager<HashableState>::UpdateState(const HashableState
                                             &hashable_state) {
  const auto it = state_to_state_id_.find(hashable_state);

  if (it == state_to_state_id_.end()) {
    std::ostringstream ss;
    ss << "Asked to update a non-existent state " << hashable_state << std::endl;
    std::cerr << ss.str();
    throw std::runtime_error(ss.str());
  }

  const unsigned int old_state_id = it->second;

  state_to_state_id_.erase(it);
  state_to_state_id_[hashable_state] = old_state_id;

  state_id_to_state_[old_state_id] = hashable_state;
}

template<class HashableState>
void HashManager<HashableState>::Reset() {
  state_to_state_id_.clear();
  state_id_to_state_.clear();
}
}  // namespace sbpl_utils





