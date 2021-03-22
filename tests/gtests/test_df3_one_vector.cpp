#include <gtest/gtest.h>

#include <df1b2fun.h>
#include <df3fun.h>

class test_df3_one_vector: public ::testing::Test {};

TEST_F(test_df3_one_vector, default_constructor)
{
  df3_one_vector x;
  ASSERT_EQ(x.indexmin(), 0);
  ASSERT_EQ(x.indexmax(), -1);
  dvector v = value(x);
  ASSERT_EQ(v.indexmin(), 1);
  ASSERT_EQ(v.indexmax(), 0);
}
TEST_F(test_df3_one_vector, size_constructor)
{
  df3_one_vector x(1, 5);
  ASSERT_EQ(x.indexmin(), 1);
  ASSERT_EQ(x.indexmax(), 5);
  x.initialize();
  dvector v = value(x);
  ASSERT_EQ(v.indexmin(), 1);
  ASSERT_EQ(v.indexmax(), 5);
  ASSERT_DOUBLE_EQ(0, v(1));
  ASSERT_DOUBLE_EQ(0, v(2));
  ASSERT_DOUBLE_EQ(0, v(3));
  ASSERT_DOUBLE_EQ(0, v(4));
  ASSERT_DOUBLE_EQ(0, v(5));
  x.deallocate();
  ASSERT_EQ(x.indexmin(), 0);
  ASSERT_EQ(x.indexmax(), -1);
}
TEST_F(test_df3_one_vector, copy)
{
  df3_one_vector x(1, 5);
  df3_one_vector y(x);

  ASSERT_TRUE(&x[1] == &y(1));
  ASSERT_TRUE(&x[2] == &y(2));
  ASSERT_TRUE(&x[3] == &y(3));
  ASSERT_TRUE(&x[4] == &y(4));
  ASSERT_TRUE(&x[5] == &y(5));
}
