#ifndef __OSOS__COMMON__LIB__LIBIO_H
#define __OSOS__COMMON__LIB__LIBIO_H

#include <common/types.h>
#include <common/lib/libasm.h>
#include <common/lib/libstr.h>

// Helpers for printing functions

void VerifyXY(); // If x and y over the desired limit, new line and/or scroll
int32_t VerifyPrintProgress(const char* functionInUse); // Better off an int than a bool


// Printing things

void putchar(uint8_t character); // wrapper for printf %s and %d and whatnot
void printf(char* str, ...); // the main screen output function.


// Cursor things

void EnableCursor(uint8_t start, uint8_t end);
void DisableCursor();
void UpdateCursor();


// Key functions

void FlushShell();

#endif