/*
 * BTProtocol.h
 *
 * Created: 5/5/2015 12:52:26 AM
 *  Author: NMs
 */ 


#ifndef BTPROTOCOL_H_
#define BTPROTOCOL_H_

void initBTProtocol(void);

void BTProtocolReadByte(unsigned char);
void BTTransmitChar(unsigned char);
void BTTransmitMsg(char *, uint8_t);
void BTTransmitMsgU(unsigned char *theString, uint8_t len);
void BTTransmitStr(char *);
void reTransmit(void);
void resetALL(void);
void sendCarStartedSignal();

#endif /* BTPROTOCOL_H_ */