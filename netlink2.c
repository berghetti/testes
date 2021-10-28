// see man netlink

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/sock_diag.h>
#include <linux/unix_diag.h>
#include <linux/inet_diag.h>

#include <netinet/in.h>   // IPPROTO_TCP
#include <arpa/inet.h>    // inet_ntop

#define TCPF_ALL 0xFFF

enum{
    TCP_ESTABLISHED = 1,
    TCP_SYN_SENT,
    TCP_SYN_RECV,
    TCP_FIN_WAIT1,
    TCP_FIN_WAIT2,
    TCP_TIME_WAIT,
    TCP_CLOSE,
    TCP_CLOSE_WAIT,
    TCP_LAST_ACK,
    TCP_LISTEN,
    TCP_CLOSING
};


static int
send_query(int fd)
{
  struct sockaddr_nl nladdr = {
    .nl_family = AF_NETLINK
  };
  struct
  {
    struct nlmsghdr nlh;
    struct inet_diag_req idr;
  } req = {
    .nlh = {
      .nlmsg_len = sizeof(req),
      // .nlmsg_type = SOCK_DIAG_BY_FAMILY,
      .nlmsg_type = TCPDIAG_GETSOCK,
      .nlmsg_flags = NLM_F_ROOT | NLM_F_MATCH | NLM_F_REQUEST
    },
    .idr = {
      .idiag_family = AF_INET,
      // .sdiag_protocol = IPPROTO_TCP,
      // .idiag_ext = (1<<(INET_DIAG_INFO-1)) | (1<<(INET_DIAG_VEGASINFO-1)) | (1<<(INET_DIAG_CONG-1)),
      .idiag_states = -1,
    }
  };

  // struct iovec iov[3] = {0};
  struct iovec iov = {
    .iov_base = &req,
    .iov_len = sizeof(req)
  };
  struct msghdr msg = {
    .msg_name = &nladdr,
    .msg_namelen = sizeof(nladdr),
    .msg_iov = &iov,
    .msg_iovlen = 1
  };

  // iov[0] = (struct iovec){
	// 	.iov_base = &req,
	// 	.iov_len = sizeof(req)
	// };

  for (;;) {
    if (sendmsg(fd, &msg, 0) < 0) {
      if (errno == EINTR)
        continue;

      perror("sendmsg");
      return -1;
    }

    return 0;
  }
}

  static int
print_diag(const struct inet_diag_msg *diag, unsigned int len)
{
  if (len < NLMSG_LENGTH(sizeof(*diag))) {
    fputs("short response\n", stderr);
    return -1;
  }
  // if (diag->udiag_family != AF_UNIX) {
  //   fprintf(stderr, "unexpected family %u\n", diag->udiag_family);
  //   return -1;
  // }

  unsigned int rta_len = len - NLMSG_LENGTH(sizeof(*diag));
  unsigned int peer = 0;
  size_t path_len = 0;
  char path[sizeof(((struct sockaddr_un *) 0)->sun_path) + 1];

  char src_addr_buf[128];
  char dst_addr_buf[128];

  inet_ntop(AF_INET, (struct in_addr*) &(diag->id.idiag_src),
          src_addr_buf, INET_ADDRSTRLEN);

  inet_ntop(AF_INET, (struct in_addr*) &(diag->id.idiag_dst),
          dst_addr_buf, INET_ADDRSTRLEN);

  printf("family - %d\nstate - %d\ninode - %d\n"
         "src - %s:%d\ndst - %s:%d\n\n",
         diag->idiag_family,
         diag->idiag_state,
         diag->idiag_inode,
         src_addr_buf,
         ntohs(diag->id.idiag_sport),
         dst_addr_buf,
         ntohs(diag->id.idiag_dport));

  return 0;
}

  static int
receive_responses(int fd)
{
  long buf[8192 / sizeof(long)];
  struct sockaddr_nl nladdr = {
    .nl_family = AF_NETLINK
  };
  struct iovec iov = {
    .iov_base = buf,
    .iov_len = sizeof(buf)
  };
  int flags = 0;

  for (;;) {
    struct msghdr msg = {
      .msg_name = &nladdr,
      .msg_namelen = sizeof(nladdr),
      .msg_iov = &iov,
      .msg_iovlen = 1
    };

    ssize_t ret = recvmsg(fd, &msg, flags);

    if (ret < 0) {
      if (errno == EINTR)
        continue;

      perror("recvmsg");
      return -1;
    }
    if (ret == 0)
      return 0;

    const struct nlmsghdr *h = (struct nlmsghdr *) buf;

    if (!NLMSG_OK(h, ret)) {
      fputs("!NLMSG_OK\n", stderr);
      return -1;
    }

    for (; NLMSG_OK(h, ret); h = NLMSG_NEXT(h, ret)) {
      if (h->nlmsg_type == NLMSG_DONE)
        return 0;

      if (h->nlmsg_type == NLMSG_ERROR) {
        const struct nlmsgerr *err = NLMSG_DATA(h);

        if (h->nlmsg_len < NLMSG_LENGTH(sizeof(*err))) {
          fputs("NLMSG_ERROR\n", stderr);
        } else {
          errno = -err->error;
          perror("NLMSG_ERROR");
        }

        return -1;
      }

      // if (h->nlmsg_type != SOCK_DIAG_BY_FAMILY) {
      //   fprintf(stderr, "unexpected nlmsg_type %u\n",
      //       (unsigned) h->nlmsg_type);
      //   return -1;
      // }

      if (print_diag(NLMSG_DATA(h), h->nlmsg_len))
        return -1;
    }
  }
}

  int
main(void)
{
  int fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_SOCK_DIAG);

  if (fd < 0) {
    perror("socket");
    return 1;
  }

  int ret = send_query(fd) || receive_responses(fd);

  close(fd);
  return ret;
}
