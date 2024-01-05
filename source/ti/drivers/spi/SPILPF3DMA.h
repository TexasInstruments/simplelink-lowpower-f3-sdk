/*
 * Copyright (c) 2022-2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!*****************************************************************************
 *  @file       SPILPF3DMA.h
 *
 *  @brief      SPI driver implementation for a Low Power F3 device SPI
 *              controller using the UDMA controller.
 *
 * # Driver include #
 *  The SPI header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/SPI.h> #include <ti/drivers/spi/SPILPF3DMA.h> #include
 *  <ti/drivers/dma/UDMALPF3.h>
 *  @endcode
 *
 *  Refer to @ref SPI.h for a complete description of APIs.
 *
 * Note that the user also needs to include the UDMALPF3.h driver since the SPI
 * uses uDMA in order to improve throughput.
 *
 * # Overview #
 * The general SPI API should be used in application code, i.e. SPI_open()
 * should be used instead of SPILPF3DMA_open(). The board file will define the
 * device specific config, and casting in the general API will ensure that the
 * correct device specific functions are called. This is also reflected in the
 * example code in [Use Cases](@ref USE_CASES_SPI_23X0).
 *
 * # General Behavior #
 * Before using SPI on Low Power F3 devices:
 *   - The SPI driver is initialized by calling SPI_init().
 *   - The SPI HW is configured and flags system dependencies (e.g. IOs, power,
 *     etc.) by calling SPI_open().
 *   - The SPI driver makes use of DMA in order to optimize throughput. This is
 *     handled directly by the SPI driver, so the application should never make
 *     any calls directly to the UDMALPF3.h driver.
 *   - This implementation supports queueing multiple transactions in callback
 *     mode. See the @ref USE_CASE_QUEUE_23X0 "queueing example."
 *   - When queueing multiple transactions that should transfer one after the
 *     other, it is recommended to use the driver in 'manual start' mode by
 *     using the #SPILPF3DMA_CMD_SET_MANUAL command. In this mode, the driver
 *     will not start any queued transfers until SPI_control() is called with
 *     the #SPILPF3DMA_CMD_MANUAL_START command. This mode is off by default and
 *     can be disabled by using command #SPILPF3DMA_CMD_CLR_MANUAL. See the
 *     @ref USE_CASE_MANUAL_START_23X0 "Manual Start Example".
 *
 * The following is true for peripheral operation:
 *   - RX overrun IRQ, SPI and UDMA modules are enabled by calling
       SPI_transfer().
 *   - All received bytes are ignored after SPI_open() is called, until the
 *     first SPI_transfer().
 *   - If an RX overrun occurs or if SPI_transferCancel() is called, RX overrun
       IRQ, SPI and UDMA modules are disabled, TX and RX FIFOs are flushed and
       all bytes are ignored.
 *   - After a successful transfer, RX overrun IRQ and SPI module remains
       enabled and UDMA module is disabled. SPI_transfer() must be called again
       before RX FIFO fills up in order to avoid overflow. If the TX buffer
       overflows, zeros will be output. It is safe to call another
       SPI_transfer() from the transfer callback, see [Continuous Peripheral
       Transfer] (@ref USE_CASE_CST_23X0) use case below.
 *   - The SPI driver supports partial return, that can be used if the transfer
 *     size is unknown. If #SPILPF3DMA_CMD_RETURN_PARTIAL_ENABLE is passed to
 *     SPI_control(), the transfer will end when chip select is deasserted. The
 *     #SPI_Transaction.status and the #SPI_Transaction.count will be updated to
 *     indicate whether the transfer ended due to a chip select deassertion and
 *     how many bytes were transferred. See [Peripheral Mode With Return
 *     Partial] (@ref USE_CASE_RP_23X0) use case below.
 *   - When queueing several transactions if the first is a 'short' transaction
 *     (8 or fewer frames), it is required to use
 *     @ref USE_CASE_MANUAL_START_23X0 "Manual Start mode."
 *
 * The following apply for controller operation:
 *   - SPI and UDMA modules are enabled by calling SPI_transfer().
 *   - If the SPI_transfer() succeeds, SPI module is enabled and UDMA module is
       disabled.
 *   - If SPI_transferCancel() is called, SPI and UDMA modules are disabled and
 *     TX and RX FIFOs are flushed.
 *
 * After SPI operation has ended:
 *   - Release system dependencies for SPI by calling SPI_close().
 *
 * The callback function is always called in a SWI context.
 *
 * # Error handling #
 * If an RX overrun occurs during peripheral operation:
 *   - If a transfer is ongoing, all bytes received up until the error occurs
       will be returned, with the error signaled in the #SPI_Transaction.status
       field. RX overrun IRQ, SPI and UDMA modules are then disabled, TX and RX
       FIFOs are flushed and all bytes will be ignored until a new transfer is
       issued.
 *   - If a transfer is not ongoing, RX overrun IRQ, SPI and UDMA modules are
       disabled, TX and RX FIFOs are flushed and all bytes will be ignored until
       a new transfer is issued.
 *
 * # Timeout #
 * Timeout can occur in #SPI_MODE_BLOCKING, there's no timeout in
   #SPI_MODE_CALLBACK. When in #SPI_MODE_CALLBACK, the transfer must be
   cancelled by calling SPI_transferCancel().@n If a timeout happens in either
   #SPI_PERIPHERAL or #SPI_CONTROLLER mode, the receive buffer will contain the
   bytes received up until the timeout occurred. The SPI transaction status will
   be set to #SPI_TRANSFER_FAILED. The SPI transaction count will be set to the
   number of bytes sent/received before timeout. The remaining bytes will be
   flushed from the TX FIFO so that the subsequent transfer can be executed
   correctly. Note that specifying a timeout prevents the driver from performing
   a polling transfer when in peripheral mode.
 *
 * # Power Management #
 * The TI-RTOS power management framework will try to put the device into the
   most power efficient mode whenever possible. Please see the technical
   reference manual for further details on each power mode.
 *
 * The SPILPF3DMA.h driver is setting a power constraint during transfers to
   keep the device out of standby. When the transfer has finished, the power
   constraint is released. The following statements are valid:
 *   - After SPI_open(): the device is still allowed to enter standby.
 *   - In peripheral mode:
 *     - During SPI_transfer(): the device cannot enter standby, only idle.
 *     - After an RX overflow: device is allowed to enter standby.
 *     - After a successful SPI_transfer(): the device is allowed to enter
 *       standby, but SPI module remains enabled.
 *       - _Note_: In peripheral mode, the device might enter standby while a
           byte is being transferred if SPI_transfer() is not called again after
           a successful transfer. This could result in corrupt data being
           transferred.
 *     - Application thread should typically either issue another transfer after
 *       SPI_transfer() completes successfully, or call SPI_transferCancel() to
 *       disable the SPI module and thus assuring that no data is received while
 *       entering standby.
 *   - In controller mode:
 *     - During SPI_transfer(): the device cannot enter standby, only idle.
 *     - After SPI_transfer() succeeds: the device can enter standby.
 *     - If SPI_transferCancel() is called: the device can enter standby.
 *
 *  @note The external hardware connected to the SPI might have some pull
    configured on the SPI lines. When the SPI is inactive, this might cause
    leakage on the IO and the current consumption to increase. The application
    must configure a pull configuration that aligns with the external hardware.
    See [Ensure low power during inactive periods] (@ref USE_CASE_LPWR_23X0) for
    code example.
 *
 *  # SPI details #
 *  ## Chip Select #
 *  This SPI controller supports a hardware chip select pin. Refer to the user
 *  manual on how this hardware chip select pin behaves in regards to the SPI
 *  frame format.
 *
 *  <table>
 *  <tr>
 *  <th>Chip select type</th>
 *  <th>SPI_CONTROLLER mode</th>
 *  <th>SPI_PERIPHERAL mode</th>
 *  </tr>
 *  <tr>
 *  <td>Hardware chip select</td>
 *  <td>No action is needed by the application to select the peripheral.</td>
 *  <td>See the device documentation on it's chip select requirements.</td>
 *  </tr>
 *  <tr>
 *  <td>Software chip select</td>
 *  <td>The application is responsible to ensure that correct SPI peripheral is
 *      selected before performing a SPI_transfer().</td>
 *  <td>See the device documentation on it's chip select requirements.</td>
 *  </tr>
 *  </table>
 *
 *  ### Multiple peripherals when operating in controller mode #
 *  In a scenario where the SPI module is operating in controller mode with
    multiple SPI peripherals, the chip select pin can be reallocated at runtime
    to select the appropriate peripheral device. See [Controller Mode With
    Multiple Peripherals](@ref USE_CASE_MMMS_23X0) use case below. This is only
    relevant when chip select is a hardware chip select. Otherwise the
    application can control the chip select pins directly using the GPIO driver.
 *
 *  ## Data Frames #
 *
 *  SPI data frames can be any size from 4-bits to 16-bits. If the dataSize in
 *  #SPI_Params is greater that 8-bits, then the SPILPF3DMA driver
 *  implementation will assume that the #SPI_Transaction txBuf and rxBuf point
 *  to an array of 16-bit uint16_t elements.
 *
 *  dataSize  | buffer element size |
 *  --------  | ------------------- |
 *  4-8 bits  | uint8_t             |
 *  9-16 bits | uint16_t            |
 *
 *  ## Bit Rate ##
 *  When the SPI is configured as SPI peripheral, the maximum bit rate is 8MHz.
 *
 *  When the SPI is configured as SPI controller, the maximum bit rate is 12MHz.
 *
 *
 *  ## UDMA #
 *  ### Interrupts #
 *  The UDMA module generates IRQs on the SPI interrupt vector. This driver
    automatically installs a UDMA aware Hwi (interrupt) to service the assigned
    UDMA channels.
 *
 *  ### Transfer Size Limit #
 *
 *  The UDMA controller only supports data transfers of up to 1024 data frames.
 *  A transfer with more than 1024 frames will be transmitted/received in
 *  multiple 1024 sized portions until all data has been transmitted/received. A
 *  data frame can be 4 to 16 bits in length.
 *
 *  ### Scratch Buffers #
 *  A uint16_t scratch buffer is used to allow SPI_transfers where txBuf or
    rxBuf are NULL. Rather than requiring txBuf or rxBuf to have a dummy buffer
    of size of the transfer count, a single-word UDMA accessible uint16_t
    scratch buffer is used. When rxBuf is NULL, the UDMA will transfer all the
    received SPI data into the scratch buffer as a "bit-bucket". When txBuf is
    NULL, the scratch buffer is initialized to defaultTxBufValue so the uDMA
    will send some known value. Each SPI driver instance uses its own scratch
    buffer.
 *
 *  ### TX and RX buffers #
 *  Before SPI_transfer, txBuf should be filled with the outgoing SPI data.
    These data are sent out during the transfer, while the incoming data are
    received into rxBuf. To save memory space, txBuf and rxBuf can be assigned
    to the same buffer location. At the beginning of the transfer, this buffer
    holds outgoing data. At the end of the transfer, the outgoing data are
    overwritten and the buffer holds the received SPI data.
 *
 *  ## Polling SPI transfers #
 *  When used in blocking mode small SPI transfers are can be done by polling
 *  the peripheral & sending data frame-by-frame. A controller device can
 *  perform the transfer immediately and return, but a peripheral will block
 *  until it receives the number of frames specified in the SPI_Transfer() call.
 *  The minDmaTransferSize field in the hardware attributes is the threshold; if
 *  the transaction count is below the threshold a polling transfer is
 *  performed; otherwise a DMA transfer is done.  This is intended to reduce the
 *  overhead of setting up a DMA transfer to only send a few data frames.
 *
 *  Notes:
 *  - Specifying a timeout prevents peripheral devices from using polling
      transfers.
 *  - Keep in mind that during polling transfers the current task is still being
 *    executed; there is no context switch to another task.
 *
 * # Supported Functions #
 *  Generic API function  | API function                   | Description
 *  ----------------------|------------------------------- |------------------------------------------------------------
 *  SPI_init()            | SPICC26X4DMA_init()            | Initialize SPI driver
 *  SPI_open()            | SPICC26X4DMA_open()            | Initialize SPI HW and set system dependencies
 *  SPI_close()           | SPICC26X4DMA_close()           | Disable SPI and UDMA HW and release system dependencies
 *  SPI_control()         | SPICC26X4DMA_control()         | Configure an already opened SPI handle
 *  SPI_transfer()        | SPICC26X4DMA_transfer()        | Start transfer from SPI
 *  SPI_transferCancel()  | SPICC26X4DMA_transferCancel()  | Cancel ongoing transfer from SPI
 *
 *  @note All calls should go through the generic API
 *
 * ## Use Cases @anchor USE_CASES_SPI_23X0 ##
 * ### Basic Peripheral Mode #
 *  Receive 100 bytes over SPI in #SPI_MODE_BLOCKING.
 *  @code
 *  SPI_Handle handle; SPI_Params params; SPI_Transaction transaction; uint8_t
 *  rxBuf[100];     // Receive buffer
 *
 *  // Init SPI and specify non-default parameters SPI_Params_init(&params);
 *  params.bitRate     = 1000000; params.frameFormat = SPI_POL1_PHA1;
 *  params.mode        = SPI_PERIPHERAL;
 *
 *  // Configure the transaction transaction.count = 100; transaction.txBuf =
 *  NULL; transaction.rxBuf = rxBuf;
 *
 *  // Open the SPI and perform the transfer handle = SPI_open(CONFIG_SPI,
 *  &params); SPI_transfer(handle, &transaction);
 *  @endcode
 *
 * ### Peripheral Mode With Return Partial @anchor USE_CASE_RP_23X0 #
 *  This use case will perform a transfer in #SPI_MODE_BLOCKING until the wanted
    amount of bytes is transferred or until chip select is deasserted by the SPI
    controller. This SPI_transfer() call can be used when unknown amount of
    bytes shall be transferred. Note: The partial return is also possible in
    #SPI_MODE_CALLBACK mode. Note: Polling transfers are not available when
    using return partial mode.
 *  @code
 *  SPI_Handle handle; SPI_Params params; SPI_Transaction transaction; uint8_t
 *  rxBuf[100];     // Receive buffer
 *
 *  // Init SPI and specify non-default parameters SPI_Params_init(&params);
 *  params.bitRate     = 1000000; params.frameFormat = SPI_POL1_PHA1;
 *  params.mode        = SPI_PERIPHERAL;
 *
 *  // Configure the transaction transaction.count = 100; transaction.txBuf =
 *  NULL; transaction.rxBuf = rxBuf;
 *
 *  // Open the SPI and initiate the partial read handle = SPI_open(CONFIG_SPI,
 *  &params);
 *
 *  // Enable RETURN_PARTIAL SPI_control(handle,
 *  SPILPF3DMA_RETURN_PARTIAL_ENABLE, NULL);
 *
 *  // Begin transfer SPI_transfer(handle, &transaction);
 *  @endcode
 *
 * ### Continuous Peripheral Transfer In #SPI_MODE_CALLBACK @anchor USE_CASE_CST_23X0 #
 *  This use case will configure the SPI driver to transfer continuously in
 *  #SPI_MODE_CALLBACK, 16 bytes at the time and echoing received data after
 *  every 16 bytes.
 *  @code
 *  // Callback function static void transferCallback(SPI_Handle handle,
 *  SPI_Transaction *transaction)
 *  {
 *      // Start another transfer
 *      SPI_transfer(handle, transaction);
 *  }
 *
 *  static void taskFxn(uintptr_t a0, uintptr_t a1)
 *  {
 *      SPI_Handle handle;
 *      SPI_Params params;
 *      SPI_Transaction transaction;
 *      uint8_t buf[16];                  // Receive and transmit buffer
 *
 *      // Init SPI and specify non-default parameters
 *      SPI_Params_init(&params);
 *      params.bitRate             = 1000000;
 *      params.frameFormat         = SPI_POL1_PHA1;
 *      params.mode                = SPI_PERIPHERAL;
 *      params.transferMode        = SPI_MODE_CALLBACK;
 *      params.transferCallbackFxn = transferCallback;
 *
 *      // Configure the transaction
 *      transaction.count = 16;
 *      transaction.txBuf = buf;
 *      transaction.rxBuf = buf;
 *
 *      // Open the SPI and initiate the first transfer
 *      handle = SPI_open(CONFIG_SPI, &params);
 *      SPI_transfer(handle, &transaction);
 *
 *      // Wait forever
 *      while(true);
 *  }
 *  @endcode
 *
 * ### Basic Controller Mode #
 *  This use case will configure a SPI controller to send the data in txBuf
    while receiving data to rxBuf in BLOCKING_MODE.
 *  @code
 *  SPI_Handle handle; SPI_Params params; SPI_Transaction transaction; uint8_t
 *  txBuf[] = "Hello World";    // Transmit buffer uint8_t rxBuf[11];
 *  // Receive buffer
 *
 *  // Init SPI and specify non-default parameters SPI_Params_init(&params);
 *  params.bitRate     = 1000000; params.frameFormat = SPI_POL1_PHA1;
 *  params.mode        = SPI_CONTROLLER;
 *
 *  // Configure the transaction transaction.count = sizeof(txBuf);
 *  transaction.txBuf = txBuf; transaction.rxBuf = rxBuf;
 *
 *  // Open the SPI and perform the transfer handle = SPI_open(CONFIG_SPI,
 *  &params); SPI_transfer(handle, &transaction);
 *  @endcode
 *
 *  ### Controller Mode With Multiple Peripherals @anchor USE_CASE_MMMS_23X0 #
 *  This use case will configure a SPI controller to send data to one peripheral
    and then to another in BLOCKING_MODE. It is assumed that SysConfig is
    configured so that the two chip select pins have a default setting of a high
    output and that the #SPILPF3DMA_HWAttrs used points to one of them since the
    SPI driver will revert to this default setting when switching the chip
    select pin.
 *
 *  @code
 *  // From ti_drivers_config.c // Use the sysconfig settings to make sure both
 *  pins are set to HIGH when not in use GPIO_PinConfig gpioPinConfigs[31] = {
 *      ...
 *      GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH, // CONFIG_CSN_0
 *      ...
 *      GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH, // CONFIG_CSN_1
 *  }
 *
 *  const SPILPF3DMA_HWAttrs SPILPF3DMAHWAttrs[CONFIG_SPI_COUNT] = { {   // Use
 *  SPI0 module with default chip select on CONFIG_CSN_0 .baseAddr = SPI0_BASE,
 *  .intNum = INT_SPI0_COMB, .intPriority = (~0), .swiPriority = 0, .powerID =
 *  PowerLPF3_PERIPH_SPI0, .defaultTxBufValue = 0xFF, .rxChannelBitMask =
 *  UDMA_CHANNEL_1_M, .txChannelBitMask = UDMA_CHANNEL_0_M, .rxChannelEvtMux =
 *  EVTSVT_DMACH1SEL_IPID_SPI0RXTRG, .txChannelEvtMux =
 *  EVTSVT_DMACH0SEL_IPID_SPI0TXTRG, .dmaTxTableEntryPri =
 *  &dmaChannel0ControlTableEntry, .dmaRxTableEntryPri =
 *  &dmaChannel1ControlTableEntry, .dmaTxTableEntryAlt =
 *  &dmaChannel0AltControlTableEntry, .dmaRxTableEntryAlt =
 *  &dmaChannel1AltControlTableEntry, .minDmaTransferSize = 10, .picoPinMux =
 *  GPIO_MUX_PORTCFG_PFUNC2, .pociPinMux   = GPIO_MUX_PORTCFG_PFUNC1,
 *  .sclkPinMux  = GPIO_MUX_PORTCFG_PFUNC4, .csnPinMux  =
 *  GPIO_MUX_PORTCFG_PFUNC1, .picoPin = CONFIG_GPIO_SPI_CONTROLLER_PICO,
 *  .pociPin = CONFIG_GPIO_SPI_CONTROLLER_POCI, .sclkPin  =
 *  CONFIG_GPIO_SPI_CONTROLLER_SCLK, .csnPin  = CONFIG_SPI_CSN_0,
 *  }
 *
 *  // From your_application.c static void taskFxn(uintptr_t a0, uintptr_t a1)
 *  {
 *      SPI_Handle handle;
 *      SPI_Params params;
 *      SPI_Transaction transaction;
 *      uint_least8_t csnPin1 = CONFIG_CSN_1;
 *      uint8_t txBuf[] = "Hello World";    // Transmit buffer
 *
 *      // Init SPI and specify non-default parameters
 *      SPI_Params_init(&params);
 *      params.bitRate     = 1000000;
 *      params.frameFormat = SPI_POL1_PHA1;
 *      params.mode        = SPI_CONTROLLER;
 *
 *      // Configure the transaction
 *      transaction.count = sizeof(txBuf);
 *      transaction.txBuf = txBuf;
 *      transaction.rxBuf = NULL;
 *
 *      // Open the SPI and perform transfer to the first peripheral
 *      handle = SPI_open(CONFIG_SPI, &params);
 *      SPI_transfer(handle, &transaction);
 *
 *      // Then switch chip select pin and perform transfer to the second
        peripheral
        SPI_control(handle, SPILPF3DMA_SET_CSN_PIN, &csnPin1);
        SPI_transfer(handle, &transaction);
 *  }
 *  @endcode
 *
 *  ### Queueing Transactions in Callback Mode #
 *  @anchor USE_CASE_QUEUE_23X0 Below is an example of queueing three
 *  transactions
 *  @code
 *  // SPI already opened in callback mode SPI_Transaction t0, t1, t2;
 *
 *  t0.txBuf = txBuff0; t0.rxBuf = rxBuff0; t0.count = 2000;
 *
 *  t1.txBuf = txBuff1; t1.rxBuf = rxBuff1; t1.count = 1000;
 *
 *  t2.txBuf = txBuff2; t2.rxBuf = NULL; t2.count = 1000;
 *
 *  bool transferOk = false;
 *
 *  if (SPI_transfer(spiHandle, &t0)) { if (SPI_transfer(spiHandle, &t1)) {
 *      transferOk = SPI_transfer(spiHandle, &t2);
 *          }
 *      }
 *  }
 *  @endcode
 *
 *  ### Queueing in Manual Start Mode#
 *  This example shows a peripheral device queueing two transactions that will
 *  complete one after the other. From the controller's perspective there will
 *  be one long transfer.
 *  @note Manual mode also works while the device is in #SPI_CONTROLLER mode.
    The control call to MANUAL_START will start the transfers.
 *
 *  @warning Manual start mode should not be enabled or disabled while a
 *  transaction is in progress.
 *
 *  @anchor USE_CASE_MANUAL_START_23X0
 *  @code
 *  SPI_Handle spi; SPI_Params params; SPI_Transaction t0, t1; uint8_t status =
 *  SPI_STATUS_SUCCESS;
 *
 *  SPI_Params_init(&params); params.mode = SPI_PERIPHERAL; spi =
 *  SPI_open(CONFIG_SPI, &params);
 *
 *  if (spi == NULL) { exit(0);
 *  }
 *
 *  // Enable manual start mode SPI_control(spi, SPILPF3DMA_CMD_SET_MANUAL,
 *  NULL);
 *
 *  // Queue transactions t0.txBuf = txBuff0; t0.rxBuf = rxBuff0; t0.count =
 *  2000; if (!SPI_transfer(spi, &t0)) { status = SPI_STATUS_FAIL;
 *  }
 *
 *  t1.txBuf = txBuff1; t1.rxBuf = rxBuff1; t1.count = 1000; if
 *  (!SPI_transfer(spi, &t1)) { status = SPI_STATUS_FAIL;
 *  }
 *
 *  // Enable the transfers if (status == SPI_STATUS_SUCCESS) { SPI_control(spi,
 *  SPILPF3DMA_CMD_MANUAL_START, NULL);
 *  }
 *  else { status = SPI_STATUS_FAILURE;
 *  }
 *
 *  // At this point the peripheral is ready for the controller to start the
    transfer // Assume the callback implementation (not shown) posts a semaphore
    when // the last transaction completes sem_wait(&spiSemaphore);
 *
 *  // Disable manual start mode SPI_control(spi, SPILPF3DMA_CMD_CLR_MANUAL,
 *  NULL);
 *
 *  @endcode
 *
 *  ### Ensure low power during inactive periods @anchor USE_CASE_LPWR_23X0 #
 *  External hardware connected on the SPI, i.e. SPI controller/peripheral,
    might have configured a pull on one or more of the SPI lines. Dependent on
    the hardware, it might conflict with the pull used for the Low Power F3
    device SPI. To avoid increased leakage and ensure the lowest possible power
    consumption when the SPI is inactive, the application must configure a
    matching pull on the SPI IOs. An example of how this can be done is shown
    below.
 *
 *  @code
 *  SPI_Params params; SPI_Transaction transaction; uint8_t txBuf[] =
 *  "Heartbeat";    // Transmit buffer uint8_t rxBuf[9];                 //
 *  Receive buffer uint32_t standbyDurationMs = 100;
 *
 *  // Init SPI and specify non-default parameters SPI_Params_init(&params);
 *  params.bitRate     = 1000000; params.frameFormat = SPI_POL1_PHA1;
 *  params.mode        = SPI_CONTROLLER;
 *
 *  // Configure the transaction transaction.count = sizeof(txBuf);
 *  transaction.txBuf = txBuf; transaction.rxBuf = rxBuf;
 *
 *  // Open the SPI and perform the transfer handle = SPI_open(CONFIG_SPI_0,
 *  &params);
 *
 *  // Apply low power sleep pull config for POCI
 *  GPIO_setConfig(CONFIG_GPIO_SPI_0_POCI, GPIO_CFG_IN_PU);
 *
 *  // Do forever while(1) { // Transfer data SPI_transfer(handle,
 *  &transaction); // Sleep Task_sleep(standbyDurationMs*100);
 *  }
 *  @endcode
 *
 *  ### Wake Up On Chip Select Deassertion In Peripheral Mode Using #SPI_MODE_CALLBACK #
 *  This example demonstrates using a GPIO callback on Chip Select to wake up
    the device to allow low power modes while waiting for a chip select edge.
 *
 *  In sysconfig or the board file, the CSN GPIO should be configured as
 *  input/pull up with an interrupt on falling edge. Otherwise, SPI_close() will
 *  reset the pin to the wrong settings and you may see line glitches.
 *
 *  *Note: The SPI controller must allow enough time between deasserting the
    chip select and the start of the transaction for the SPI peripheral to wake
    up and open up the SPI driver.
 *
 *  @code
 *  // Global variables SPI_Handle spiHandle SPI_Params spiParams;
 *  SPI_Transaction spiTransaction; const uint8_t transferSize = 8; uint8_t
 *  txBuf[8];
 *
 *  // Chip select callback static void chipSelectCallback(uint_least8_t)
 *  {
 *      // Open SPI driver, which will override any previous GPIO configuration
 *      spiHandle = SPI_open(CONFIG_SPI, &spiParams);
 *      // Issue the transfer
 *      SPI_transfer(spiHandle, &spiTransaction);
 *  }
 *
 *  // SPI transfer callback static void transferCallback(SPI_Handle handle,
 *  SPI_Transaction *transaction)
 *  {
 *      // Close the SPI driver
 *      SPI_close(handle);
 *
 *      // Note: SPI_close() will reset the pin configuration, so it is
        important to
        // set the default values correctly in sysconfig. We just need to set
        the
        // callback and enable the falling edge interrupt
 *
 *      GPIO_setCallback(CS_PIN_INDEX, chipSelectCallback);
 *      GPIO_enableInt(CS_PIN_INDEX);
 *  }
 *
 *  // From your_application.c static void taskFxn(uintptr_t a0, uintptr_t a1)
 *  {
 *      uint8_t i;
 *
 *      // Setup SPI params
 *      SPI_Params_init(&spiParams);
 *      spiParams.bitRate     = 1000000;
 *      spiParams.frameFormat = SPI_POL1_PHA1;
 *      spiParams.mode        = SPI_PERIPHERAL;
 *      spiParams.dataSize    = transferSize;
 *      spiParams.transferMode = SPI_MODE_CALLBACK;
 *      spiParams.transferCallbackFxn = transferCallback;
 *
 *      // Setup SPI transaction
 *      spiTransaction.arg = NULL;
 *      spiTransaction.count = transferSize;
 *      spiTransaction.txBuf = txBuf;
 *      spiTransaction.rxBuf = txBuf;
 *
 *      // First echo message
 *      for (i = 0; i < transferSize; i++) {
 *          txBuf[i] = i;
 *      }
 *
 *      // Configure chip select callback
 *      GPIO_setCallback(CS_PIN_INDEX, chipSelectCallback);
 *      GPIO_enableInt(CS_PIN_INDEX);
 *
 *      // Wait forever
 *      while(true);
 *  }
 *  @endcode
 *
 *  <hr>
 */

#ifndef ti_drivers_spi_SPILPF3DMA__include
#define ti_drivers_spi_SPILPF3DMA__include

#include <stdint.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/gpio/GPIOLPF3.h>
#include <ti/drivers/dma/UDMALPF3.h>
#include <ti/drivers/Power.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/SwiP.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @addtogroup SPI_STATUS
 *  SPILPF3DMA_STATUS_* macros are command codes only defined in the
 *  SPILPF3DMA.h driver implementation and need to:
 *  @code
 *  #include <ti/drivers/spi/SPILPF3DMA.h>
 *  @endcode
 *  @{
 */

/* Add SPILPF3DMA_STATUS_* macros here */

/** @}*/

/**
 *  @addtogroup SPI_CMD
 *  SPILPF3DMA_CMD_* macros are command codes only defined in the
 *  SPILPF3DMA.h driver implementation and need to:
 *  @code
 *  #include <ti/drivers/spi/SPILPF3DMA.h>
 *  @endcode
 *  @{
 */

/*!
 * @brief Command used by SPI_control() to enable partial return
 *
 * Enabling this command allows SPI_transfer to return partial data if the
 * controller de-asserts the CS line before the expected number of frames were
 * received. This command @b arg is of type @a don't @a care and it returns
 * SPI_STATUS_SUCCESS or SPI_STATUS_ERROR.
 */
#define SPILPF3DMA_CMD_RETURN_PARTIAL_ENABLE (SPI_CMD_RESERVED + 0)

/*!
 * @brief Command used by SPI_control() to disable partial return
 *
 * Disabling this command returns the SPILPF3DMA to the default blocking
 * behavior where SPI_transfer blocks until all data bytes were received. With
 * this command @b arg is @a don't @a care and it returns #SPI_STATUS_SUCCESS.
 */
#define SPILPF3DMA_CMD_RETURN_PARTIAL_DISABLE (SPI_CMD_RESERVED + 1)

/*!
 * @brief Command used by SPI_control() to re-configure chip select pin
 *
 * This command specifies a chip select pin in @b arg with type @c uint_least8_t.
 * It always returns #SPI_STATUS_SUCCESS.
 */
#define SPILPF3DMA_CMD_SET_CSN_PIN (SPI_CMD_RESERVED + 2)

/*!
 * @brief Command used by SPI_control() to clear the chip select pin
 *
 * This command @b arg should be NULL. It always returns #SPI_STATUS_SUCCESS.
 */
#define SPILPF3DMA_CMD_CLEAR_CSN_PIN (SPI_CMD_RESERVED + 3)

/*!
 * @brief Command used by SPI_control() to enable manual start mode
 *
 * Manual start mode can only be used when in callback mode. In manual start
 * mode, calls to SPI_transfer() queue the transaction but does not start the
 * transfer until another control call is made with
 * #SPILPF3DMA_CMD_MANUAL_START. This allows multiple transactions to be
 * queued and executed seamlessly using the DMA's ping pong mechanism. This
 * mode is MANDATORY for peripherals queueing multiple short transactions. Manual
 * start mode can only be enabled or disabled when no transactions are queued.
 *
 * Returns #SPI_STATUS_SUCCESS or #SPI_STATUS_ERROR.
 */
#define SPILPF3DMA_CMD_SET_MANUAL (SPI_CMD_RESERVED + 4)

/*!
 * @brief Command used by SPI_control() to disable manual start mode
 *
 * Manual start mode is disabled by default. Enabling and disabling manual mode
 * can only be done if no transactions are currently queued.
 *
 * Returns #SPI_STATUS_SUCCESS or #SPI_STATUS_ERROR.
 *
 */
#define SPILPF3DMA_CMD_CLR_MANUAL (SPI_CMD_RESERVED + 5)

/*!
 * @brief Command used by SPI_control() to enable manual start mode
 *
 * This command is used with manual start mode enabled. If transactions have
 * been queued and the driver is in manual mode, this command will enable the
 * SSI and DMA. For controller devices, the transfer will start. For peripheral devices,
 * the transfer will start when the controller initiates.
 *
 * Returns #SPI_STATUS_SUCCESS or #SPI_STATUS_ERROR.
 */
#define SPILPF3DMA_CMD_MANUAL_START (SPI_CMD_RESERVED + 6)

/*!
 * @brief Command used by SPI_control() to set the sample delay in controller mode
 *
 * This command is only used when in controller mode. After SPI_open() is called,
 * the default DSAMPLE value is set as follows.
 *   * (bitRate <  4MHz) -> delay = 0
 *   * (bitRate >= 4MHz) -> delay = 1
 *   * (bitRate >= 8MHz) -> delay = 2
 *
 * The sample delay is a measurement of clock cycles. In controller mode the data
 * on the input pin will delay sampling by the defined clock cycles. At lower
 * bitRates a delay is usually not necessary. At higher bitRates a delay may
 * become necessary if the peripheral device is not keeping pace with the controller
 * device. In this case, the delay can be used to ensure the controller device
 * captures the data on the input pin.
 * This command @b arg is of type uint8_t.
 *
 * Returns #SPI_STATUS_SUCCESS or #SPI_STATUS_ERROR.
 */
#define SPILPF3DMA_CMD_SET_SAMPLE_DELAY (SPI_CMD_RESERVED + 7)

/** @}*/

/* BACKWARDS COMPATIBILITY */
#define SPILPF3DMA_RETURN_PARTIAL_ENABLE  SPILPF3DMA_CMD_RETURN_PARTIAL_ENABLE
#define SPILPF3DMA_RETURN_PARTIAL_DISABLE SPILPF3DMA_CMD_RETURN_PARTIAL_DISABLE
#define SPILPF3DMA_SET_CSN_PIN            SPILPF3DMA_CMD_SET_CSN_PIN
/* END BACKWARDS COMPATIBILITY */

/*!
 *  @internal
 *  @brief
 *  SPI function table pointer
 */
extern const SPI_FxnTable SPILPF3DMA_fxnTable;

/*!
 *  @internal
 *  @brief
 *  SPILPF3DMA data frame size is used to determine how to configure the
 *  UDMA data transfers. This field is to be only used internally.
 *
 *  - SPILPF3DMA_8bit:  txBuf and rxBuf are arrays of uint8_t elements
 *  - SPILPF3DMA_16bit: txBuf and rxBuf are arrays of uint16_t elements
 */
typedef enum
{
    SPILPF3DMA_8bit  = 0,
    SPILPF3DMA_16bit = 1
} SPILPF3DMA_FrameSize;

/*!
 *  @internal
 *  @brief
 *  SPILPF3DMA return partial field indicates the status of the return
 *  partial mode and the associated pin interrupt. This field is for internal
 *  use only.
 */
typedef enum
{
    SPILPF3DMA_retPartDisabled         = 0,
    SPILPF3DMA_retPartEnabledIntNotSet = 1,
    SPILPF3DMA_retPartEnabledIntSet    = 2
} SPILPF3DMA_ReturnPartial;

/*!
 *  @brief  SPILPF3DMA Hardware attributes
 *
 *  These fields, with the exception of intPriority,
 *  are used by driverlib APIs and therefore must be populated by
 *  driverlib macro definitions. For driverlib these definitions are found in:
 *      - inc/hw_memmap.h
 *      - inc/hw_ints.h
 *      - driverlib/udma.h
 *
 *  intPriority is the SPI peripheral's interrupt priority, as defined by the
 *  underlying OS.  It is passed unmodified to the underlying OS's interrupt
 *  handler creation code, so you need to refer to the OS documentation
 *  for usage.  For example, for SYS/BIOS applications, refer to the
 *  ti.sysbios.family.arm.m3.Hwi documentation for SYS/BIOS usage of
 *  interrupt priorities.  If the driver uses the ti.dpl interface
 *  instead of making OS calls directly, then the HwiP port handles the
 *  interrupt priority in an OS specific way.  In the case of the SYS/BIOS
 *  port, intPriority is passed unmodified to Hwi_create().
 *
 *  A sample structure is shown below:
 *  @code
 *  const SPILPF3DMA_HWAttrs SPILPF3DMAobjects[] = {
 *      {
 *          .baseAddr = SPI0_BASE,
 *          .intNum = INT_SPI0,
 *          .intPriority = ~0,
 *          .swiPriority = 0,
 *          .powerMngrId = PERIPH_SPI0,
 *          .defaultTxBufValue = 0,
 *          .rxChannelBitMask = UDMA_CHAN_SPI0_RX,
 *          .txChannelBitMask = UDMA_CHAN_SPI0_TX,
 *          .picoPin = CONFIG_SPI0_POCI,
 *          .pociPin = CONFIG_SPI0_PICO,
 *          .clkPin = CONFIG_SPI0_CLK,
 *          .csnPin = CONFIG_SPI0_CSN
 *      },
 *      {
 *          .baseAddr = SPI1_BASE,
 *          .intNum = INT_SPI1,
 *          .intPriority = ~0,
 *          .swiPriority = 0,
 *          .powerMngrId = PERIPH_SPI1,
 *          .defaultTxBufValue = 0,
 *          .rxChannelBitMask = UDMA_CHAN_SPI1_RX,
 *          .txChannelBitMask = UDMA_CHAN_SPI1_TX,
 *          .picoPin = CONFIG_SPI1_POCI,
 *          .pociPin = CONFIG_SPI1_PICO,
 *          .clkPin = CONFIG_SPI1_CLK,
 *          .csnPin = CONFIG_SPI1_CSN
 *      },
 *  };
 *  @endcode
 */
typedef struct
{
    /*! @brief SPI Peripheral's base address */
    uint32_t baseAddr;
    /*! @brief SPI SWI priority.
        The higher the number, the higher the priority.
        The minimum is 0 and the maximum is 15 by default.
        The maximum can be reduced to save RAM by adding or modifying
        Swi.numPriorities in the kernel configuration file.
    */
    uint32_t swiPriority;
    /*! uDMA controlTable channel index */
    uint32_t rxChannelBitMask;
    /*! uDMA controlTable channel index */
    uint32_t txChannelBitMask;
    /*! uDMA controlTable primary tx entry */
    volatile uDMAControlTableEntry *dmaTxTableEntryPri;
    /*! uDMA controlTable primary tx entry */
    volatile uDMAControlTableEntry *dmaRxTableEntryPri;
    /*! uDMA controlTable alternate tx entry */
    volatile uDMAControlTableEntry *dmaTxTableEntryAlt;
    /*! uDMA controlTable alternate rx entry */
    volatile uDMAControlTableEntry *dmaRxTableEntryAlt;
    /*! POCI PIN mux value. Can be applied to either PICO or POCI */
    int32_t pociPinMux;
    /*! PICO PIN mux value. Can be applied to either PICO or POCI */
    int32_t picoPinMux;
    /*! SCLK PIN mux value for flow control */
    int32_t sclkPinMux;
    /*! CSN PIN mux value for flow control */
    int32_t csnPinMux;
    /*! Minimum transfer size for DMA based transfer */
    uint32_t minDmaTransferSize;
    /*! Power driver ID for this SPI instance */
    PowerLPF3_Resource powerID;
    /*! Default TX value if txBuf == NULL */
    uint16_t defaultTxBufValue;
    /*! DMA Mux ID for this SPI TX channel */
    uint8_t txChannelEvtMux;
    /*! DMA Mux ID for this SPI RX channel */
    uint8_t rxChannelEvtMux;
    /*! SPILPF3DMA Peripheral's interrupt vector */
    uint8_t intNum;
    /*! @brief SPILPF3DMA Peripheral's interrupt priority.

        The Low Power F3 devices use either three or two priority bits,
        depending on the device. That means ~0 has the same effect as (7 << 5)
        or (3 << 6), respectively.

        Setting the priority to 0 is not supported by this driver.

        HWI's with priority 0 ignore the HWI dispatcher to support zero-latency
        interrupts, thus invalidating the critical sections in this driver.
    */
    uint8_t intPriority;
    /*! SPI PICO pin */
    uint_least8_t picoPin;
    /*! SPI POCI pin */
    uint_least8_t pociPin;
    /*! SPI SCLK pin */
    uint_least8_t sclkPin;
    /*! SPI CSN pin */
    uint_least8_t csnPin;
} SPILPF3DMA_HWAttrs;

/*!
 *  @brief  SPILPF3DMA Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct
{
    HwiP_Struct hwi;
    Power_NotifyObj spiPostObj;
    SwiP_Struct swi;
    SemaphoreP_Struct transferComplete;

    SPI_CallbackFxn transferCallbackFxn;
    SPI_Transaction *headPtr;
    SPI_Transaction *tailPtr;
    SPI_Transaction *completedTransfers;

    size_t framesQueued;
    size_t framesTransferred;
    size_t priTransferSize;
    size_t altTransferSize;

    uint32_t activeChannel;
    uint32_t bitRate;
    uint32_t dataSize;
    uint32_t transferTimeout;
    uint32_t busyBit;
    uint32_t dsample;

    uint16_t rxScratchBuf;
    uint16_t txScratchBuf;

    SPI_TransferMode transferMode;
    SPI_Mode mode;
    uint8_t format;
    uint_least8_t csnPin;
    SPILPF3DMA_ReturnPartial returnPartial;
    bool isOpen;
    bool manualStart;
} SPILPF3DMA_Object;

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_spi_SPILPF3DMA__include */
