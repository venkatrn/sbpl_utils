#include <sbpl_utils/examples/hashable_states.h>

#include <iostream>

using std::hash;

namespace sbpl_utils {

///////////////////////////////////////////////////////////////////////////////
// StateXY
///////////////////////////////////////////////////////////////////////////////

StateXY::StateXY() : x_(0), y_(0) {}
StateXY::StateXY(int x, int y) : x_(x), y_(y) {}
bool StateXY::operator==(const StateXY &other) const {
  return x_ == other.x() && y_ == other.y();
}
size_t StateXY::GetHash() const {
  return hash<int>()(x_) ^ hash<int>()(y_);
}
std::ostream &operator<< (std::ostream &stream, const StateXY &state) {
  stream << "(" << state.x() << ", " << state.y() << ")";
  return stream;
}

///////////////////////////////////////////////////////////////////////////////
// StateXYTheta
///////////////////////////////////////////////////////////////////////////////

StateXYTheta::StateXYTheta() : x_(0), y_(0), theta_(0) {}
StateXYTheta::StateXYTheta(int x, int y, int theta) : x_(0), y_(y),
  theta_(theta) {}
bool StateXYTheta::operator==(const StateXYTheta &other) const {
  return x_ == other.x() && y_ == other.y() && theta_ == other.theta();
}
size_t StateXYTheta::GetHash() const {
  return hash<int>()(x_) ^ hash<int>()(y_) ^ hash<int>()(theta_);
}
std::ostream &operator<< (std::ostream &stream, const StateXYTheta &state) {
  stream << "(" << state.x() << ", " << state.y() <<  ", " << state.theta() << ")";
  return stream;
}

///////////////////////////////////////////////////////////////////////////////
// StateDiscVector
///////////////////////////////////////////////////////////////////////////////

StateDiscVector::StateDiscVector() : coords_(0) {}
StateDiscVector::StateDiscVector(const std::vector<int> &coords) : coords_
  (coords) {}
bool StateDiscVector::operator==(const StateDiscVector &other) const {
  return coords_ == other.coords();
}
size_t StateDiscVector::GetHash() const {
  size_t hash_value = 0;

  for (const auto &coord : coords_) {
    hash_value ^= hash<int>()(coord);
  }

  return hash_value;
}
}  // namespace sbpl_utils

