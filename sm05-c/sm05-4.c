#include <unistd.h>

struct FileWriteState {
  int fd; // "файловый дескриптор", для вывода на стандартный поток вывода - 1
  unsigned char *buf; // указатель на буфер
  int bufsize;        // размер буфера
  // здесь потребуется добавить поля для реализации буферизованной записи
  int cur_size;
};

enum {
  BUF_SIZE = 4096,
};

static unsigned char buf[BUF_SIZE];
struct FileWriteState stout_impl = {1, buf, BUF_SIZE, 0};
struct FileWriteState *stout = &stout_impl;

void flush(struct FileWriteState *out) {
  write(out->fd, out->buf, out->cur_size);
  out->cur_size = 0;
}

void writechar(int c, struct FileWriteState *out) {
  buf[out->cur_size] = c;
  if (++out->cur_size == out->bufsize) {
    flush(out);
  }
}
