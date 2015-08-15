#pragma once

#include <cstddef>
#include <iostream>
#include <vector>

namespace sbpl_utils {

///////////////////////////////////////////////////////////////////////////////
// StateXY
///////////////////////////////////////////////////////////////////////////////

class StateXY {
 public:
  StateXY();
  StateXY(int x, int y);
  const int &x() const {
    return x_;
  }
  const int &y() const {
    return y_;
  }
  bool operator==(const StateXY &other) const;
  size_t GetHash() const;
 private:
  int x_;
  int y_;
};

// Optionally implement the ostream<< operator to see useful debug results when using the hash manager.
std::ostream &operator<< (std::ostream &stream, const StateXY &state);

///////////////////////////////////////////////////////////////////////////////
// StateXYTheta
///////////////////////////////////////////////////////////////////////////////

class StateXYTheta {
 public:
  StateXYTheta();
  StateXYTheta(int x, int y, int theta);
  const int &x() const {
    return x_;
  }
  const int &y() const {
    return y_;
  }
  const int &theta() const {
    return theta_;
  }
  bool operator==(const StateXYTheta &other) const;
  size_t GetHash() const;
 private:
  int x_;
  int y_;
  int theta_;
};

// Optionally implement the ostream<< operator to see useful debug results when using the hash manager.
std::ostream &operator<< (std::ostream &stream, const StateXYTheta &state);

///////////////////////////////////////////////////////////////////////////////
// StateDiscVector
///////////////////////////////////////////////////////////////////////////////

class StateDiscVector {
 public:
  StateDiscVector();
  StateDiscVector(const std::vector<int> &coords);
  const std::vector<int> &coords() const {
    return coords_;
  }
  bool operator==(const StateDiscVector &other) const;
  size_t GetHash() const;
 private:
  std::vector<int> coords_;
};
}  // namespace sbpl_utils
