#include <stdio.h>
#include <stdio.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <poll.h>
#include <signal.h>

typedef struct pcap_hdr_s {
  uint32_t magic_number;   /* magic number */
  uint16_t version_major;  /* major version number */
  uint16_t version_minor;  /* minor version number */
  int32_t  thiszone;       /* GMT to local correction */
  uint32_t sigfigs;        /* accuracy of timestamps */
  uint32_t snaplen;        /* max length of captured packets, in octets */
  uint32_t network;        /* data link type */
} pcap_hdr_t;

typedef struct pcaprec_hdr_s {
  uint32_t ts_sec;         /* timestamp seconds */
  uint32_t ts_usec;        /* timestamp microseconds */
  uint32_t incl_len;       /* number of octets of packet saved in file */
  uint32_t orig_len;       /* actual length of packet */
} pcaprec_hdr_t;

FILE* pcapfile;

void handle_frame(struct tpacket_hdr* tphdr, struct sockaddr_ll* addr, char* l2content, char * l3content){

  pcaprec_hdr_t hdr;
  hdr.ts_sec = ntohl(tphdr->tp_sec);
  hdr.ts_usec = ntohl(tphdr->tp_usec);
  hdr.incl_len = tphdr->tp_snaplen;
  hdr.orig_len = tphdr->tp_len;

  fwrite(&hdr,sizeof(hdr),1,pcapfile);
  fwrite(l2content,tphdr->tp_snaplen,1,pcapfile);
}

int main(){
  pcapfile = fopen("pcapfile.pcap","a+");

  pcap_hdr_t hdr;
  hdr.magic_number = 0xa1b2c3d4;
  hdr.version_major = 0x02;
  hdr.version_minor = 0x04;
  hdr.thiszone = 0;
  hdr.sigfigs = 0;
  hdr.snaplen = 524288;
  hdr.network = 1;

  fwrite(&hdr,sizeof(hdr),1,pcapfile);

int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
if (fd == -1) {
  perror("socket");
  exit(1);
 }

struct tpacket_req req = {0};
req.tp_frame_size = TPACKET_ALIGN(TPACKET_HDRLEN + ETH_HLEN) + TPACKET_ALIGN(1500);
req.tp_block_size = sysconf(_SC_PAGESIZE);
while (req.tp_block_size < req.tp_frame_size) {
  req.tp_block_size <<= 1;
 }
 req.tp_block_nr = 4;
size_t frames_per_buffer = req.tp_block_size / req.tp_frame_size;
req.tp_frame_nr = req.tp_block_nr * frames_per_buffer;

 int version = TPACKET_V1;
 setsockopt(fd, SOL_PACKET, PACKET_VERSION, &version, sizeof(version));

 setsockopt(fd, SOL_PACKET, PACKET_RX_RING, (void*)&req, sizeof(req));

size_t rx_ring_size = req.tp_block_nr * req.tp_block_size;
char* rx_ring = mmap(0, rx_ring_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

struct pollfd fds[1] = {0};
fds[0].fd = fd;
fds[0].events = POLLIN;
size_t frame_idx = 0;
char* frame_ptr = rx_ring;

while (1) {
  struct tpacket_hdr* tphdr = (struct tpacket_hdr*)frame_ptr;
  while (!(tphdr->tp_status & TP_STATUS_USER)) {
    if (poll(fds, 1, -1) == -1) {
      perror("poll");
      exit(1);
    }
  }

  struct sockaddr_ll* addr = (struct sockaddr_ll*)(frame_ptr + TPACKET_HDRLEN - sizeof(struct sockaddr_ll));
  char* l2content = frame_ptr + tphdr->tp_mac;
  char* l3content = frame_ptr + tphdr->tp_net;
  handle_frame(tphdr, addr, l2content, l3content);

  frame_idx = (frame_idx + 1) % req.tp_frame_nr;
  int buffer_idx = frame_idx / frames_per_buffer;
  char* buffer_ptr = rx_ring + buffer_idx * req.tp_block_size;
  int frame_idx_diff = frame_idx % frames_per_buffer;
  frame_ptr = buffer_ptr + frame_idx_diff * req.tp_frame_size;
 }

fflush(pcapfile);
fclose(pcapfile);
}
