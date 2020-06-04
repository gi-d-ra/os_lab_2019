#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>

#define VERBOSE
#define SADDR struct sockaddr

int main(int argc, char *argv[]) {
  const size_t kSize = sizeof(struct sockaddr_in);

  int lfd, cfd;
  int nread;
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;
  int buf_size = -1;
  int port = -1;

  while (1) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"buf_size", required_argument, 0, 0},
                                      {"port", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        if ((buf_size = atoi(optarg)) == 0) {
          printf("Error: bad buf_size value\n");
          return -1;
        }
        break;
      case 1:
        if ((port = atoi(optarg)) == 0) {
          printf("Error: bad port value\n");
          return -1;
        }
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (buf_size == -1 || port == -1) {
    fprintf(stderr, "Using: %s --buf_size [NUMBER] --port [NUMBER]\n",
            argv[0]);
    return -1;
  }

  char buf[buf_size];

  // Сокет
  // AF_INET - IPv4 протокол Интернета
  //!!!!!!
  // --!!!SOCK_STREAM!!! - Обеспечивает создание двусторонних надежных и последовательных потоков байтов , поддерживающих соединения. Может также поддерживаться механизм
  //внепоточных данных. - Что в нашем случае и делает это TCP соединением
  //!!!!!
  if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }
#ifdef VERBOSE
  printf("Socket %d created\n", lfd);
#endif

  // заполняет первые САЙЗ байтов той области памяти, на которую указывает servaddr, постоянным байтом 0.
  //Функция htonl() преобразует узловой порядок расположения байтов положительного целого hostlong в сетевой порядок расположения байтов.
  //Функция htons() преобразует узловой порядок расположения байтов положительного короткого целого hostshort в сетевой порядок расположения байтов
  memset(&servaddr, 0, kSize);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  int opt_val = 1;
  // Настройка опций сокета
  // lfd- дескриптор
  // SOL_SOCKET - (level) задается для манипуляции флагами на уровне сокета.
  // SO_REUSEADDR - Указывает, что правила проверки адресов, передаваемых с помощью вызова bind(2),
  // должны позволять повторное использование локального адреса
  //opt_val - задать логический флаг
  //
  setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

  // привязывает сокету lfd локальный адресс servaddr длинной ***k-Pop****, ой точнее kSize
  //
  if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
    perror("bind");
    exit(1);
  }

#ifdef VERBOSE
  printf("Socket %d bound to %d:%d\n", lfd, servaddr.sin_addr.s_addr, ntohs(servaddr.sin_port));
#endif

  //Выражает готовность сокета принимать входящие соединения и задает размер очереди
  //
  if (listen(lfd, 5) < 0) {
    perror("listen");
    exit(1);
  }

#ifdef VERBOSE
  printf("Socket %d is listening\n", lfd);
#endif

  while (1) {
    unsigned int clilen = kSize;

    //Функция accept используется с сокетами, ориентированными на устанавление соединения (SOCK_STREAM, SOCK_SEQPACKET и SOCK_RDM).
    // Эта функция извлекает первый запрос на соединение из очереди ожидающих соединений, создаёт новый подключенный сокет
    //почти с такими же параметрами, что и у s, и выделяет для сокета новый файловый дескриптор, который и возвращается.
    //Новый сокет более не находится в слушающем состоянии. Исходный сокет s не изменяется при этом вызове.
    //
    if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
      perror("accept");
      printf("Accept error: %d\n", errno);
      exit(1);
    }

#ifdef VERBOSE
    printf("Connection established, client: %d [%d:%d]\n", cfd, ntohl(cliaddr.sin_addr.s_addr), ntohs(cliaddr.sin_port));
#endif

    while ((nread = read(cfd, buf, buf_size)) > 0) {
      write(1, &buf, nread);
    }

    if (nread == -1) {
      perror("read");
      exit(1);
    }
    close(cfd);
  }
  close(lfd);
}
