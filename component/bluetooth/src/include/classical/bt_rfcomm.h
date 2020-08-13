/******************************************************************************
  * @file           bt_rfcomm.h
  * @author         Yu-ZhongJun(124756828@qq.com)
  * @Taobao link    https://shop220811498.taobao.com/
  * @version        V0.0.1
  * @date           2020-4-16
  * @brief          bt rfcomm header file
******************************************************************************/

#ifndef BT_RFCOMM_H_H_H
#define BT_RFCOMM_H_H_H

#include "bt_l2cap.h"




#define rfcomm_cn(pcb) ((pcb)->cn)
#define rfcomm_cl(pcb) ((pcb)->cl)
#define rfcomm_local_credits(pcb) ((pcb)->k)
#define rfcomm_remote_credits(pcb) ((pcb)->rk)
#define rfcomm_fc(pcb) ((pcb)->fc)
#define rfcomm_mfs(pcb) ((pcb)->n)



/* Control field values */
#define RFCOMM_SABM 0x3F
#define RFCOMM_UA 0x73
#define RFCOMM_DM 0x0F
#define RFCOMM_DM_PF 0x1F
#define RFCOMM_DISC 0x53
#define RFCOMM_UIH 0xEF
#define RFCOMM_UIH_PF 0xFF 

/* Multiplexer message types */
#define RFCOMM_PN_CMD 0x83
#define RFCOMM_PN_RSP 0x81
#define RFCOMM_TEST_CMD 0x23
#define RFCOMM_TEST_RSP 0x21
#define RFCOMM_FCON_CMD 0xA3
#define RFCOMM_FCON_RSP 0xA1
#define RFCOMM_FCOFF_CMD 0x63
#define RFCOMM_FCOFF_RSP 0x61
#define RFCOMM_MSC_CMD 0xE3
#define RFCOMM_MSC_RSP 0xE1
#define RFCOMM_RPN_CMD 0x93
#define RFCOMM_RPN_RSP 0x91
#define RFCOMM_RLS_CMD 0x53
#define RFCOMM_RLS_RSP 0x51 
#define RFCOMM_NSC_RSP 0x11

/* Length of RFCOMM hdr with 1 or 2 byte lengh field excluding credit */
#define RFCOMM_HDR_LEN_1 3
#define RFCOMM_HDR_LEN_2 4

/* Length of a multiplexer message */
#define RFCOMM_MSGHDR_LEN 2
#define RFCOMM_PNMSG_LEN 8
#define RFCOMM_MSCMSG_LEN 2
#define RFCOMM_RPNMSG_LEN 8
#define RFCOMM_NCMSG_LEN 1

/* Length of a frame */
#define RFCOMM_DM_LEN 4
#define RFCOMM_SABM_LEN 4
#define RFCOMM_DISC_LEN 4
#define RFCOMM_UA_LEN 4
#define RFCOMM_UIH_LEN 3
#define RFCOMM_UIHCRED_LEN 4

/* Default convergence layer */
#define RFCOMM_CL 0xF /* Credit based flow control */

/* Default port settings for a communication link */
#define RFCOMM_COM_BR 0x03 /* Baud rate (9600 bit/s)*/
#define RFCOMM_COM_CFG 0x03 /* Data bits (8 bits), stop bits (1), parity (no parity) 
			       and parity type */
#define RFCOMM_COM_FC 0x00 /* Flow control (no flow ctrl) */
#define RFCOMM_COM_XON 0x00 /* No flow control (default DC1) */ 
#define RFCOMM_COM_XOFF 0x00 /* No flow control (default DC3) */

/* FCS calc */
#define RFCOMM_CODE_WORD 0xE0 /* pol = x8+x2+x1+1 */
#define RFCOMM_CRC_CHECK_LEN 3
#define RFCOMM_UIHCRC_CHECK_LEN 2

/* RFCOMM configuration parameter masks */
#define RFCOMM_CFG_IR 0x01
#define RFCOMM_CFG_FC 0x02
#define RFCOMM_CFG_MSC_OUT 0x04
#define RFCOMM_CFG_MSC_IN 0x08

enum rfcomm_state_e {
  RFCOMM_CLOSED, RFCOMM_LISTEN, W4_RFCOMM_MULTIPLEXER, W4_RFCOMM_SABM_RSP, RFCOMM_CFG, RFCOMM_OPEN, 
  W4_RFCOMM_DISC_RSP
};

/* The RFCOMM frame header */
struct rfcomm_hdr_t {
  uint8_t addr;
  uint8_t ctrl;
  uint16_t len;
  uint8_t k;
}BT_PACK_END;

struct rfcomm_msg_hdr_t {
  uint8_t type;
  uint8_t len;
}BT_PACK_END;

struct rfcomm_pn_msg_t {
  uint8_t dlci; /* Data link connection id */
  uint8_t i_cl; /* Type frame for information and Convergece layer to use */
  uint8_t p; /* Priority */
  uint8_t t; /* Value of acknowledgement timer */
  uint16_t n; /* Maximum frame size */
  uint8_t na; /* Maximum number of retransmissions */
  uint8_t k; /* Initial credit value */
}BT_PACK_END;

struct rfcomm_msc_msg_t {
  uint8_t dlci; /* Data link connection id */
  uint8_t rs232; /* RS232 control signals */
}BT_PACK_END;

struct rfcomm_rpn_msg_t {
  uint8_t dlci; /* Data link connection id */
  uint8_t br; /* Baud Rate */
  uint8_t cfg; /* Data bits, Stop bits, Parity, Parity type */
  uint8_t fc; /* Flow control */
  uint8_t xon;
  uint8_t xoff;
  uint16_t mask;
}BT_PACK_END;

/* The RFCOMM protocol control block */
struct rfcomm_pcb_t {
  struct rfcomm_pcb_t *next; /* For the linked list */

  enum rfcomm_state_e state; /* RFCOMM state */

  struct l2cap_pcb_t *l2cappcb; /* The L2CAP connection */

  uint8_t cn; /* Channel number */
  
  uint8_t cl; /* Convergence layer */
  uint8_t p; /* Connection priority */
  uint16_t n; /* Maximum frame size */
  uint8_t k; /* Number of local credits:The number of packets we can send */

  uint8_t rk; /* Number of remote credits:The number of packets that the other party can send */

  uint8_t rfcommcfg; /* Bit 1 indicates if we are the initiator of this connection
		   * Bit 2 indicates if the flow control bit is set so that we are allowed to send data
		   * Bit 3 indicates if modem status has been configured for the incoming direction
		   * Bit 4 indicates if modem status has been configured for the outgoing direction
		   */

  uint16_t to; /* Frame and cmd timeout */
  
  uint8_t uih_in_fcs; /* Frame check sequence for uih frames (P/F bit = 0) */
  uint8_t uihpf_in_fcs; /* Frame check sequence for uih frames (P/F bit = 1) */
  uint8_t uih_out_fcs; /* Frame check sequence for uih frames (P/F bit = 0) */
  uint8_t uihpf_out_fcs; /* Frame check sequence for uih frames (P/F bit = 1) */

  uint8_t uih0_in_fcs; /* Frame check sequence for uih frames on the control channel (P/F bit = 0) */
  uint8_t uih0_out_fcs; /* Frame check sequence for uih frames on the control channel (P/F bit = 0) */

#if RFCOMM_FLOW_QUEUEING
  struct bt_pbuf_t *buf;
#endif
  void *callback_arg;
  
  /* RFCOMM Frame commands and responses */
  err_t (* connected)(void *arg, struct rfcomm_pcb_t *pcb, err_t err);
  err_t (* accept)(void *arg, struct rfcomm_pcb_t *pcb, err_t err);
  err_t (* disconnected)(void *arg, struct rfcomm_pcb_t *pcb, err_t err);

  /* RFCOMM Multiplexer responses */
  err_t (* pn_rsp)(void *arg, struct rfcomm_pcb_t *pcb, err_t err);
  err_t (* test_rsp)(void *arg, struct rfcomm_pcb_t *pcb, err_t err);
  err_t (* msc_rsp)(void *arg, struct rfcomm_pcb_t *pcb, err_t err);
  err_t (* rpn_rsp)(void *arg, struct rfcomm_pcb_t *pcb, err_t err);

  err_t (* recv)(void *arg, struct rfcomm_pcb_t *pcb, struct bt_pbuf_t *p, err_t err);
};

/* The server channel */
struct rfcomm_pcb_listen_t {
  struct rfcomm_pcb_listen_t *next; /* For the linked list */

  enum rfcomm_state_e state; /* RFCOMM state */

  uint8_t cn; /* Channel number */

  void *callback_arg;

  err_t (* accept)(void *arg, struct rfcomm_pcb_t *pcb, err_t err);
};

#define RFCOMM_EVENT_CONNECTED(pcb,err,ret) \
                              if((pcb)->connected != NULL) \
                              (ret = (pcb)->connected((pcb)->callback_arg,(pcb),(err)))
#define RFCOMM_EVENT_ACCEPT(pcb,err,ret) \
                              if((pcb)->accept != NULL) \
                              (ret = (pcb)->accept((pcb)->callback_arg,(pcb),(err)))
#define RFCOMM_EVENT_DISCONNECTED(pcb,err,ret) \
                                 if((pcb)->disconnected != NULL) { \
                                   (ret = (pcb)->disconnected((pcb)->callback_arg,(pcb),(err))); \
                                 } else { \
                                   rfcomm_close(pcb); \
				 }
#define RFCOMM_EVENT_PN_RSP(pcb,err,ret) \
                       if((pcb)->pn_rsp != NULL) \
                       (ret = (pcb)->pn_rsp((pcb)->callback_arg,(pcb),(err)))
#define RFCOMM_EVENT_TEST(pcb,err,ret) \
                       if((pcb)->test_rsp != NULL) \
                       (ret = (pcb)->test_rsp((pcb)->callback_arg,(pcb),(err)))
#define RFCOMM_EVENT_MSC(pcb,err,ret) \
                        if((pcb)->msc_rsp != NULL) \
                        (ret = (pcb)->msc_rsp((pcb)->callback_arg,(pcb),(err)))
#define RFCOMM_EVENT_RPN(pcb,err,ret) \
                        if((pcb)->rpn_rsp != NULL) \
                        (ret = (pcb)->rpn_rsp((pcb)->callback_arg,(pcb),(err)))
#define RFCOMM_EVENT_RECV(pcb,err,p,ret) \
                          if((pcb)->recv != NULL) { \
                            (ret = (pcb)->recv((pcb)->callback_arg,(pcb),(p),(err))); \
                          } else { \
                            bt_pbuf_free(p); \
                          }


/* The RFCOMM PCB lists. */
extern struct rfcomm_pcb_listen_t *rfcomm_listen_pcbs; /* List of all RFCOMM PCBs listening for 
							 an incomming connection on a specific
							 server channel */
extern struct rfcomm_pcb_t *rfcomm_active_pcbs; /* List of all active RFCOMM PCBs */

extern struct rfcomm_pcb_t *rfcomm_tmp_pcb;      /* Only used for temporary storage. */

/* Define two macros, RFCOMM_REG and RFCOMM_RMV that registers a RFCOMM PCB
   with a PCB list or removes a PCB from a list, respectively. */

#define RFCOMM_REG(pcbs, npcb) do { \
                            npcb->next = *pcbs; \
                            *pcbs = npcb; \
                            } while(0)
#define RFCOMM_RMV(pcbs, npcb) do { \
                            if(*pcbs == npcb) { \
                               *pcbs = (*pcbs)->next; \
                            } else for(rfcomm_tmp_pcb = *pcbs; rfcomm_tmp_pcb != NULL; rfcomm_tmp_pcb = rfcomm_tmp_pcb->next) { \
                               if(rfcomm_tmp_pcb->next != NULL && rfcomm_tmp_pcb->next == npcb) { \
                                  rfcomm_tmp_pcb->next = npcb->next; \
                                  break; \
                               } \
                            } \
                            npcb->next = NULL; \
                            } while(0)


/* Functions for interfacing with RFCOMM: */

/* Lower layer interface to RFCOMM: */
err_t rfcomm_init(void); /* Must be called first to initialize RFCOMM */
void rfcomm_tmr(void); /* Must be called every 1s interval */

/* Application program's interface: */
struct rfcomm_pcb_t *rfcomm_new(struct l2cap_pcb_t *pcb);
void rfcomm_close(struct rfcomm_pcb_t *pcb);
void rfcomm_reset_all(void);
void rfcomm_arg(struct rfcomm_pcb_t *pcb, void *arg);
void rfcomm_recv(struct rfcomm_pcb_t *pcb, 
		 err_t (* recv)(void *arg, struct rfcomm_pcb_t *pcb, struct bt_pbuf_t *p, err_t err));
void rfcomm_disc(struct rfcomm_pcb_t *pcb, 
		 err_t (* disc)(void *arg, struct rfcomm_pcb_t *pcb, err_t err));

err_t rfcomm_input(void *arg, struct l2cap_pcb_t *l2cappcb, struct bt_pbuf_t *p, err_t err);

err_t rfcomm_connect(struct rfcomm_pcb_t *pcb, uint8_t cn, 
		     err_t (* connected)(void *arg, struct rfcomm_pcb_t *tpcb, err_t err));
err_t rfcomm_disconnect(struct rfcomm_pcb_t *pcb);
err_t rfcomm_listen(struct rfcomm_pcb_t *npcb, uint8_t cn, 
		    err_t (* accept)(void *arg, struct rfcomm_pcb_t *pcb, err_t err));
err_t rfcomm_pn(struct rfcomm_pcb_t *pcb,
		err_t (* pn_rsp)(void *arg, struct rfcomm_pcb_t *pcb, err_t err));
err_t rfcomm_test(struct rfcomm_pcb_t *pcb, 
		  err_t (* test_rsp)(void *arg, struct rfcomm_pcb_t *tpcb, err_t err));
err_t rfcomm_msc(struct rfcomm_pcb_t *pcb, uint8_t fc, 
		 err_t (* msc_rsp)(void *arg, struct rfcomm_pcb_t *pcb, err_t err));
err_t rfcomm_rpn(struct rfcomm_pcb_t *pcb, uint8_t br,
	   err_t (* rpn_rsp)(void *arg, struct rfcomm_pcb_t *pcb, err_t err));
err_t rfcomm_uih(struct rfcomm_pcb_t *pcb, uint8_t cn, struct bt_pbuf_t *q);
err_t rfcomm_uih_credits(struct rfcomm_pcb_t *pcb, uint8_t credits, struct bt_pbuf_t *q);
err_t rfcomm_issue_credits(struct rfcomm_pcb_t *pcb, uint8_t credits);
err_t rfcomm_lp_disconnected(struct l2cap_pcb_t *pcb);

#endif

