

/* ******* Add NoRTOS to the configuration ********* */

let board = system.deviceData.board;

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