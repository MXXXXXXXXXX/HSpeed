//
// Latest edit by Mxxxxx on 2018/1/23.
//


#ifndef HTTP_PARSE_H
#define HTTP_PARSE_H

#define CR '\r'
#define LF '\n'

// http请求行解析
int hs_http_parse_request_line(hs_http_request_t *request);
// http请求体解析
int hs_http_parse_request_body(hs_http_request_t *request);

#endif
