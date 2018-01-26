#ifndef __PKT_H_
#define __PKT_H_

#include "afx.h"
#include "xlog.h"


#define PKT_YS_START_TAG 0xA5
#define PKT_YS_END_TAG 0x5A
#define PKT_YS_FRAME_TYPE 0xB5
#define PKT_YS_HEADLEN 5
#define PKT_YS_ENDLEN 1




unsigned char pkt_build_check_sum ( unsigned char * buf , int size);
int pkt_check_sum ( unsigned char * buf , int size , unsigned char checksum );
unsigned char *  pkt_build_byte_tag(unsigned char * buf, unsigned char tag);
unsigned char *  pkt_build_short_tag(unsigned char * buf, unsigned short tag);
unsigned char *  pkt_match_head(unsigned char *buf, int len, unsigned char tag);
int	 pkt_make_client(unsigned char *buf, int len, unsigned char **out_data, int *out_len);
int	 pkt_parse_data(unsigned char *buf, int len, unsigned char **out_data, int *out_len);
int	 pkt_parse_frame(unsigned char *buf, int len, unsigned char *out_data, int out_len);
int	 pkt_make_frame(unsigned char *buf, int len, unsigned char *out_data, int out_len);


#endif

