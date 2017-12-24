 PMR |= 0x01; /* Enable W77E58 on chip 1K XRAM */
  
 // Initial 
 m10Tick = 0;
 MasterRing = SYS_IDLE;
 MainStatus = STAT_INIT;
 InitialQue(P1FIFOIn);
 InitialQue(P1FIFOOut);
 InitialQue(P2FIFOIn);
 InitialQue(P2FIFOOut);
 WatchDog = 1;
// GetFromQue(P1FIFO,&temp);

 RESET_REGISTER(EA);          /* Disable Interrupt */
 
 InitUART0();
 InitUART1();

 Init_Physical_Timer0();    /* Init Timer0 */
 Start_Timer0();            /* Star Timer0 */
 Enable_Timer0_Interrupt(); /* Enable Timer 0 Interrupt */

 SET_REGISTER(EA);          /* Enable Interrupt */

 InitialSoftTimer(softtimer1);
 InitialSoftTimer(systimer);
 P2TxCount = 0;
 bInitial = TRUE;
 P2PrevTimer = 3; 
 AckID = 1;
 Count = 1;
 bSetPORTA = 0;
//  bWaitingP2ACK = 0;
 bCompleteP1Tx = TRUE;

 // setting I/O
 SET_PORTA(0x00);
 SET_PORTB(0x00);
 SET_PORTC(0x00);