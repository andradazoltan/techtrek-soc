/*
 * RegisterDefs.h
 *
 *  Created on: Feb 6, 2020
 *      Author: Andrada Zoltan
 */

#ifndef REGISTERDEFS_H_
#define REGISTERDEFS_H_

#define RS232_ReceiverFifo (*(volatile unsigned char *)(0xFF210200))
#define RS232_TransmitterFifo (*(volatile unsigned char *)(0xFF210200))
#define RS232_InterruptEnableReg (*(volatile unsigned char *)(0xFF210202))
#define RS232_InterruptIdentificationReg (*(volatile unsigned char *)(0xFF210204))
#define RS232_FifoControlReg (*(volatile unsigned char *)(0xFF210204))
#define RS232_LineControlReg (*(volatile unsigned char *)(0xFF210206))
#define RS232_ModemControlReg (*(volatile unsigned char *)(0xFF210208))
#define RS232_LineStatusReg (*(volatile unsigned char *)(0xFF21020A))
#define RS232_ModemStatusReg (*(volatile unsigned char *)(0xFF21020C))
#define RS232_ScratchReg (*(volatile unsigned char *)(0xFF21020E))
#define RS232_DivisorLatchLSB (*(volatile unsigned char *)(0xFF210200))
#define RS232_DivisorLatchMSB (*(volatile unsigned char *)(0xFF210202))

#define GPS_ReceiverFifo (*(volatile unsigned char *)(0xFF210210))
#define GPS_TransmitterFifo (*(volatile unsigned char *)(0xFF210210))
#define GPS_InterruptEnableReg (*(volatile unsigned char *)(0xFF210212))
#define GPS_InterruptIdentificationReg (*(volatile unsigned char *)(0xFF210214))
#define GPS_FifoControlReg (*(volatile unsigned char *)(0xFF210214))
#define GPS_LineControlReg (*(volatile unsigned char *)(0xFF210216))
#define GPS_ModemControlReg (*(volatile unsigned char *)(0xFF210218))
#define GPS_LineStatusReg (*(volatile unsigned char *)(0xFF21021A))
#define GPS_ModemStatusReg (*(volatile unsigned char *)(0xFF21021C))
#define GPS_ScratchReg (*(volatile unsigned char *)(0xFF21021E))
#define GPS_DivisorLatchLSB (*(volatile unsigned char *)(0xFF210210))
#define GPS_DivisorLatchMSB (*(volatile unsigned char *)(0xFF210212))

#define TouchScreen_ReceiverFifo (*(volatile unsigned char *)(0xFF210230))
#define TouchScreen_TransmitterFifo (*(volatile unsigned char *)(0xFF210230))
#define TouchScreen_InterruptEnableReg (*(volatile unsigned char *)(0xFF210232))
#define TouchScreen_InterruptIdentificationReg (*(volatile unsigned char *)(0xFF210234))
#define TouchScreen_FifoControlReg (*(volatile unsigned char *)(0xFF210234))
#define TouchScreen_LineControlReg (*(volatile unsigned char *)(0xFF210236))
#define TouchScreen_ModemControlReg (*(volatile unsigned char *)(0xFF210238))
#define TouchScreen_LineStatusReg (*(volatile unsigned char *)(0xFF21023A))
#define TouchScreen_ModemStatusReg (*(volatile unsigned char *)(0xFF21023C))
#define TouchScreen_ScratchReg (*(volatile unsigned char *)(0xFF21023E))
#define TouchScreen_DivisorLatchLSB (*(volatile unsigned char *)(0xFF210230))
#define TouchScreen_DivisorLatchMSB (*(volatile unsigned char *)(0xFF210232))

#define WIFI_ReceiverFifo (*(volatile unsigned char *)(0xFF210200))
#define WIFI_TransmitterFifo (*(volatile unsigned char *)(0xFF210200))
#define WIFI_InterruptEnableReg (*(volatile unsigned char *)(0xFF210202))
#define WIFI_InterruptIdentificationReg (*(volatile unsigned char *)(0xFF210204))
#define WIFI_FifoControlReg (*(volatile unsigned char *)(0xFF210204))
#define WIFI_LineControlReg (*(volatile unsigned char *)(0xFF210206))
#define WIFI_ModemControlReg (*(volatile unsigned char *)(0xFF210208))
#define WIFI_LineStatusReg (*(volatile unsigned char *)(0xFF21020A))
#define WIFI_ModemStatusReg (*(volatile unsigned char *)(0xFF21020C))
#define WIFI_ScratchReg (*(volatile unsigned char *)(0xFF21020E))
#define WIFI_DivisorLatchLSB (*(volatile unsigned char *)(0xFF210200))
#define WIFI_DivisorLatchMSB (*(volatile unsigned char *)(0xFF210202))

#define GraphicsCommandReg          (*(volatile unsigned short int *)(0xFF210000))
#define GraphicsStatusReg           (*(volatile unsigned short int *)(0xFF210000))
#define GraphicsX1Reg               (*(volatile unsigned short int *)(0xFF210002))
#define GraphicsY1Reg               (*(volatile unsigned short int *)(0xFF210004))
#define GraphicsX2Reg               (*(volatile unsigned short int *)(0xFF210006))
#define GraphicsY2Reg               (*(volatile unsigned short int *)(0xFF210008))
#define GraphicsColourReg           (*(volatile unsigned short int *)(0xFF21000E))
#define GraphicsBackGroundColourReg     (*(volatile unsigned short int *)(0xFF210010))

#endif /* REGISTERDEFS_H_ */
