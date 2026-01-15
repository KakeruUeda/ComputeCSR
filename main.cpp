/**
 * @brief Simple implementation for converting COO format to CSR format
 * @ref Timony A. Davis, Direct Methods for Sparse Linear Systems, SIAM.
 */
#include <iostream>
#include <vector>
#include <cassert>

class CsrMatrix
{
public:
  CsrMatrix(int m, int n, int nnz)
  {
    m_ = m;
    n_ = n;
    nnz_ = nnz;

    row_ptr_.resize(m_ + 1, 0);
    column_ind_.resize(nnz_, 0);
    value_.resize(nnz_, 0);
  }

  ~CsrMatrix() = default;

  const std::vector<int> &get_row_ptr() const
  {
    return row_ptr_;
  }

  const std::vector<int> &get_column_ind() const
  {
    return column_ind_;
  }
  
  const std::vector<double> &get_value() const
  {
    return value_;
  }
  
  int get_nnz() const
  {
    return nnz_;
  }

  void compress(std::vector<int> &r,
                std::vector<int> &c,
                std::vector<double> &v)
  {
    assert(nnz_ == r.size() && r.size() == c.size() && c.size() == v.size());

    std::vector<int> w(m_, 0);

    for (int i = 0; i < nnz_; ++i)
    {
      w[r[i]]++;
    }

    // Cumulative sum
    row_ptr_[0] = 0;
    for (int i = 0; i < m_; ++i)
    {
      row_ptr_[i + 1] = row_ptr_[i] + w[i];
    }

    for (int i = 0; i < m_; ++i)
    {
      w[i] = row_ptr_[i];
    }

    for (int i = 0; i < nnz_; ++i)
    {
      int j = w[r[i]]++;
      column_ind_[j] = c[i];
      value_[j] = v[i];
    }
  }

private:
  int m_;
  int n_;
  int nnz_;

  std::vector<int> row_ptr_;
  std::vector<int> column_ind_;
  std::vector<double> value_;
};

void print_results(const CsrMatrix &csr)
{
  std::cout << "CSR Format:" << std::endl;
  std::cout << "row_ptr: ";
  for (int i = 0; i < csr.get_row_ptr().size(); ++i)
  {
    std::cout << csr.get_row_ptr()[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "column_ind: ";
  for (int i = 0; i < csr.get_nnz(); ++i)
  {
    std::cout << csr.get_column_ind()[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "value: ";
  for (int i = 0; i < csr.get_nnz(); ++i)
  {
    std::cout << csr.get_value()[i] << " ";
  }
  std::cout << std::endl;
}

int main()
{
  // Example matrix
  // |1.1, 0.0, 5.9, 0.0, 0.0|
  // |2.0, 0.0, 0.0, 1.0, 0.0|
  // |0.0, 0.0, 7.2, 0.0, 2.0|

  // Matrix info
  int m = 3; // Num of rows
  int n = 5; // Num of columns

  // Create COO format
  std::vector<int> row_ind = {0, 0, 1, 1, 2, 2};
  std::vector<int> column_ind = {0, 2, 0, 3, 2, 4};
  std::vector<double> value = {1.1, 5.9, 2.0, 1.0, 7.2, 2.0};

  int nnz = value.size();

  // Create CSR
  CsrMatrix csr(m, n, nnz);
  csr.compress(row_ind, column_ind, value);

  // Print CSR entries
  print_results(csr);

  return 0;
}
