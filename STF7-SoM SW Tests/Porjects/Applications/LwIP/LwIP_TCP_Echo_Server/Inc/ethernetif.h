#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"


#define ETH_TXD0_Pin                GPIO_PIN_12
#define ETH_TXD1_Pin                GPIO_PIN_13
#define ETH_TX_EN_Pin               GPIO_PIN_11

#define ETH_RXD0_Pin                GPIO_PIN_4
#define ETH_RXD1_Pin                GPIO_PIN_5

#define ETH_MDC_Pin                 GPIO_PIN_1
#define ETH_MDIO_Pin                GPIO_PIN_2

#define ETH_CRS_DV_Pin              GPIO_PIN_7
#define ETH_REF_CLK_Pin             GPIO_PIN_1

#define ETH_RXER_Pin				GPIO_PIN_2

#define ETH_TXD0_GPIO_Port          GPIOB
#define ETH_TXD1_GPIO_Port          GPIOB
#define ETH_TX_EN_GPIO_Port         GPIOG

#define ETH_RXD0_GPIO_Port          GPIOC
#define ETH_RXD1_GPIO_Port          GPIOC

#define ETH_MDC_GPIO_Port           GPIOC
#define ETH_MDIO_GPIO_Port          GPIOA

#define ETH_CRS_DV_GPIO_Port        GPIOA
#define ETH_REF_CLK_GPIO_Port       GPIOA

#define ETH_RXER_GPIO_Port			GPIOC
/* Exported types ------------------------------------------------------------*/
err_t ethernetif_init(struct netif *netif);
void ethernetif_input(struct netif *netif);
void ethernetif_set_link(struct netif *netif);
void ethernetif_update_config(struct netif *netif);
void ethernetif_notify_conn_changed(struct netif *netif);
#endif
