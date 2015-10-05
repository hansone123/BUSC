#define BOSC_SERVER_IP "127.0.0.1"
#define BOSC_SERVER_PORT 8000
#define BOSC_IFACE_MTU 9150
#define ERR_SOCKET -3

#include <stdio.h>
//#include <stdlib.h>
//#include <stdint.h>
//#include <time.h>
//#include <pthread.h>
#include <unistd.h>
//#include <signal.h>
//#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
//#include <sys/types.h>
//#include <sys/time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <string>
#include "et/com_err.h"
#include "bosc_config.h"
#include "string.h"
#include <iostream>

typedef struct st_busc_record {
    std::string rowkey;
    char value[OBJECT_NAME_LEN];    
    int val_len;
    std::string tlb_name;
} BUSC_RECORD;

unsigned int count_select_length(int flag);
unsigned long long bytetolonglong (unsigned char* arr, int off);
int connect_to_socket_server (char *IP, unsigned short port, int iface_mtu, char *svr_name, char *cl_name);
int normal_client (void * req, int op);
int select_client(req_select_t * req, char * value, char * content_type);
int cdc_busc_get(BUSC_RECORD *result);
int cdc_busc_insert(BUSC_RECORD *record);
int cdc_busc_delete(BUSC_RECORD *record);
int cdc_busc_modify(BUSC_RECORD *record);

