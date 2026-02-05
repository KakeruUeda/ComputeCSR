#pragma once

#include <cassert>
#include <iomanip>
#include <iostream>

#include "sparse.h"

void printStatus(const char* name, bool ok, const char* reason = nullptr)
{
  std::cout << std::left << std::setw(18) << name << (ok ? " PASSED" : " FAILED");
  if (!ok && reason)
    std::cout << " (" << reason << ")";
  std::cout << "\n";
}

int testSort(const CooMatrix& coo)
{
  const int*    rind = coo.getRowInd();
  const int*    cind = coo.getColInd();
  int           nnz  = coo.getNnz();

  // Check sorted order
  for (int k = 0; k < nnz - 1; ++k)
  {
    if (rind[k] > rind[k + 1] ||
        (rind[k] == rind[k + 1] && cind[k] > cind[k + 1]))
    {
      printStatus("Test: sort", false, "not sorted");
      return 1;
    }
  }
  printStatus("Test: sort", true);
  return 0;
}

int testDeduplicate(const CooMatrix& coo)
{
  const int*    rind = coo.getRowInd();
  const int*    cind = coo.getColInd();
  const double* vals = coo.getValues();
  int           nnz  = coo.getNnz();

  int    true_rind[] = {0, 0, 0, 1, 1, 2};
  int    true_cind[] = {0, 1, 2, 0, 1, 2};
  double true_vals[] = {3.5, 4.5, 0.6, 0.5, 3.1, 5.0};
  int    true_nnz = 6;

  if (nnz != true_nnz)
  {
    printStatus("Test: deduplicate", false, "nnz mismatch");
    return 1;
  }
  for (int i = 0; i < true_nnz; ++i)
  {
    if (rind[i] != true_rind[i])
    {
      printStatus("Test: deduplicate", false, "row index mismatch");
      return 1;
    }
  }
  for (int i = 0; i < true_nnz; ++i)
  {
    if (cind[i] != true_cind[i])
    {
      printStatus("Test: deduplicate", false, "column index mismatch");
      return 1;
    }
  }
  for (int i = 0; i < true_nnz; ++i)
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

int testCsr(const CsrMatrix& csr)
{
  const int*    rptr = csr.getRowPtr();
  const int*    cind = csr.getColInd();
  const double* vals = csr.getValues();

  int    true_rptr[] = {0, 3, 5, 6};
  int    true_cind[] = {0, 1, 2, 0, 1, 2};
  double true_vals[] = {3.5, 4.5, 0.6, 0.5, 3.1, 5.0};
  int    true_rptr_size = 4;
  int    true_cind_size = 6;
  int    true_vals_size = 6;

  for (int i = 0; i < true_rptr_size; ++i)
  {
    if (rptr[i] != true_rptr[i])
    {
      printStatus("Test: csr", false, "row pointers mismatch");
      return 1;
    }
  }
  for (int i = 0; i < true_cind_size; ++i)
  {
    if (cind[i] != true_cind[i])
    {
      printStatus("Test: csr", false, "column index mismatch");
      return 1;
    }
  }
  for (int i = 0; i < true_vals_size; ++i)
  {
    if (vals[i] != true_vals[i])
    {
      printStatus("Test: csr", false, "value mismatch");
      return 1;
    }
  }
  printStatus("Test: csr", true);
  return 0;
}

int testUpdate(const CsrMatrix& csr)
{
  const int*    rptr = csr.getRowPtr();
  const int*    cind = csr.getColInd();
  const double* vals = csr.getValues();

  int    true_rptr[] = {0, 3, 5, 6};
  int    true_cind[] = {0, 1, 2, 0, 1, 2};
  double true_vals[] = {7.0, 9.0, 1.2, 1.0, 6.2, 10.0};
  int    true_rptr_size = 4;
  int    true_cind_size = 6;
  int    true_vals_size = 6;

  for (int i = 0; i < true_rptr_size; ++i)
  {
    if (rptr[i] != true_rptr[i])
    {
      printStatus("Test: update", false, "row pointers mismatch");
      return 1;
    }
  }
  for (int i = 0; i < true_cind_size; ++i)
  {
    if (cind[i] != true_cind[i])
    {
      printStatus("Test: update", false, "column index changed");
      return 1;
    }
  }
  for (int i = 0; i < true_vals_size; ++i)
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
