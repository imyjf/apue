// my_aio_suspend.c
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

  // 为了传递给 aio_suspend 用，创建一个大小为 5 的数组
  const struct aiocb* aio_list[5] = { NULL };

  // 将其一中元素赋值，不一定是第 0 个，随便啊。
  aio_list[0] = &my_aiocb;

  // 只要 my_aiocb 这个异步读还没完成，aio_suspend 就会阻塞
  ret = aio_suspend(aio_list, 5, NULL);
  if (ret < 0) ERR_EXIT("aio_suspend");
  puts("aio_suspend return");

  // 实际上，这个 while 循环我们不可能看到它执行的。写在这里只是为了演示。
  while(aio_error(&my_aiocb) == EINPROGRESS) {
    puts("EINPROGRESS");
  }

  // 获取返回值。
  ret = aio_return(&my_aiocb);
  if (ret < 0) ERR_EXIT("aio_return");

  printf("content: %s, return: %d\n", buf, ret);

  return 0;
}
