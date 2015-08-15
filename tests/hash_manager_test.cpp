#include<sbpl_utils/examples/hashable_states.h>
#include <sbpl_utils/hash_manager/hash_manager.h>

#include <gtest/gtest.h>

#include <stdexcept>

using namespace sbpl_utils;

TEST(HashManagerTests, StateXYTest) {
  HashManager<StateXY> hash_manager;

  StateXY s1(1, 2);
  StateXY s2(1, 2);
  StateXY s3(1, 1);
  StateXY s4(100, 100);

  const int id1 = hash_manager.GetStateIDForceful(s1);
  const int id2 = hash_manager.GetStateIDForceful(s2);
  const int id3 = hash_manager.GetStateIDForceful(s3);

  EXPECT_EQ(id1, 0);
  // s1 and s2 are identical.
  EXPECT_EQ(id1, id2);
  // s1 and s3 are not identical.
  EXPECT_NE(id1, id3);

  EXPECT_NO_THROW(hash_manager.GetState(0));
  // Only two IDs should exist at this point (0 and 1).
  EXPECT_THROW(hash_manager.GetState(2), std::runtime_error);

  EXPECT_NO_THROW(hash_manager.GetStateID(s1));
  // We never added s4 to the hash manager.
  EXPECT_THROW(hash_manager.GetStateID(s4), std::runtime_error);

  // Now 'update' state s3 and check if the ID remains the same.
  StateXY s5(1, 1);  // In theory, this could have different 'continuous coordinates' from what s3 has.
  hash_manager.UpdateState(s5);
  unsigned int new_id;
  EXPECT_NO_THROW(new_id = hash_manager.GetStateID(s5));
  EXPECT_EQ(new_id, id3);

  // We should have 2 unique states at this point.
  EXPECT_EQ(hash_manager.Size(), 2);

  // Reset the hash manager and check if there are no states.
  hash_manager.Reset();
  EXPECT_EQ(hash_manager.Size(), 0);
}

TEST(HashManagerTests, StateDiscVectorTest) {
  HashManager<StateDiscVector> hash_manager;

  StateDiscVector s1({10, 4, 3});
  StateDiscVector s2({10, 4, 3});
  StateDiscVector s3({10, 1, 5});
  StateDiscVector s4({100, 100, 100});

  const int id1 = hash_manager.GetStateIDForceful(s1);
  const int id2 = hash_manager.GetStateIDForceful(s2);
  const int id3 = hash_manager.GetStateIDForceful(s3);

  EXPECT_EQ(id1, 0);
  EXPECT_EQ(id1, id2);
  EXPECT_NE(id1, id3);

  EXPECT_NO_THROW(hash_manager.GetState(0));
  EXPECT_THROW(hash_manager.GetState(2), std::runtime_error);

  EXPECT_NO_THROW(hash_manager.GetStateID(s1));
  EXPECT_THROW(hash_manager.GetStateID(s4), std::runtime_error);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
