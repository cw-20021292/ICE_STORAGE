/**
 * File : hal_gpio.c
 *
 * Hardware Abstraction Layer
 * Depend on Renesas MCU Chip
*/
//#pragma interrupt INTP3 FlowMeterInnoInterrupt   // Flow Meter Inno
#pragma interrupt INTP4 FlowMeterInterrupt   // Flow Meter
//#pragma interrupt INTP5 FlowMeterNeoInterrupt    // Flow Meter Neo
//#pragma interrupt INTP6 FlowMeterOutInterrupt    // Flow Meter Out
//#pragma interrupt INTP7 TdsInInterrupt           // Tds In

#include "hw.h"
#include "hal_gpio.h"

#include "flow_meter.h"

#if ( CONFIG_INTP3 == 1 )

__interrupt static void FlowMeterInnoInterrupt(void)
{
  CheckFlowMeterInnoIsr();
}

#endif

#if ( CONFIG_INTP4 == 1 )

__interrupt static void FlowMeterInterrupt(void)
{
  CheckFlowMeterIsr();
}

#endif

#if ( CONFIG_INTP5 == 1 )


__interrupt static void FlowMeterNeoInterrupt(void)
{ 
  CheckFlowMeterNeoIsr();
}

#endif

#if ( CONFIG_INTP6 == 1 )

__interrupt static void FlowMeterOutInterrupt(void)
{
  CheckFlowMeterOutIsr();
}

#endif

#if ( CONFIG_INTP7 == 1 )

__interrupt static void TdsInInterrupt(void)
{
  CheckTdsInIsr();
}

#endif
