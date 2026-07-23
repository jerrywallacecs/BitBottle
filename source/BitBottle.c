#include <stdio.h>
#include <stdlib.h>

#include "BitBottle.h"
#include "ErrorHandling.h"

/*
    INPUT
*/

BitFile* OpenInputBitFile(char* name)
{
    BitFile* bitFile;
    bitFile = (BitFile*)calloc(1, sizeof(BitFile));

    if (bitFile == NULL)
    {
        return bitFile;
    }

    bitFile->file = fopen(name, "rb");
    bitFile->mask = 0x80;
    bitFile->rack = 0;
    bitFile->pacifierCounter = 0;

    return bitFile;
}

int InputBit(BitFile* bitFile)
{
    int value;

    if (bitFile->mask == 0x80)
    {
        bitFile->rack = getc(bitFile->file);

        if (bitFile->rack == EOF)
        {
            FatalError("Fatal error in InputBit!\n");
        }
        if ((bitFile->pacifierCounter++ & 2047) == 0)
        {
            putc('.', stdout);
        }
    }

    value = bitFile->rack & bitFile->mask;
    bitFile->mask >>= 1;

    if (bitFile->mask == 0)
    {
        bitFile->mask = 0x80;
    }

    return (value ? 1 : 0);
}

unsigned long InputBits(BitFile* bitFile, int bitCount)
{
    unsigned long mask = 1L << (bitCount - 1);
    unsigned long returnValue = 0;

    while (mask != 0)
    {
        if (bitFile->mask == 0x80)
        {
            bitFile->rack = getc(bitFile->file);

            if (bitFile->rack == EOF)
            {
                FatalError("Fatal error in InputBits!\n");
            }
            if ((bitFile->pacifierCounter++ & 2047) == 0)
            {
                putc('.', stdout);
            }
        }

        if (bitFile->rack & bitFile->mask)
        {
            returnValue |= mask;
        }

        mask >>= 1;
        bitFile->mask >>= 1;

        if (bitFile->mask == 00)
        {
            bitFile->mask = 0x80;
        }
    }

    return returnValue;
}

void CloseInputBitFile(BitFile* bitFile)
{
    fclose(bitFile->file);
    free((char*)bitFile);
}

/*
    OUTPUT
*/

BitFile* OpenOutputBitFile(char* filename)
{
    BitFile* bitFile = (BitFile*)calloc(1, sizeof(BitFile));

    if (bitFile == NULL)
    {
        return bitFile;
    }

    bitFile->file = fopen(filename, "wb");
    bitFile->mask = 0x80;
    bitFile->rack = 0;
    bitFile->pacifierCounter = 0;

    return bitFile;
}

void OutputBit(BitFile* bitFile, int bit)
{
    if (bit)
    {
        bitFile->rack |= bitFile->mask;
    }

    bitFile->mask >>= 1;

    if (bitFile->mask == 0)
    {
        if (putc(bitFile->rack, bitFile->file) != bitFile->rack)
        {
            FatalError("Fatal error in OutputBit!\n");
        }
        else if ((bitFile->pacifierCounter++ & 4095) == 0)
        {
            putc('.', stdout);
        }

        bitFile->rack = 0;
        bitFile->mask = 0x80;
    }
}

void OutputBits(BitFile* bitFile, unsigned long code, int count)
{
    unsigned long mask = 1L << (count - 1);

    while (mask != 0)
    {
        if (mask & code)
        {
            bitFile->rack |= bitFile->mask;
        }

        bitFile->mask >>= 1;

        if (bitFile->mask == 0)
        {
            if (putc(bitFile->rack, bitFile->file) != bitFile->rack)
            {
                FatalError("Fatal error in OutputBits!\n");
            }
            else if ((bitFile->pacifierCounter++ & 2047) == 0)
            {
                putc('.', stdout);
            }

            bitFile->rack = 0;
            bitFile->mask = 0x80;
        }

        mask >>= 1;
    }
}

void CloseOutputBitFile(BitFile* bitFile)
{
    if (bitFile->mask != 0x80)
    {
        if (putc(bitFile->rack, bitFile->file) != bitFile->rack)
        {
            FatalError("Fatal error in CloseOutputBitFile!\n");
        }

        fclose(bitFile->file);
        free((char*)bitFile);
    }
}

void FilePrintBinary(FILE* file, unsigned int code, int bits)
{
    unsigned int mask = 1L << (bits - 1);

    while (mask != 0)
    {
        if (code & mask)
        {
            fputc('1', file);
        }
        else
        {
            fputc('0', file);
        }

        mask >>= 1;
    }
}