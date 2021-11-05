#include <gtest/gtest.h>
extern "C" {
  #include <utils.h>
  #include <sequential_masked_implementation.h>
}

//test MxN matrix
TEST(sequential, t1) {
  struct COO_mtx *mtx_coo_fmt = (COO_mtx*) malloc(sizeof(struct COO_mtx));
  struct CSR_mtx *mtx_csr_fmt = (CSR_mtx*) malloc(sizeof(struct CSR_mtx));
  read_matrix((char**)"../data/test.mtx", mtx_coo_fmt, 0);
  coo_to_csr(mtx_coo_fmt, mtx_csr_fmt);
  float num = triagle_counting_sequential_masked_implementation(mtx_csr_fmt);
  EXPECT_EQ(3, num);
};

//test MxN matrix
TEST(sequential, t2) {
  struct COO_mtx *mtx_coo_fmt = (COO_mtx*) malloc(sizeof(struct COO_mtx));
  struct CSR_mtx *mtx_csr_fmt = (CSR_mtx*) malloc(sizeof(struct CSR_mtx));
  read_matrix((char**)"../data/test2.mtx", mtx_coo_fmt, 0);
  coo_to_csr(mtx_coo_fmt, mtx_csr_fmt);
  float num = triagle_counting_sequential_masked_implementation(mtx_csr_fmt);
  EXPECT_EQ(12, num);
};

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}