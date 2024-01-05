

/* ******* Add FreeRTOS to the configuration ********* */

/*eslint-disable no-unused-vars*/

/* ================ Kernel (FreeRTOS) configuration ================ */

let board = system.deviceData.board;
let isFPGA = false;

/* Users may be using SysConfig with no provided board */
if (board) {
    isFPGA = board.name.match(/FPGA/);
}

const FreeRTOS = scripting.addModule("/freertos/FreeRTOS");

if (isFPGA) {
    FreeRTOS.cpuFrequency = 24000000;
}

const POSIX = scripting.addModule("/ti/posix/freertos/Settings");

if (system.modules["/ti/utils/TrustZone"]) {
    /*
     * Enable the secure callback module
     */
    const SecureCallback = scripting.addModule("/ti/drivers/tfm/SecureCallback");

    /* Set Secure image reset vector table address */
    const CCFG = scripting.addModule("/ti/devices/CCFG");

    CCFG.setFlashVectorTable = true;
    CCFG.addressFlashVectorTable = 0x00006900;
}

/* If the board has the Rev A HW Workaround, configure the PMOS CTL pin to
 * be pulled up.
 */
if (board && board.components.PMOS_CTL)
{
    const GPIO          = scripting.addModule("/ti/drivers/GPIO");
    const PMOS_CTL_GPIO = GPIO.addInstance();
    PMOS_CTL_GPIO.$name = "CONFIG_GPIO_PMOS_CTL";
    PMOS_CTL_GPIO.mode  = "Input";
    PMOS_CTL_GPIO.pull  = "Pull Up";
    PMOS_CTL_GPIO.$hardware = board.components.PMOS_CTL;
}


/*eslint-enable no-unused-vars*/