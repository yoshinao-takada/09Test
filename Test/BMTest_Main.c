#define BMTEST_MAIN_C
#include "BMDefs.h"
#include "BMTest.h"
#include <stdlib.h>

BMStatus_t BMDLNodeUT();

static int _argc;
static const char* const *_argv;

int ARGC() { return _argc; }

const char* const * ARGV() { return _argv; }

int main(int argc, const char* *argv)
{
    BMStatus_t status = BMStatus_SUCCESS;
    _argc = argc;
    _argv = argv;
    BMTest_START;
    do {
        if (BMStatus_SUCCESS != (status = BMDLNodeUT()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNodeUT()");
        }
    } while (0);
    BMTest_ENDFUNC(status);
    BMTest_END;
    return status ? EXIT_FAILURE : EXIT_SUCCESS; 
}