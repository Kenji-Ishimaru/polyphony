#include    "inthandler.h"

void INTERRUPT_Handler1(void *baseaddr_p){
	int x = PP_STATUS;
	if ((x & 4) != 0) {
	  // render dma int
	  PP_VTX_DMA_CTRL = 0;  // clear int
	  #ifdef DUAL_VTX_BUFFER
	  // cache flush
	  cache_flush();
	  #endif
	  render_dma_end = 1;
	}
	if ((x & 2) != 0) {
	  // buffer clear dma int
	  PP_DMA_CTRL = 0;  // clear int
	  dma_end = 1;
	  #ifdef DUAL_VTX_BUFFER
	    // start rendering
	    if (!render_dma_end) {
	      PP_VTX_DMA_CTRL = 1; // render start
	    }
      #endif
	}
	if ((x & 1) != 0) {
      // v int
	  if (render_end) {  // flip
	    x = PP_FRONT_BUFFER;
	    x &= 1;
	    if (x) {
	      PP_FRONT_BUFFER = 0;
	      PP_COLOR_OFFSET    = FB10_ADRS;
	      PP_COLOR_MS_OFFSET = FB11_ADRS ;
	    } else {
	      PP_FRONT_BUFFER = 1;
	      PP_COLOR_OFFSET    = FB00_ADRS;
	      PP_COLOR_MS_OFFSET = FB01_ADRS;
	    }
	    render_end = 0;
	  }
      PP_INT_CLEAR = 0;
	  int_vsync = 1;
 	}
}
XScuGic InterruptController; /* Instance of the Interrupt Controller */
static XScuGic_Config *GicConfig;/* The configuration parameters of the controller */

#define FPGA_INT_ID 61
int ScuGicInterrupt_Init()
{
	int Status;
	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 * */
	printf("ScuGicInterrupt_Init\n");
	Xil_ExceptionInit();

	GicConfig = XScuGic_LookupConfig(XPAR_PS7_SCUGIC_0_DEVICE_ID);
	if (NULL == GicConfig) {
		printf("ScuGicInterrupt_Int return false\n");
		return XST_FAILURE;
	}
	Status = XScuGic_CfgInitialize(&InterruptController, GicConfig,
			GicConfig->CpuBaseAddress);

	if (Status != XST_SUCCESS) {
		printf("ScuGicInterrupt_Int return XST_FAILURE\n");
		return XST_FAILURE;
	}

	/*
	 * Setup the Interrupt System
	 * */

	/*
	 * Connect the interrupt controller interrupt handler to the hardware
	 * interrupt handling logic in the ARM processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
			(Xil_ExceptionHandler) XScuGic_InterruptHandler,
			(void *) &InterruptController);


	/*
	 * Connect a device driver handler that will be called when an
	 * interrupt for the device occurs, the device driver handler performs
	 * the specific interrupt processing for the device
	 */
	Status = XScuGic_Connect(&InterruptController,FPGA_INT_ID,
			(Xil_ExceptionHandler)INTERRUPT_Handler1,
			(void *)&InterruptController);

	XScuGic_Enable(&InterruptController, FPGA_INT_ID);


	/*
	 * Enable interrupts in the ARM
	 */
	Xil_ExceptionEnable();

	XScuGic_SetPriorityTriggerType(&InterruptController, FPGA_INT_ID,
						0xa0, 3);

	if (Status != XST_SUCCESS) {
		printf("ScuGicInterrupt_Int return XST_FAILURE\n");
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}
