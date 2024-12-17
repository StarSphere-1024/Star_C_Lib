#ifndef __QUICK_IO_H__
#define __QUICK_IO_H__

#include <stdint.h>
#include <stdbool.h>

bool QIOGetInt(int *x);

void QIOGetDouble(double *x);

void QIOPutInt(int x);

void QIOPutDouble(int precision, double x);

#endif


