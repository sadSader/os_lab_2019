#include <stdint.h>
#include <stddef.h>
struct FactorialArgs {
  uint64_t begin;
  uint64_t end;
  uint64_t mod;
};
struct FactorialArgs* prepare_factorial_args(uint64_t begin, uint64_t end, uint64_t mod,  size_t count);