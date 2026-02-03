/**
 * @brief Demonstrates COO assembly from multiple components
 *        and conver to CSR format with deduplication and sort.
 *
 */

#include "sparse.h"
#include "utils.h"

int main()
{
  // Local COO matrices

  // Component 0 (3x3 with internal equations)
  std::vector<int>    rind_c0 = {0, 0, 1, 2};
  std::vector<int>    cind_c0 = {0, 2, 1, 2};
  std::vector<double> vals_c0 = {2.4, 0.6, 3.1, 5.0};

  // Component 1 (2x2)
  std::vector<int>    rind_c1 = {0, 1, 1};
  std::vector<int>    cind_c1 = {1, 0, 1};
  std::vector<double> vals_c1 = {0.5, 4.5, 1.1};

  int nnz_c0 = 4;
  int nnz_c1 = 3;

  // Local index to global index
  int map_c0[3];  
  int map_c1[2];  
  map_c0[0] = 0;  // n0
  map_c0[1] = 1;  // n1  
  map_c0[2] = 2;  // int
  map_c1[0] = 1;  // n1
  map_c1[1] = 0;  // n0

  // Total number of nonzeros (duplicates included)
  int nnz = nnz_c0 + nnz_c1;

  // Create grobal COO
  std::vector<int>    rind(nnz);
  std::vector<int>    cind(nnz);
  std::vector<double> vals(nnz);

  // Insert components
  int counter = 0;
  for (int i = 0; i < nnz_c0; ++i)
  {
    rind[counter] = map_c0[rind_c0[i]];
    cind[counter] = map_c0[cind_c0[i]];
    vals[counter] = vals_c0[i];
    counter++;
  }
  for (int i = 0; i < nnz_c1; ++i)
  {
    rind[counter] = map_c1[rind_c1[i]];
    cind[counter] = map_c1[cind_c1[i]];
    vals[counter] = vals_c1[i];
    counter++;
  }

  int m = 3;
  int n = 3;

  // Create global CSR
  CsrMatrix csr(m, n, nnz);

  int results = 0;

  // COO -> CSR
  csr.compress(rind, cind, vals);
  results += testCompress(csr);

  // Deduplicate entries
  csr.deduplicate();
  results += testDeduplicate(csr);

  // Sort entries
  csr.sort();
  results += testSort(csr);

  std::cout << "\nCreated CSR entries:\n";
  csr.printEntries();
  std::cout << "\n";

  // Update values
  vals_c0[0] = 4.8;  // (0,0)
  vals_c0[1] = 1.2;  // (0,2)
  vals_c0[2] = 6.2;  // (1,1)
  vals_c0[3] = 10.0; // (2,2)
  vals_c1[0] = 1.0;  // (0,1)
  vals_c1[1] = 9.0;  // (1,0)
  vals_c1[2] = 2.2;  // (1,1)

  counter = 0;
  for (int i = 0; i < nnz_c0; ++i)
  {
    vals[counter] = vals_c0[i];
    counter++;
  }
  for (int i = 0; i < nnz_c1; ++i)
  {
    vals[counter] = vals_c1[i];
    counter++;
  }

  // Update entries with new values
  csr.update(vals);
  results += testUpdate(csr);

  // Print results
  std::cout << "\nCreated CSR entries (after values updated):\n";
  csr.printEntries();

  if (results == 0)
  {
    std::cout << "\nAll tests PASSED\n";
    return 0;
  }
  else
  {
    std::cout << "\nSome tests FAILED\n";
    return 1;
  }
}
