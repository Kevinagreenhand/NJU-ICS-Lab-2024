#include "common.h"
#include <inttypes.h>
#include<assert.h>
void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);
static uint64_t cycle_cnt = 0;
static uint64_t total_size;
static uint64_t associativity_size;
uint64_t group=0;
uint64_t cacheline=0;
void cycle_increase(int n) { cycle_cnt += n; }


typedef struct 
{
  bool valid;
  bool change;
  uint32_t tag;
  uint32_t data[16];
}Cache;

static Cache *cache;
void write_back(uint32_t group_number,uint32_t lines){
  cache[lines].valid=false;
  if(cache[lines].change==1){
  uintptr_t mem_addr=(cache[lines].tag<<group)+group_number;
  mem_write(mem_addr,(uint8_t*)(cache[lines].data));
  cache[lines].change=0;
  }
}

// TODO: implement the following functions
int line_choose(uintptr_t addr,uint32_t group_number,uint32_t tag){
    int every_group=exp2(associativity_size);
    int start=group_number*every_group;
    int end=(group_number+1)*every_group;
    assert(start>=0&&end>=0);
      for(int i=start;i<end;i++){
      if(cache[i].valid==false){
        uintptr_t mem_addr=(addr>>BLOCK_WIDTH);
        mem_read(mem_addr,(uint8_t*)(cache[i].data));
        cache[i].valid=true;
        cache[i].change=false;
        cache[i].tag=tag;
        return i;
      }
  }
    int random=rand()%every_group;
    int replace=start+random;
    assert(replace<=end);
    write_back(group_number,replace);
    uintptr_t  mem_addr=(addr>>BLOCK_WIDTH);
    mem_read(mem_addr,(uint8_t*)(cache[replace].data));
    cache[replace].valid=true;
    cache[replace].change=false;
    cache[replace].tag=tag;
    return replace;
}

int find(uint32_t group_number,uint32_t tag){
  int every_group=exp2(associativity_size);
  int start=group_number*every_group;
  int end=(group_number+1)*(every_group);
  for(int i=start;i<end;i++){
      if(cache[i].tag==tag&&cache[i].valid==1)  return i;
  }
  return -1;
}


uint32_t cache_read(uintptr_t addr) {
  uint32_t tag=addr>>(group+BLOCK_WIDTH);
  uint32_t temp=(1<<group)-1;
  uint32_t group_num=(addr>>BLOCK_WIDTH)&temp;
  uint32_t group_addr=(addr&0x3f)>>2;
  int line_number=find(group_num,tag);
  if(line_number!=-1){
      return cache[line_number].data[group_addr];
  }
  else{
    int line=line_choose(addr,group_num,tag);
    return cache[line].data[group_addr];
  }
  return 0;
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  uint32_t tag=addr>>(group+BLOCK_WIDTH);
  uint32_t temp=(1<<group)-1;
  uint32_t group_num=(addr>>BLOCK_WIDTH)&temp;
  uint32_t group_addr=(addr&0x3f)>>2;
  int line_number=-1; 
  int every_group=exp2(associativity_size);
  int start=group_num*every_group;
  int end=(group_num+1)*(every_group);
  for(int i=start;i<end;i++){
    if(cache[i].tag==tag){
      line_number=i;
      }
  }
  if(line_number!=-1){
    cache[line_number].change=1;
    cache[line_number].data[group_addr] &= (~wmask);
	  cache[line_number].data[group_addr] |= (data & wmask);
  }
  else{
    int new_line=line_choose(addr,group_num,tag);
    cache[new_line].change=1;
    cache[new_line].data[group_addr] &= (~wmask);
	  cache[new_line].data[group_addr] |= (data & wmask);
  }
}


void init_cache(int total_size_width, int associativity_width) {
  total_size=total_size_width;
  associativity_size=associativity_width;
  assert(total_size_width > associativity_width);
  group=total_size_width-BLOCK_WIDTH-associativity_width;
  cacheline=exp2(total_size_width-BLOCK_WIDTH);
  cache = (Cache*)malloc(sizeof(Cache) * cacheline);
  for(int i=0;i<cacheline;i++){
    cache[i].valid=false;
    cache[i].change=false;
  }
}

void display_statistic(void) {
}