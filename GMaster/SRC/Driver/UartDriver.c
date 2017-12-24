
// UART 0 Driver
{
  if (!P1_IsReceive())  {
    if ((TLength0 > 0) && (__uart0 == OFF))  {
      __uart0 = ON;
      __DIR0 = TRANSMIT;
      SET_REGISTER(TI); 
    }
  }
}


// UART 1 Driver
{
  if (!P2_IsReceive()) {
    if ((TLength1 > 0) && (__uart1 == OFF)) {
      __uart1 = ON;
      __DIR1 = TRANSMIT;
      SET_REGISTER(TI); 
    }
  }
}