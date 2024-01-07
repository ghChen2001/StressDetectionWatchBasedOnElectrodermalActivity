
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <lwip/api.h>
#include <lwip/arch.h>
#include <lwip/opt.h>
#include <lwip/inet.h>
#include <lwip/errno.h>
#include <netdb.h>

#include "utils_getopt.h"
#include "bflb_mtimer.h"

#include "cJSON.h"

#define HOST_NAME                        "183.230.40.33"
#define ONENET_HTTP_POST_MAX_LEN         512
#define ONENET_HTTP_POST_CONTENT_MAX_LEN 512
#define DEV_ID                           "1089267244"
#define API_KEY                          "wokPkV4gfOTvejG6UsfROOwzTyQ="
#define XINZHI_KEY                       "SdTJiZMBU6zuimx2a"
#define XINZHI_IP                        "116.62.81.138"

// clang-format off
// static const uint8_t get_buf[] = "GET / HTTP/1.1 \r\nHost: www.gov.cn\r\n\r\n";
uint32_t recv_buf[512] = { 0 };
// clang-format on

uint64_t total_cnt = 0;
int sock_client = -1;
struct timeval timeout;

void onenet_close()
{
    if (sock_client) {
        closesocket(sock_client);
    }
    if (total_cnt > 0) {
        printf("Total send data=%lld\r\n", total_cnt);
    }
}

#define PING_USAGE                                \
    "wifi_http_test [hostname] [port]\r\n"        \
    "\t hostname: hostname or dest server ip\r\n" \
    "\t port: dest server listen port, defualt port:80\r\n"

void onenet_transfer(float temp, uint32_t hr, int ML_output, uint32_t stepCnt)
{
    char *host_name;
    char *addr;
    char *port;
    struct sockaddr_in remote_addr;

    total_cnt = 0;
    port = "80";
    addr = HOST_NAME;
    if ((sock_client = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Http Client create socket error\r\n");
        return;
    }

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(atoi(port));
    remote_addr.sin_addr.s_addr = inet_addr(addr);
    memset(&(remote_addr.sin_zero), 0, sizeof(remote_addr.sin_zero));

    printf("Host:%s, Server ip Address : %s:%s\r\n", HOST_NAME, addr, port);

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    lwip_setsockopt(sock_client, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

    if (connect(sock_client, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) != 0) {
        printf("Http client connect server falied!\r\n");
        closesocket(sock_client);
        return;
    }
    printf("Http client connect server success!\r\n");

    memset(recv_buf, 0, sizeof(recv_buf));
    //创建post数据
    char post_buf[ONENET_HTTP_POST_MAX_LEN];
    char post_content[ONENET_HTTP_POST_CONTENT_MAX_LEN];
    char post_content_len[4];

    memset(post_content, 0, sizeof(post_content));
    memset(post_buf, 0, sizeof(post_buf));

    if (ML_output == 0) {
        sprintf(post_content, "{\"datastreams\":["
                              "{\"id\":\"body_tempurature\",\"datapoints\":[{\"value\":%.1f}]},"
                              "{\"id\":\"heart_rate\",\"datapoints\":[{\"value\":%d}]},"
                              "{\"id\":\"Mental\",\"datapoints\":[{\"value\":\"Neutral\"}]},"
                              "{\"id\":\"step\",\"datapoints\":[{\"value\":\"%d\"}]}"
                              "]}",
                temp, hr, stepCnt);
    } else if (ML_output == 1) {
        sprintf(post_content, "{\"datastreams\":["
                              "{\"id\":\"body_tempurature\",\"datapoints\":[{\"value\":%.1f}]},"
                              "{\"id\":\"heart_rate\",\"datapoints\":[{\"value\":%d}]},"
                              "{\"id\":\"Mental\",\"datapoints\":[{\"value\":\"Stress\"}]},"
                              "{\"id\":\"step\",\"datapoints\":[{\"value\":\"%d\"}]}"
                              "]}",
                temp, hr, stepCnt);
    } else if (ML_output == 2) {
        sprintf(post_content, "{\"datastreams\":["
                              "{\"id\":\"body_tempurature\",\"datapoints\":[{\"value\":%.1f}]},"
                              "{\"id\":\"heart_rate\",\"datapoints\":[{\"value\":%d}]},"
                              "{\"id\":\"Mental\",\"datapoints\":[{\"value\":\"Amusement\"}]},"
                              "{\"id\":\"step\",\"datapoints\":[{\"value\":\"%d\"}]}"
                              "]}",
                temp, hr, stepCnt);
    } else {
        sprintf(post_content, "{\"datastreams\":["
                              "{\"id\":\"body_tempurature\",\"datapoints\":[{\"value\":%.1f}]},"
                              "{\"id\":\"heart_rate\",\"datapoints\":[{\"value\":%d}]},"
                              "{\"id\":\"Mental\",\"datapoints\":[{\"value\":\"---\"}]},"
                              "{\"id\":\"step\",\"datapoints\":[{\"value\":\"%d\"}]}"
                              "]}",
                temp, hr, stepCnt);
    }

    // sprintf(post_content, "{\"datastreams\":["
    //                       "{\"id\":\"body_tempurature\",\"datapoints\":[{\"value\":%.1f}]},"
    //                       "{\"id\":\"heart_rate\",\"datapoints\":[{\"value\":%d}]}"
    //                       "]}",
    //         temp, hr);

    sprintf(post_content_len, "%d", strlen(post_content));

    strcat(post_buf, "POST /devices/");
    strcat(post_buf, DEV_ID);
    strcat(post_buf, "/datapoints HTTP/1.1\r\n");
    strcat(post_buf, "api-key:");
    strcat(post_buf, API_KEY);
    strcat(post_buf, "\r\n");
    strcat(post_buf, "Host:api.heclouds.com\r\n");
    strcat(post_buf, "Content-Length:");
    strcat(post_buf, post_content_len);
    strcat(post_buf, "\r\n\r\n");
    strcat(post_buf, post_content);
    strcat(post_buf, "\r\n");
    printf("%s\r\n", post_buf);
    write(sock_client, post_buf, sizeof(post_buf));
    total_cnt = recv(sock_client, (uint8_t *)recv_buf, sizeof(recv_buf), 0);
    if (total_cnt > 0)
        printf("%s\r\n", (uint8_t *)recv_buf);
    // while (1) {
    //     total_cnt = recv(sock_client, (uint8_t *)recv_buf, sizeof(recv_buf), 0);
    //     if (total_cnt <= 0)
    //         break;
    //     printf("%s\r\n", (uint8_t *)recv_buf);
    //     vTaskDelay(1);
    // }
    // vTaskDelay(100);
    closesocket(sock_client);
}

void onenet_transfer_GSR(float *gsr, uint16_t len)
{
    char *host_name;
    char *addr;
    char *port;
    struct sockaddr_in remote_addr;

    total_cnt = 0;
    port = "80";
    addr = HOST_NAME;
    if ((sock_client = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Http Client create socket error\r\n");
        return;
    }

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(atoi(port));
    remote_addr.sin_addr.s_addr = inet_addr(addr);
    memset(&(remote_addr.sin_zero), 0, sizeof(remote_addr.sin_zero));

    printf("Host:%s, Server ip Address : %s:%s\r\n", HOST_NAME, addr, port);

    if (connect(sock_client, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) != 0) {
        printf("Http client connect server falied!\r\n");
        closesocket(sock_client);
        return;
    }
    printf("Http client connect server success!\r\n");

    memset(recv_buf, 0, sizeof(recv_buf));
    //创建post数据
    char post_buf[4096];
    char post_content[4096];
    char post_content_len[4];
    char temp[20];

    memset(post_content, 0, sizeof(post_content));
    memset(post_buf, 0, sizeof(post_buf));
    memset(temp, 0, sizeof(temp));

    strcat(post_content, "{\"datastreams\":[{\"id\":\"GSR\",\"datapoints\":[");
    for (int i = 0; i < len; i++) {
        sprintf(temp, "{\"value\":%.3f},", gsr[i]);
        strcat(post_content, temp);
    }
    // sprintf(post_content, "{\"datastreams\":["
    //                       "{\"id\":\"GSR\",\"datapoints\":[{\"value\":%.3f}]}"
    //                       "]}",
    //         temp, hr);
    strcat(post_content, "]}]}");

    sprintf(post_content_len, "%d", strlen(post_content));

    strcat(post_buf, "POST /devices/");
    strcat(post_buf, DEV_ID);
    strcat(post_buf, "/datapoints HTTP/1.1\r\n");
    strcat(post_buf, "api-key:");
    strcat(post_buf, API_KEY);
    strcat(post_buf, "\r\n");
    strcat(post_buf, "Host:api.heclouds.com\r\n");
    strcat(post_buf, "Content-Length:");
    strcat(post_buf, post_content_len);
    strcat(post_buf, "\r\n\r\n");
    strcat(post_buf, post_content);
    strcat(post_buf, "\r\n");
    printf("%s\r\n", post_buf);
    write(sock_client, post_buf, sizeof(post_buf));
    while (1) {
        total_cnt = recv(sock_client, (uint8_t *)recv_buf, sizeof(recv_buf), 0);
        if (total_cnt <= 0)
            break;
        printf("%s\r\n", (uint8_t *)recv_buf);
        vTaskDelay(1);
    }
    vTaskDelay(100);
    closesocket(sock_client);
}

void weather_get(char *location, char *city_name, char *weather_text, uint8_t *weather_code, uint8_t *weather_temp)
{
    char *host_name;
    char *addr;
    char *port;
    struct sockaddr_in remote_addr;

    total_cnt = 0;
    port = "80";
    addr = XINZHI_IP;
    if ((sock_client = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Http Client create socket error\r\n");
        return;
    }

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(atoi(port));
    remote_addr.sin_addr.s_addr = inet_addr(addr);
    memset(&(remote_addr.sin_zero), 0, sizeof(remote_addr.sin_zero));

    printf("Host:%s, Server ip Address : %s:%s\r\n", XINZHI_IP, addr, port);

    if (connect(sock_client, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) != 0) {
        printf("Http client connect server falied!\r\n");
        closesocket(sock_client);
        return;
    }
    printf("Http client connect server success!\r\n");

    memset(recv_buf, 0, sizeof(recv_buf));
    //创建get头
    char get_buf[ONENET_HTTP_POST_MAX_LEN];

    memset(get_buf, 0, sizeof(get_buf));

    strcat(get_buf, "GET /v3/weather/now.json?key=");
    strcat(get_buf, XINZHI_KEY);
    strcat(get_buf, "&location=");
    strcat(get_buf, location);
    strcat(get_buf, "&language=zh-Hans&unit=c");
    strcat(get_buf, "\r\n");
    strcat(get_buf, "Host:api.seniverse.com\r\n");
    printf("%s\r\n", get_buf);
    write(sock_client, get_buf, sizeof(get_buf));

    vTaskDelay(1);
    recv(sock_client, (uint8_t *)recv_buf, sizeof(recv_buf), 0);
    printf("%s\r\n", (uint8_t *)recv_buf);

    cJSON *cjson_weather = NULL;
    cJSON *cjson_level0 = NULL;
    cJSON *cjson_level1 = NULL;
    cJSON *cjson_level2 = NULL;

    cjson_weather = cJSON_Parse(recv_buf);

    cjson_level0 = cJSON_GetObjectItem(cjson_weather, "results");
    // printf("cJSON_GetArraySize %d\r\n", cJSON_GetArraySize(cjson_level0));
    cjson_level0 = cJSON_GetArrayItem(cjson_level0, 0);
    cjson_level1 = cJSON_GetObjectItem(cjson_level0, "location");
    cjson_level2 = cJSON_GetObjectItem(cjson_level1, "name");
    memcpy(city_name, cjson_level2->valuestring, strlen(cjson_level2->valuestring));
    printf("city name: %s\r\n", city_name);

    cjson_level1 = cJSON_GetObjectItem(cjson_level0, "now");
    cjson_level2 = cJSON_GetObjectItem(cjson_level1, "text");
    memcpy(weather_text, cjson_level2->valuestring, strlen(cjson_level2->valuestring));
    printf("weather text: %s\r\n", weather_text);

    cjson_level2 = cJSON_GetObjectItem(cjson_level1, "code");
    *weather_code = atoi(cjson_level2->valuestring);
    printf("weather code: %d\r\n", *weather_code);

    cjson_level2 = cJSON_GetObjectItem(cjson_level1, "temperature");
    memcpy(weather_temp, cjson_level2->valuestring, strlen(cjson_level2->valuestring));
    printf("weather temperature: %s\r\n", weather_temp);

    cJSON_Delete(cjson_weather);

    closesocket(sock_client);
}