#define BOSC_SERVER_IP "127.0.0.1"
#define BOSC_SERVER_PORT 8000
#define BOSC_IFACE_MTU 9150
#define ERR_SOCKET -3

#include "busc_api.h"


using namespace std;

int main(char args[])
{
    BUSC_RECORD *record;
    record = new BUSC_RECORD;
    record->rowkey.assign("2014524404");
    
    
//    memcpy(new_record->value,(char*)buf, );
//    strcpy(record->value,"value");
//    
//    if (cdc_busc_delete(record))
//        cout<<"continue!\n";
//    if (cdc_busc_insert(record))
//        return -1;    
//    if(cdc_busc_get(record))
//        return -1;
//    cout<<"\nGET:"<<record->value<<endl;
//    strcpy(record->value,"after modify"); 
//    if(cdc_busc_modify(record))
//        return -1;
    if(cdc_busc_get(record))
        return -1;
    cout<<record->value<<endl;
    
    return 0;
    
}

int cdc_busc_get(BUSC_RECORD *result)
{
//    Initialize GET REQ
    req_select_t req;
    bzero(&req, sizeof(req_select_t));
//    set rowkey
    strcpy(req.object_key, result->rowkey.c_str());
    strcpy(req.owner_id, result->rowkey.c_str());    
    
    req.flag = 8176;
    req.msg_type = REQ_TYPE_SELECT;
    req.seq_no = 0;
    char value[OBJECT_NAME_LEN];
    char length[CONTENT_TYPE_LEN];
    if (select_client(&req, value, length))
        return -1;
    
    result->val_len = atoi(length);
    std::cout<<"value_length:\t"<<result->val_len<<std::endl;
    memcpy(result->value, value, result->val_len);
    return 0;
}
int cdc_busc_insert(BUSC_RECORD *record)
{
    char buffer[CONTENT_TYPE_LEN];
    
//    Initialize INSERT REQ
    req_insert_t req;
    bzero(&req,sizeof(req_insert_t));
    
//    ownerid+objectkey will be rowkey
    strcpy(req.object_key,record->rowkey.c_str());
    strcpy(req.owner_id,record->rowkey.c_str());
    
    memcpy(req.object_name, record->value, record->val_len);    
    std::sprintf(buffer,"%i", record->val_len);
    strcpy(req.content_type, buffer);

    req.version_id[0] = 0;
    req.last_modified = 0;
    req.object_size = 0;
    req.flags= 0;
//    Set request to INSERT REQ
    req.msg_type = REQ_TYPE_INSERT;
    req.seq_no = 0;
    req.object_id= 0;
    
//    Test the Info in REQ
    string object_key,object_name,content_type;
    object_key.assign(req.object_key,770);
    object_name.assign(req.object_name, 256);
    content_type.assign(req.content_type, 128);
    cout<<"req.object_key:\t"<<object_key<<"\nreq.object_name:\t"<<object_name<<"\ncontent_type:\t"<<content_type<<endl;
    
//    Send REQ to BUSC
    if (normal_client((void *)&req, 0))
        return -1;
    
    return 0;
}
int cdc_busc_delete(BUSC_RECORD *record)
{
//    Initialize INSERT REQ
    req_delete_t req;
    bzero(&req, sizeof(req_delete_t));

    strcpy(req.object_key, record->rowkey.c_str());
    strcpy(req.owner_id, record->rowkey.c_str());
    req.flag = 0;
    req.msg_type = REQ_TYPE_DELETE;
    req.seq_no = 0;
  
    if (normal_client((void *)&req, 3)) 
        return -1;
    
    return 0;
}
int cdc_busc_modify(BUSC_RECORD *record)
{
    char buffer[CONTENT_TYPE_LEN];
//    Initialize INSERT REQ    
    req_modify_t req;    
    bzero(&req, sizeof(req_modify_t));
    
    strcpy(req.object_key, record->rowkey.c_str());
    strcpy(req.owner_id, record->rowkey.c_str());
    memcpy(req.object_name, record->value, record->val_len);    
    std::sprintf(buffer,"%i", record->val_len);
    strcpy(req.content_type, buffer);
    strcpy(req.etag, "");
    req.version_id[0] = 0;
    req.last_modified = 0;
    req.object_size = 10;
    req.flags = 0;
    req.msg_type = REQ_TYPE_MODIFY;
    req.seq_no = 0;
    
    if (normal_client((void *)&req, 4))
        return -1;
    else
        return 0;
}
unsigned int count_select_length(int flag)
{
    unsigned int length = 0;
    if (flag & OBJECT_KEY_FLAG) length += OBJECT_KEY_LEN;
    if (flag & OBJECT_NAME_FLAG) length += OBJECT_NAME_LEN;
    if (flag & CONTENT_TYPE_FLAG) length += CONTENT_TYPE_LEN;
    if (flag & ETAG_FLAG) length += ETAG_LEN;
    if (flag & OWNER_ID_FLAG) length += OWNER_ID_LEN;
    if (flag & VERSION_ID_FLAG) length += VERSION_ID_LEN;
    if (flag & LAST_MODIFIED_FLAG) length += LAST_MODIFIED_LEN;
    if (flag & OBJECT_SIZE_FLAG) length += OBJECT_SIZE_LEN;
    if (flag & OBJECT_ID_FLAG) length += OBJECT_ID_LEN;

    return length;
}

unsigned long long bytetolonglong (unsigned char* arr, int off)
{
    //printf("in bytetolonglong: %d %d %d %d\n", arr[off], arr[off+1], arr[off+2], arr[off+3]);
    unsigned long long value = ((unsigned long long) arr[off]) | ((unsigned long long) arr[off+1] << 8) | ((unsigned long long) arr[off+2] << 16) | ((unsigned long long) arr[off+3] << 24);
    return value;
}

int connect_to_socket_server (char *IP, unsigned short port, int iface_mtu, char *svr_name, char *cl_name)
{
    int sockfd;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    int maxseg = iface_mtu;
    int optval =1;

    /* Create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("connect_to_%s_server: socket create failed\n",svr_name);
        return ERR_SOCKET;
    }

    /* Set the maximum segment size to interface MTU */
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &maxseg, sizeof(maxseg)) < 0)
    {
        printf("connect_to_%s_server: set socket opt failed\n",svr_name);
        close(sockfd);
        return ERR_SOCKET;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (const void *)&optval , sizeof(int)) !=0)
    {
        close(sockfd);
        return ERR_SOCKET;
    }

    /* Get the server's DNS entry */
    server = gethostbyname(IP);
    if (!server)
    {
        printf("connect_to_%s_server: get host name failed\n",svr_name);
        close(sockfd);
        return ERR_SOCKET;
    }

    /* Build the server's Internet address */
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr=inet_addr(IP);
//	    bcopy((char *)server->h_addr, (char *)&serveraddr.sin_addr.s_addr,
//	          server->h_length);
    serveraddr.sin_port = htons(port);

    /* Create a connection with the server */
    if (connect(sockfd, (struct sockaddr *)&serveraddr,
                sizeof(serveraddr)) < 0)
    {
        printf("connect_to_%s_server: socket connect failed, %d\n",svr_name,errno);
        close(sockfd);
        return ERR_SOCKET;
    }

//	    timestamp();
//	    printf("connect_to_%s_server: %s connection established successfully\n",cl_name,cl_name);

    return sockfd;
}

int normal_client (void * req, int op)
{
    int sockfd = -1;
    int ret = -1;
    
    while (1)
    {
        while (sockfd < 0)
        {
            sockfd = connect_to_socket_server(BOSC_SERVER_IP, BOSC_SERVER_PORT, BOSC_IFACE_MTU, "0", "0");
        }  
 
        printf("sockfd: %d\n", sockfd);  
     
        if (op == 0)
            ret = write(sockfd, (req_insert_t *)req, sizeof(req_insert_t));
        else if (op == 3)
            ret = write(sockfd, (req_delete_t *)req, sizeof(req_delete_t));
        else if (op == 4)
            ret = write(sockfd, (req_modify_t *)req, sizeof(req_modify_t));
        
        printf("ret: %d\n", ret);
        printf("size of insert: %d\n", sizeof(req_insert_t)); 
        printf("size of delete: %d\n", sizeof(req_delete_t));
        printf("size of modify: %d\n", sizeof(req_modify_t));

        if (ret == -1)
        {
            printf("Process_recover_thread: failed to send req_type to bosc server\n");
            close(sockfd);
            sockfd = -1;
            continue;
        }

        int bytes_read, bytes_remaining;
        rsp_ack_t rsp;
        uint8_t *buf;
        buf = (uint8_t *)(&rsp);
        bytes_remaining = RSP_ACK_LEN;

        while (bytes_remaining > 0)
        {
            bytes_read = read(sockfd, buf, bytes_remaining);
            
            if (bytes_read <=0)
            {
                printf("reading content failed!\n");
                close(sockfd);
                sockfd=-1;
                break;
            }
            bytes_remaining -= bytes_read;
        }

        if (rsp.status != 1)
        {
            if (op == 0)
                printf("\tFailed insert!!\n");
            else if (op == 3)
                printf("\tFailed delete!\n");
            else if (op == 4)
                printf("\tFailed modify!\n");
            
            
            return -1;
        }

        close(sockfd);        
       
        return 0;
    }
}

int select_client(req_select_t * req, char * value, char * value_len)
{
    int sockfd = -1;
    int ret = -1;
    
    while (sockfd < 0)
    {
        sockfd = connect_to_socket_server(BOSC_SERVER_IP, BOSC_SERVER_PORT, BOSC_IFACE_MTU, "0", "0");
    } 
    
    ret = write(sockfd, req, sizeof(req_select_t));
    
    if (ret == -1)
    {
        printf("Process_recover_thread: failed to send req_type to bosc server\n");
        close(sockfd);
        return -1;
    }

    unsigned int bytes_read, bytes_remaining;
    rsp_ack_t rsp;
    uint8_t *buf; // usigned char : 0 ~ (2^8 - 1)
    buf = (uint8_t *)(&rsp);
    bytes_remaining = RSP_ACK_LEN;
    int read_total = 0;
    
    while (bytes_remaining > 0)
    {
        bytes_read = read(sockfd, &buf[read_total], bytes_remaining);
        
        if (bytes_read <= 0)
        {
            printf("Process Thread (process select):  read header failed(EPIPE)! %d\n", bytes_read);
            close(sockfd);
            return -1;
        }
        
        bytes_remaining -= bytes_read;
        read_total += bytes_read;
    }

    if (rsp.status == 0)
    {
        printf("select object failed\n");
        close(sockfd);
        return -1;
    }
    else
    {
        printf("select object success\n");
    }
    
    unsigned int num_objects = bytetolonglong((unsigned char*)buf, 9);
    bytes_remaining = num_objects * count_select_length((*req).flag);
    char * data = new char[bytes_remaining];
    read_total = 0;
    int byte_idx = 0;
    
    while (bytes_remaining > 0)
    {
        bytes_read = read(sockfd, &data[read_total], bytes_remaining);
        
        if (bytes_read <=0)
        {
            printf("Process Thread (process select):  read content failed(EPIPE)!\n");
            close(sockfd);
            free(data);
            return -1;
        }
        
        bytes_remaining -= bytes_read;
        read_total += bytes_read;
    }
    
    byte_idx = byte_idx + OBJECT_KEY_LEN;
    memcpy(value, &data[byte_idx], OBJECT_NAME_LEN);
    byte_idx = byte_idx + OBJECT_NAME_LEN;
    memcpy(value_len, &data[byte_idx], CONTENT_TYPE_LEN);
    
    delete [] data;
    
    return 0;
}

