#include <gtest/gtest.h>
extern "C" {
  #include <utils.h>
  #include <sequential_masked_implementation.h>
}

//test MxN matrix
TEST(sequential, t1) {
  struct COO_mtx *mtx_coo_fmt = (COO_mtx*) malloc(sizeof(struct COO_mtx));
  struct CSR_mtx *mtx_csr_fmt = (CSR_mtx*) malloc(sizeof(struct CSR_mtx));
  read_matrix((char**)"../data/test1.mtx", mtx_coo_fmt, 0);
  coo_to_csr(mtx_coo_fmt, mtx_csr_fmt);
  float time_taken;
  float num = triagle_counting_sequential_masked_implementation(mtx_csr_fmt,&time_taken,0);
  EXPECT_EQ(3, num);
};

//test MxN matrix
TEST(sequential, t2) {
  struct COO_mtx *mtx_coo_fmt = (COO_mtx*) malloc(sizeof(struct COO_mtx));
  struct CSR_mtx *mtx_csr_fmt = (CSR_mtx*) malloc(sizeof(struct CSR_mtx));
  read_matrix((char**)"../data/test1.mtx", mtx_coo_fmt, 1);
  coo_to_csr(mtx_coo_fmt, mtx_csr_fmt);
  float time_taken;
  float num = triagle_counting_sequential_masked_implementation(mtx_csr_fmt,&time_taken,0);
  EXPECT_EQ(18, num);
};


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}