#include "mongoose.h"

static const char *s_url = "http://127.0.0.1:5000/classify";
static const char *s_post_data = "{\"text\": \"This is a great day!\"}";

static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_CONNECT) {
    // 构造 POST 请求
    mg_printf(c,
              "POST /classify HTTP/1.1\r\n"
              "Host: 127.0.0.1\r\n"
              "Content-Type: application/json\r\n"
              "Content-Length: %d\r\n\r\n"
              "%s",
              (int) strlen(s_post_data), s_post_data);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    printf("------post request---------\n");
    fwrite(hm->body.buf, 1, hm->body.len, stdout);
    printf("\n");
    c->is_closing = 1;  // 关闭连接
  }
}

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_connect(&mgr, s_url, fn, NULL);

  for (;;) mg_mgr_poll(&mgr, 1000);  // 事件循环
  mg_mgr_free(&mgr);
  return 0;
}

