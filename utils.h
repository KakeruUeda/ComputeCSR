#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

#include "sparse.h"

void printStatus(const char* name, bool ok, const char* reason = nullptr)
{
  std::cout << std::left << std::setw(18) << name << (ok ? " PASSED" : " FAILED");
  if (!ok && reason)
    std::cout << " (" << reason << ")";
  std::cout << "\n";
}

int testCompress(const CsrMatrix& csr)
{
  const auto& rptr = csr.get_row_ptr();
  const auto& cind = csr.get_column_ind();
  const auto& vals = csr.get_value();

  std::vector<int>    true_rptr = {0, 4, 6, 7};
  std::vector<int>    true_cind = {0, 2, 1, 0, 1, 0, 2};
  std::vector<double> true_vals = {2.4, 0.6, 4.5, 1.1, 3.1, 0.5, 5.0};

  for (std::size_t i = 0; i < true_rptr.size(); ++i)
  {
    if (rptr[i] != true_rptr[i])
    {
      printStatus("Test: compress", false, "row pointers mismatch");
      return 1;
    }
  }
  if (cind.size() < true_cind.size())
  {
    printStatus("Test: compress", false, "column indices size mismatch");
    return 1;
  }
  for (std::size_t i = 0; i < true_cind.size(); ++i)
  {
    if (cind[i] != true_cind[i])
    {
      printStatus("Test: compress", false, "column index mismatch");
      return 1;
    }
  }
  if (vals.size() < true_vals.size())
  {
    printStatus("Test: compress", false, "values size mismatch");
    return 1;
  }
  for (std::size_t i = 0; i < true_vals.size(); ++i)
  {
    if (vals[i] != true_vals[i])
    {
      printStatus("Test: compress", false, "value mismatch");
      return 1;
    }
  }
  printStatus("Test: compress", true);
  return 0;
}

int testDeduplicate(const CsrMatrix& csr)
{
  const auto& rptr = csr.get_row_ptr();
  const auto& cind = csr.get_column_ind();
  const auto& vals = csr.get_value();

  std::vector<int>    true_rptr = {0, 3, 5, 6};
  std::vector<int>    true_cind = {0, 2, 1, 1, 0, 2};
  std::vector<double> true_vals = {3.5, 0.6, 4.5, 3.1, 0.5, 5.0};

  for (std::size_t i = 0; i < true_rptr.size(); ++i)
  {
    if (rptr[i] != true_rptr[i])
    {
      printStatus("Test: deduplicate", false, "row pointers mismatch");
      return 1;
    }
  }
  for (std::size_t i = 0; i < true_cind.size() && i < cind.size(); ++i)
  {
    if (cind[i] != true_cind[i])
    {
      printStatus("Test: deduplicate", false, "column index mismatch");
      return 1;
    }
  }
  for (std::size_t i = 0; i < true_vals.size() && i < vals.size(); ++i)
  {
    if (vals[i] != true_vals[i])
    {
      printStatus("Test: deduplicate", false, "value mismatch");
      return 1;
    }
  }
  printStatus("Test: deduplicate", true);
  return 0;
}

int testSort(const CsrMatrix& csr)
{
  const auto& rptr = csr.get_row_ptr();
  const auto& cind = csr.get_column_ind();
  const auto& vals = csr.get_value();

  std::vector<int>    true_rptr = {0, 3, 5, 6};
  std::vector<int>    true_cind = {0, 1, 2, 0, 1, 2};
  std::vector<double> true_vals = {3.5, 4.5, 0.6, 0.5, 3.1, 5.0};

  for (std::size_t i = 0; i < true_rptr.size(); ++i)
  {
    if (rptr[i] != true_rptr[i])
    {
      printStatus("Test: sort", false, "row pointers mismatch");
      return 1;
    }
  }
  for (std::size_t i = 0; i < true_cind.size() && i < cind.size(); ++i)
  {
    if (cind[i] != true_cind[i])
    {
      printStatus("Test: sort", false, "column index mismatch");
      return 1;
    }
  }
  for (int i = 0; i < static_cast<int>(rptr.size()) - 1; ++i)
  {
    for (int p = rptr[i]; p < rptr[i + 1] - 1; ++p)
    {
      if (cind[p] > cind[p + 1])
      {
        printStatus("Test: sort", false, "columns not sorted in row");
        return 1;
      }
    }
  }
  for (std::size_t i = 0; i < true_vals.size() && i < vals.size(); ++i)
  {
    if (vals[i] != true_vals[i])
    {
      printStatus("Test: sort", false, "value mismatch");
      return 1;
    }
  }
  printStatus("Test: sort", true);
  return 0;
}

int testUpdate(const CsrMatrix& csr)
{
  const auto& rptr = csr.get_row_ptr();
  const auto& cind = csr.get_column_ind();
  const auto& vals = csr.get_value();

  std::vector<int>    true_rptr = {0, 3, 5, 6};
  std::vector<int>    true_cind = {0, 1, 2, 0, 1, 2};
  std::vector<double> true_vals = {7.0, 9.0, 1.2, 1.0, 6.2, 10.0};

  for (std::size_t i = 0; i < true_rptr.size(); ++i)
  {
    if (rptr[i] != true_rptr[i])
    {
      printStatus("Test: update", false, "row pointers mismatch");
      return 1;
    }
  }
  for (std::size_t i = 0; i < true_cind.size() && i < cind.size(); ++i)
  {
    if (cind[i] != true_cind[i])
    {
      printStatus("Test: update", false, "column index changed");
      return 1;
    }
  }
  for (std::size_t i = 0; i < true_vals.size() && i < vals.size(); ++i)
  {
    if (vals[i] != true_vals[i])
    {
      printStatus("Test: update", false, "value mismatch");
      return 1;
    }
  }
  printStatus("Test: update", true);
  return 0;
}
