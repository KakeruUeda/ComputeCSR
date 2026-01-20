/**
 * @brief Simple implementation for converting COO format to CSR format
 * @ref Timony A. Davis, Direct Methods for Sparse Linear Systems, SIAM.
 */

#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>

class CsrMatrix
{
public:
  CsrMatrix(int m, int n, int nnz)
  {
    m_ = m;
    n_ = n;
    nnz_ = nnz;

    row_ptr_.resize(m_ + 1, 0);
    map2csr_.resize(nnz_, 0);
    column_ind_.resize(nnz_, 0);
    values_.resize(nnz_, 0);
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
    return values_;
  }

  const std::vector<int> &get_map2csr() const
  {
    return map2csr_;
  }

  int get_nnz() const
  {
    return nnz_;
  }

  void compress(const std::vector<int> &r,
                const std::vector<int> &c,
                const std::vector<double> &v)
  {
    assert(static_cast<std::size_t>(nnz_) == r.size() && r.size() == c.size() && c.size() == v.size());

    std::vector<int> row_buffer(m_, 0);
    for (int i = 0; i < nnz_; ++i)
    {
      assert(r[i] >= 0 && r[i] < m_);
      assert(c[i] >= 0 && c[i] < n_);
      row_buffer[r[i]]++;
    }

    row_ptr_[0] = 0;
    for (int i = 0; i < m_; ++i)
    {
      row_ptr_[i + 1] = row_ptr_[i] + row_buffer[i];
    }

    assert(row_ptr_[m_] == nnz_);

    std::fill(row_buffer.begin(), row_buffer.end(), 0);

    for (int i = 0; i < nnz_; ++i)
    {
      map2csr_[i] = row_ptr_[r[i]] + row_buffer[r[i]];
      column_ind_[map2csr_[i]] = c[i];
      values_[map2csr_[i]] = v[i];
      row_buffer[r[i]]++;
    }
  }

  // Reset using values in the original COO input order.
  void reset(const std::vector<double> &coo_values)
  {
    assert(coo_values.size() == static_cast<std::size_t>(nnz_));
    for (int i = 0; i < nnz_; ++i)
    {
      values_[map2csr_[i]] = coo_values[i];
    }
  }

private:
  int m_;
  int n_;
  int nnz_;

  std::vector<int> row_ptr_;
  std::vector<int> map2csr_;
  std::vector<int> column_ind_;
  std::vector<double> values_;
};

void print_results(const CsrMatrix &csr)
{
  const auto &row_ptr = csr.get_row_ptr();
  const auto &col = csr.get_column_ind();
  const auto &val = csr.get_value();

  std::cout << "------ CSR entries ------\n";

  std::cout << "row_ptr: ";
  for (int x : row_ptr)
    std::cout << std::setw(5) << x;
  std::cout << "\n";

  std::cout << "col:     ";
  for (int i = 0; i < csr.get_nnz(); ++i)
    std::cout << std::setw(5) << col[i];
  std::cout << "\n";

  std::cout << "val:     ";
  std::cout << std::fixed << std::setprecision(1);
  for (int i = 0; i < csr.get_nnz(); ++i)
    std::cout << std::setw(5) << val[i];
  std::cout << "\n\n";
}

int main()
{
  // ============================================
  // Example matrix
  // |1.1, 0.0, 5.9, 0.0, 0.0|
  // |2.0, 0.0, 0.0, 1.0, 0.0|
  // |0.0, 0.0, 7.2, 0.0, 2.0|

  // COO format
  // row index:      {  0,   0,   1,   1,   2,  2}
  // column index:   {  0,   2,   0,   3,   2,  4}
  // nonzero value:  {1.1, 5.9, 2.0, 1.0, 7.2, 2.0}

  // CSR format
  // row pointer:    {  0,   2,   4,   6}
  // column index:   {  0,   2,   0,   3,   2,   4}
  // nonzero value:  {1.1, 5.9, 2.0, 1.0, 7.2, 2.0}
  // ===========================================

  // Matrix info
  int m = 3; // Num of rows
  int n = 5; // Num of columns

  // Create COO (unsorted)
  std::vector<int> row_ind = {2, 0, 1, 2, 1, 0};
  std::vector<int> column_ind = {4, 0, 3, 2, 0, 2};
  std::vector<double> value = {2.0, 1.1, 1.0, 7.2, 2.0, 5.9};

  int nnz = static_cast<int>(value.size());

  // Create CSR
  CsrMatrix csr(m, n, nnz);
  csr.compress(row_ind, column_ind, value);

  // Print CSR entries
  print_results(csr);

  // Update values
  std::vector<double> updated_values = {8.0, 1.4, 3.0, 3.2, 0.5, 2.0};
  csr.reset(updated_values);

  std::cout << "After updating the nonzero values:\n\n";

  // Print CSR entries
  print_results(csr);

  return 0;
}
