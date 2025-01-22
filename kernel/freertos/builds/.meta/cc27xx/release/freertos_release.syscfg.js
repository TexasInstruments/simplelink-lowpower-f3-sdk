

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

    CCFG.pAppVtorStr = "0x0000D100";
}

/*eslint-enable no-unused-vars*/