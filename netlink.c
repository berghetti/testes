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
#include <netinet/in.h>

#include <linux/inet_diag.h>
#include <arpa/inet.h>

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
   struct msghdr msg;
   struct nlmsghdr nlh;
   //To request information about unix sockets, this would be replaced with
   //unix_diag_req, packet-sockets packet_diag_req.
   struct inet_diag_req_v2 conn_req;
   struct sockaddr_nl sa;
   struct iovec iov[4];
   int retval = 0;

   //For the filter
   // struct rtattr rta;
   // int filter_len = 0;

   memset(&msg, 0, sizeof(msg));
   memset(&sa, 0, sizeof(sa));
   memset(&nlh, 0, sizeof(nlh));
   memset(&conn_req, 0, sizeof(conn_req));

   //No need to specify groups or pid. This message only has one receiver and
   //pid 0 is kernel
   sa.nl_family = AF_NETLINK;

   //Address family and protocol we are interested in. sock_diag can also be
   //used with UDP sockets, DCCP sockets and Unix sockets, to mention a few.
   //This example requests information about TCP sockets bound to IPv4
   //addresses.
   conn_req.sdiag_family = AF_INET;
   conn_req.sdiag_protocol = IPPROTO_TCP;

   //Filter out some states, to show how it is done
   conn_req.idiag_states = -1;
   // TCPF_ALL & ~((1<<TCP_SYN_RECV) | (1<<TCP_TIME_WAIT) | (1<<TCP_CLOSE));

   //Request extended TCP information (it is the tcp_info struct)
   //ext is a bitmask containing the extensions I want to acquire. The values
   //are defined in inet_diag.h (the INET_DIAG_*-constants).
   conn_req.idiag_ext |= (1 << (INET_DIAG_INFO - 1));

   nlh.nlmsg_len = NLMSG_LENGTH(sizeof(conn_req));
   //In order to request a socket bound to a specific IP/port, remove
   //NLM_F_DUMP and specify the required information in conn_req.id
   nlh.nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;

   //Example of how to only match some sockets
   //In order to match a single socket, I have to provide all fields
   //sport/dport, saddr/daddr (look at dump_on_icsk)
   //conn_req.id.idiag_dport=htons(443);

   //Avoid using compat by specifying family + protocol in header
   nlh.nlmsg_type = SOCK_DIAG_BY_FAMILY;
   // nlh.nlmsg_type = TCPDIAG_GETSOCK;
   iov[0].iov_base = (void*) &nlh;
   iov[0].iov_len = sizeof(nlh);
   iov[1].iov_base = (void*) &conn_req;
   iov[1].iov_len = sizeof(conn_req);


   //Set essage correctly
   msg.msg_name = (void*) &sa;
   msg.msg_namelen = sizeof(sa);
   msg.msg_iov = iov;
   msg.msg_iovlen = 2;


   retval = sendmsg(fd, &msg, 0);


   puts("aqui1");
   return retval;


  // struct sockaddr_nl nladdr = { .nl_family = AF_NETLINK };
	// struct {
	// 	struct nlmsghdr nlh;
	// 	struct inet_diag_req r;
	// } req = {
	// 	.nlh.nlmsg_len = sizeof(req),
	// 	.nlh.nlmsg_flags = NLM_F_ROOT | NLM_F_MATCH | NLM_F_REQUEST,
	// 	// .nlh.nlmsg_seq = MAGIC_SEQ,
	// 	.r.idiag_family = AF_INET,
	// 	.r.idiag_states = 0xFFF
	// };
  // struct iovec iov;
  // iov.iov_base = &(req.nlh);
  // iov.iov_len = sizeof(req);
  //
  // struct msghdr msg;
  // msg = (struct msghdr) {
  // .msg_name = (void *)&nladdr,
  // .msg_namelen = sizeof(nladdr),
  // .msg_iov = &iov,
  // .msg_iovlen = 1,
  // };
  //
  // if (sendmsg(fd, &msg, 0) < 0) {
  //       perror("sendmsg");
  //       // return -1;
  //   }
  //
  // puts("aqui");
  // return 0;


  // struct sockaddr_nl nladdr = { .nl_family = AF_NETLINK };
  // struct
  // {
  //   struct nlmsghdr nlh;
  //   // struct unix_diag_req udr;
  //   struct inet_diag_req_v2 conn_req;
  // } req = {
  //   .nlh = {
  //     .nlmsg_len = NLMSG_LENGTH(sizeof(struct inet_diag_req_v2)),
  //     .nlmsg_type = SOCK_DIAG_BY_FAMILY,
  //     .nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP
  //   },
  //   .conn_req = {
  //     .sdiag_family = AF_INET,
  //     .sdiag_protocol = IPPROTO_TCP,
  //     // .udiag_states = -1,
  //     // .idiag_states = 0xFF
  //     .idiag_states =  TCPF_ALL & ~((1<<TCP_SYN_RECV) | (1<<TCP_TIME_WAIT) | (1<<TCP_CLOSE))
  //     // .udiag_show = UDIAG_SHOW_NAME | UDIAG_SHOW_PEER
  //   }
  // };
  //
  // struct iovec iov[3];
  // iov[0].iov_base = &(req.nlh);
  // iov[0].iov_len = sizeof(struct sockaddr_nl);
  // iov[1].iov_base = &(req.conn_req);;
  // iov[1].iov_len = sizeof(struct inet_diag_req_v2);
  //
  // // struct iovec iov = {
  // //   .iov_base = &req,
  // //   .iov_len = sizeof(req)
  // // };
  // struct msghdr msg = {
  //   .msg_name = &nladdr,
  //   .msg_namelen = sizeof(nladdr),
  //   .msg_iov = iov,
  //   .msg_iovlen = 2
  // };
  //
  // for (;;) {
  //   if (sendmsg(fd, &msg, 0) < 0) {
  //     if (errno == EINTR)
  //     continue;
  //
  //     perror("sendmsg");
  //     return -1;
  //   }
  //
  //   puts("aqui");
  //   return 0;
  // }
}

static int
print_diag(const struct inet_diag_msg *diag, unsigned int len)
{
  if (len < NLMSG_LENGTH(sizeof(*diag))) {
    fputs("short response\n", stderr);
    return -1;
  }

  char src_addr_buf[128];
  char dst_addr_buf[128];

  inet_ntop(AF_INET, (struct in_addr*) &(diag->id.idiag_src),
          src_addr_buf, INET_ADDRSTRLEN);

  inet_ntop(AF_INET, (struct in_addr*) &(diag->id.idiag_dst),
          dst_addr_buf, INET_ADDRSTRLEN);

  printf("family - %d\nstate - %d\ninode - %d\n"
         "src - %s:%d\ndst - %s:%d\n\n",
   diag->idiag_family, diag->idiag_state, diag->idiag_inode,
   src_addr_buf, ntohs(diag->id.idiag_sport), dst_addr_buf, ntohs(diag->id.idiag_dport));



  // printf("%d\n", diag->udiag_family);
  // puts("aqui3");
  // if (diag->udiag_family != AF_UNIX) {
  //   fprintf(stderr, "unexpected family %u\n", diag->udiag_family);
  //   return -1;
  // }

  // unsigned int rta_len = len - NLMSG_LENGTH(sizeof(*diag));
  // // unsigned int peer = 0;
  // size_t path_len = 0;
  // char path[sizeof(((struct sockaddr_un *) 0)->sun_path) + 1];
  //
  // for (struct rtattr *attr = (struct rtattr *) (diag + 1);
  // RTA_OK(attr, rta_len); attr = RTA_NEXT(attr, rta_len)) {
  //   switch (attr->rta_type) {
  //     case UNIX_DIAG_NAME:
  //     if (!path_len) {
  //       path_len = RTA_PAYLOAD(attr);
  //       if (path_len > sizeof(path) - 1)
  //       path_len = sizeof(path) - 1;
  //       memcpy(path, RTA_DATA(attr), path_len);
  //       path[path_len] = '\0';
  //     }
  //     break;
  //
  //     case UNIX_DIAG_PEER:
  //     if (RTA_PAYLOAD(attr) >= sizeof(peer))
  //     peer = *(unsigned int *) RTA_DATA(attr);
  //     break;
  //   }
  // }
  //
  // printf("inode=%u", diag->udiag_ino);
  //
  // if (peer)
  // printf(", peer=%u", peer);
  //
  // if (path_len)
  // printf(", name=%s%s", *path ? "" : "@",
  // *path ? path : path + 1);
  //
  // putchar('\n');
  return 0;
}

static int
receive_responses(int fd)
{
  long buf[8192];
  struct sockaddr_nl nladdr = {
    .nl_family = AF_NETLINK
  };
  struct iovec iov = {
    .iov_base = buf,
    .iov_len = sizeof(buf)
  };
  int flags = 0;

  // struct sockaddr_nl nladdr;
  // struct msghdr msg;
  // struct iovec iov;
  // struct nlmsghdr *nlh = NULL;
  //
  // iov.iov_base = (void *)nlh;
  // iov.iov_len = 8192;
  // msg.msg_name = (void *)&(nladdr);
  // msg.msg_namelen = sizeof(nladdr);
  //
  // msg.msg_iov = &iov;
  // msg.msg_iovlen = 1;
  // recvmsg(fd, &msg, 0);
  // puts("aqui2");
  //
  // return 0;


  for (;;) {

    struct msghdr msg = {
      .msg_name = &nladdr,
      .msg_namelen = sizeof(nladdr),
      .msg_iov = &iov,
      .msg_iovlen = 1
    };

    ssize_t ret = recvmsg(fd, &msg, flags);
    puts("aqui2");

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

      if (h->nlmsg_type != SOCK_DIAG_BY_FAMILY) {
        fprintf(stderr, "unexpected nlmsg_type %u\n",
        (unsigned) h->nlmsg_type);
        return -1;
      }


      if (print_diag(NLMSG_DATA(h), h->nlmsg_len))
        return -1;
    }
  }
}

int
main(void)
{
  int fd =  socket(AF_NETLINK, SOCK_DGRAM, NETLINK_INET_DIAG);

  if (fd < 0) {
    perror("socket");
    return 1;
  }

  send_query(fd);

  receive_responses(fd);

  close(fd);
  return 0;
}
