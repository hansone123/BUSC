/* 
 * File:   bosc_config.h
 * Author: Cliff
 *
 * Created on 2015年6月9日, 下午 9:26
 */
#ifndef BOSC_CONFIG_H
#define	BOSC_CONFIG_H

#define OBJECT_KEY_LEN 770
#define OBJECT_NAME_LEN 256
#define OWNER_ID_LEN 30
#define CONTENT_TYPE_LEN 128
#define ETAG_LEN 33
#define VERSION_ID_LEN 1
#define LAST_MODIFIED_LEN 8
#define OBJECT_SIZE_LEN 8
#define OBJECT_ID_LEN 8
#define FLAGS_LEN 4

#define AN_OBJECTS_FLAG 0
#define OBJECT_KEY_FLAG 16
#define OBJECT_NAME_FLAG 32
#define CONTENT_TYPE_FLAG 64
#define ETAG_FLAG 128
#define OWNER_ID_FLAG 256
#define VERSION_ID_FLAG 512
#define LAST_MODIFIED_FLAG 1024
#define OBJECT_SIZE_FLAG 2048
#define OBJECT_ID_FLAG 4096

#define REQ_TYPE_INSERT 0
#define REQ_TYPE_DELETE 3
#define REQ_TYPE_SELECT 2
#define REQ_TYPE_MODIFY 4

#pragma pack(push)
#pragma pack(1)

typedef struct req_insert_s
{
    short			msg_type;
    long long			seq_no;
    char			object_key[OBJECT_KEY_LEN];
    char			object_name[OBJECT_NAME_LEN];
    char			content_type[CONTENT_TYPE_LEN];
    char			etag[ETAG_LEN];
    char			owner_id[OWNER_ID_LEN];
    char 			version_id[VERSION_ID_LEN];
    long long			last_modified;
    long long			object_size;
    long long			object_id;
    int			   	flags;
} req_insert_t;
#define REQ_INSERT_LEN sizeof(req_insert_t)

typedef struct req_delete_s
{
    short                       msg_type;
    long long                   seq_no;
    char                        owner_id[OWNER_ID_LEN];
    char                        object_key[OBJECT_KEY_LEN];
    int                         flag;
} req_delete_t;
#define REQ_DELETE_LEN sizeof(req_delete_t)

typedef struct rsp_ack_s
{
    char                        status;
    unsigned long long          seq_no; 
    int                         num_objects;
} rsp_ack_t;
#define RSP_ACK_LEN sizeof(rsp_ack_t)

typedef struct req_select_s
{
    short			msg_type;
    long long			seq_no;
    char			owner_id[OWNER_ID_LEN];
    char			object_key[OBJECT_KEY_LEN];
    int			   	flag;
} req_select_t;

typedef struct req_modify_s
{
    short				msg_type;
    long long			seq_no;
    char				object_key[OBJECT_KEY_LEN];
    char				object_name[OBJECT_NAME_LEN];
    char				content_type[CONTENT_TYPE_LEN];
    char				etag[ETAG_LEN];
    char				owner_id[OWNER_ID_LEN];
    char 				version_id[VERSION_ID_LEN];
    long long			last_modified;
    long long			object_size;
    int			   	flags;
} req_modify_t;
#define REQ_MODIFY_LEN sizeof(req_modify_t)

typedef struct bosc_metadata_s
{
    char			object_key[OBJECT_KEY_LEN];
    char			object_name[OBJECT_NAME_LEN];
    char			content_type[CONTENT_TYPE_LEN];
    char			etag[ETAG_LEN];
    char			owner_id[OWNER_ID_LEN];
    char 			version_id[VERSION_ID_LEN];
    long long			last_modified;
    long long			object_size;
    int			   	flags;
    long long			object_id;
    long  long		   	inode_no;
    uint32_t		   	hash_value;
} bosc_metadata_t;
#define BOSC_METADATA_LEN	sizeof(bosc_metadata_t)

#pragma pack(pop)

#endif	/* BOSC_CONFIG_H */

