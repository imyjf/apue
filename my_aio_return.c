// my_aio_return.c
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <aio.h>
#include <strings.h>
#include <errno.h>

#define ERR_EXIT(msg) do { perror(msg); exit(1); } while(0)

int main() {
  int fd, ret;
  char buf[64] = { 0 };
  struct aiocb my_aiocb;

  bzero((char*)&my_aiocb, sizeof(struct aiocb));

  my_aiocb.aio_buf = buf;
  my_aiocb.aio_fildes = STDIN_FILENO;
  my_aiocb.aio_nbytes = 64; 
  my_aiocb.aio_offset = 0;

  ret = aio_read(&my_aiocb);
  if (ret < 0) ERR_EXIT("aio_read");

  while(aio_error(&my_aiocb) == EINPROGRESS) {
    sleep(1);
  }
	sleep(5);
  // 获取最终的异步操作状态
  ret = aio_return(&my_aiocb);
  if (ret < 0) ERR_EXIT("aio_return");

  // 打印内容和返回值。
  printf("content: %s, return: %d\n", buf, ret);

  return 0;
}
