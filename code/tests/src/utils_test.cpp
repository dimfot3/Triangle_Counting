#include <gtest/gtest.h>

extern "C" {
  #include <utils.h>
}
//test MxN matrix
TEST(coo_to_csr, t1) {
  COO_mtx mtx1;
  CSR_mtx mtx2;
  int row_idx[] = {0 ,0, 1, 1, 2, 2, 2, 3};
  int col_idx[] = {0 ,1, 1, 3, 2, 3, 4, 5};
  int new_col_idx[] = { 0,  1,  1,  3,  2,  3,  4,  5};
  int new_row_idx[] = {0,  2,  4,  7,  8 };
  mtx1.row_idx = row_idx;
  mtx1.col_idx = col_idx;
  mtx1.nz_size = 8;
  mtx1.mat_size = 5;
  coo_to_csr(&mtx1, &mtx2);
  int valid = 1;
  for(int i = 0; i < 8; i++)
  {
    valid*= new_col_idx[i] == mtx2.col_idx[i];
  }
  for(int i = 0; i < 5; i++)
  {
    valid*= new_row_idx[i] == mtx2.row_idx[i];
  }
  EXPECT_EQ(1, valid);
};
//test NxN matrix
TEST(coo_to_csr, t2) {
  COO_mtx mtx1;
  CSR_mtx mtx2;
  int row_idx[] = {0, 1, 2, 3};
  int col_idx[] = {0, 1, 2, 1};
  int new_col_idx[] = {0, 1, 2, 1};
  int new_row_idx[] = {0, 1, 2, 3, 4};
  mtx1.row_idx = row_idx;
  mtx1.col_idx = col_idx;
  mtx1.nz_size = 4;
  mtx1.mat_size = 4;
  coo_to_csr(&mtx1, &mtx2);
  int valid = 1;
  for(int i = 0; i < 4; i++)
  {
    valid*= new_col_idx[i] == mtx2.col_idx[i];
  }
  for(int i = 0; i < 5; i++)
  {
    valid*= new_row_idx[i] == mtx2.row_idx[i];
  }
  EXPECT_EQ(1, valid);
};



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}