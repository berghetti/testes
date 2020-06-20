#include <stdio.h>
#include <inttypes.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

int main(void)
{
  printf("sizeof char - %ld\n", sizeof(char));
  printf("sizeof short - %ld\n", sizeof(short));
  printf("sizeof int - %ld\n", sizeof(int));
  printf("sizeof long - %ld\n", sizeof(long));
  printf("sizeof unsigned long - %ld\n", sizeof(unsigned long));
  printf("sizeof long long - %ld\n", sizeof(long long));
  printf("sizeof unsigned long long - %ld\n", sizeof(unsigned long long));

  printf("\nsizeof uint64_t - %ld\n", sizeof(uint64_t));

  printf("\nsizeof struct ethhdr - %ld\n", sizeof(struct ethhdr));
  printf("\nsizeof struct iphdr - %ld\n", sizeof(struct iphdr));
  printf("\nsizeof struct tcphdr - %ld\n", sizeof(struct tcphdr));
  printf("\nsizeof struct udphdr - %ld\n", sizeof(struct udphdr)); 


}
