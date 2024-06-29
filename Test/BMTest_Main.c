#define BMTEST_MAIN_C
#include "BMDefs.h"
#include "BMTest.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#pragma region subcmd
static const char* FLAGS[] =
{
    "systick", "rxthread", 
};

typedef enum {
    Subcmd_SYSTICK = 0,
    Subcmd_RXTHREAD = 1,
} Subcmd_t;

static int ArgvMatch(int argc, const char* const *argv, const char* flag)
{
    int flaglen = strlen(flag);
    if (argc < 2) return 0;
    const char* argflag = argv[1];
    return
        (strlen(argflag) >= flaglen) &&
        (0 == strncasecmp(argflag, flag, flaglen));
}

static BMStatus_t ArgMatchRun(int argc, const char* const *argv, Subcmd_t subcmd,
    BMStatus_t (*ProgStart)(int argc, const char* const *argv))
{
    BMStatus_t status = BMStatus_SUCCESS;

    if (ArgvMatch(argc, argv, FLAGS[(int)subcmd]))
    {
        return (status = ProgStart(argc, argv)) ? status : BMStatus_SUCCESSBREAK;
    }
    else
    {
        return BMStatus_SUCCESS;
    }
}
#pragma endregion subcmd

BMStatus_t BMDLNodeUT();
BMStatus_t BMRingBufUT();
BMStatus_t BMTickUT();
BMStatus_t BMEvUT();
BMStatus_t BMFSMUT();
BMStatus_t BMSystick(int argc, const char* const *argv);

int main(int argc, const char* *argv)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMTest_START;
    do {
        if (BMStatus_SUCCESS != 
            (status = ArgMatchRun(argc, argv, Subcmd_SYSTICK, BMSystick)))
        {
            if (status == BMStatus_SUCCESSBREAK) break;
            else BMTest_ERRLOGBREAKEX("Fail in BMSystick()");
        }
        if (BMStatus_SUCCESS != (status = BMDLNodeUT()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNodeUT()");
        }
        if (BMStatus_SUCCESS != (status = BMRingBufUT()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMRingBufUT()");
        }
        if (BMStatus_SUCCESS != (status = BMTickUT()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMTickUT()");
        }
        if (BMStatus_SUCCESS != (status = BMEvUT()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMEvUT()");
        }
        if (BMStatus_SUCCESS != (status = BMFSMUT()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMFSMUT()");
        }
    } while (0);
    BMTest_ENDFUNC(status);
    BMTest_END;
    return status ? EXIT_FAILURE : EXIT_SUCCESS; 
}