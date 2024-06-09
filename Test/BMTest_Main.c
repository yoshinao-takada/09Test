#define BMTEST_MAIN_C
#include "BMDefs.h"
#include "BMTest.h"
#include <stdlib.h>

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

    } while (0);
    BMTest_ENDFUNC(status);
    BMTest_END;
    return status ? EXIT_FAILURE : EXIT_SUCCESS; 
}