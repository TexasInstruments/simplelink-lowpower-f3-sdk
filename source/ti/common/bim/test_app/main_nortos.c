#include <stdint.h>
#include <stddef.h>

#include <NoRTOS.h>

#include <ti/drivers/Board.h>

extern void *mainThread(void *arg0);

/*
 *  ======== main ========
 */
int main(void)
{
    Board_init();

    /* Start NoRTOS */
    NoRTOS_start();

    /* Call mainThread function */
    mainThread(NULL);

    while (1) {}
}
