typedef struct {
  char value[5];
} data;

__kernel void md5_hash(__global data * in, __global data * out)
{
  int i = get_global_id(0);
  int j;
  for(j = 0; j < 4; j++) {
    out[i].value[j] = 'X';
  }
  out[i].value[4] = '\0';
}
