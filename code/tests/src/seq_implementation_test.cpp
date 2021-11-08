#include <gtest/gtest.h>
extern "C" {
  #include <utils.h>
  #include <sequential_masked_implementation.h>
}

//test MxN matrix
TEST(sequential, t1) {
  struct COO_mtx *mtx_coo_fmt = (COO_mtx*) malloc(sizeof(struct COO_mtx));
  struct CSR_mtx *mtx_csr_fmt = (CSR_mtx*) malloc(sizeof(struct CSR_mtx));
  read_matrix((char**)"tests/data/test1.mtx", mtx_coo_fmt, 0);
  coo_to_csr(mtx_coo_fmt, mtx_csr_fmt);
  float time_taken;
  struct implementation_args func_args;
  func_args.num_of_threads = 0;
  func_args.full_mat = 0;
  func_args.verbose = 0;
  func_args.time_taken = 0;
  float num = triagle_counting_sequential_masked_implementation(mtx_csr_fmt,&func_args);
  EXPECT_EQ(3, num);
};

//test MxN matrix
TEST(sequential, t2) {
  struct COO_mtx *mtx_coo_fmt = (COO_mtx*) malloc(sizeof(struct COO_mtx));
  struct CSR_mtx *mtx_csr_fmt = (CSR_mtx*) malloc(sizeof(struct CSR_mtx));
  read_matrix((char**)"tests/data/test1.mtx", mtx_coo_fmt, 1);
  coo_to_csr(mtx_coo_fmt, mtx_csr_fmt);
  float time_taken;
  struct implementation_args func_args;
  func_args.num_of_threads = 0;
  func_args.full_mat = 1;
  func_args.verbose = 0;
  func_args.time_taken = 0;
  float num = triagle_counting_sequential_masked_implementation(mtx_csr_fmt, &func_args);
  EXPECT_EQ(3, num);
};


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}