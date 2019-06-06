#include "utils.h"

#include <stdlib.h>

struct FactorialArgs* prepare_factorial_args(uint64_t begin, uint64_t end, uint64_t mod,  size_t count){
//returns FactorialArgs splited by count parts, count must be >=end-begin
        struct FactorialArgs* args=malloc(sizeof(struct FactorialArgs)*count);
        for (size_t i = 0; i < count; i++)
        {        
        args[i].begin = begin+i*((end-begin)/count);
        args[i].end = begin+(i+1)*((end-begin)/count);
        if(i==count-1)
          args[i].end=end;
        args[i].mod = mod;
        }
        return args;

}