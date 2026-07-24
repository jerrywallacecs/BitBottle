#pragma once

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
    Represents a bit-level file stream. BitFile wraps a standard FILE pointer
    and maintains the internal state required to read and write individual bits.
    It manages byte buffering, bit positioning, and stream progress tracking for
    bit-oriented I/O.
*/
typedef struct BitFile
{
    FILE* file; // underlying file stream used for reading or writing data
    unsigned char mask; // bit mask identifying the current bit position within the active byte. used to set output bits or extract input bits. 
    int rack; // buffered byte currently being read from or written to the file. stores accumulated output bits or the current input byte.
    int pacifierCounter; // counter used to periodically report progress during large I/O operations.
} BitFile;

/*
    Opens a file for bit-level input and initializes its associated BitFile
    structure. Allocates and prepares a BitFile object for reading individual 
    bits from the specified file.
*/
BitFile* OpenInputBitFile(char* filename);

/*
    Reads a single bit from the input bitstream. Retrieves the next
    bit from the underlying file, automatically loading a new byte 
    into the internal buffer as needed. A progress indicator is
    periodically printed during long read operations.
*/
int InputBit(BitFile* bitFile);

/*
    Reads a sequence of bits from the input bitstream. Retrieves the 
    specified number of bits and returns them as an unsigned integer,
    automatically refilling the internal buffer as needed. A progress
    indicator is periodically printed during long read operations.
*/
unsigned long InputBits(BitFile* bitFile, int bitCount);

/*
    Closes a bit-level input stream and releases its associated resources.
    Closes the underlying file and frees the BitFile structure.
*/
void CloseInputBitFile(BitFile* bitFile);

/*
    Opens a file for bit-level output and initializes its associated
    BitFile structure. Allocates and prepares a BitFile object for writing
    individual bits to the specified file.
*/
BitFile* OpenOutputBitFile(char* filename);

/*
    Writes a single bit to the output bitstream. Buffers
    the bit until a full byte has been assembled, then writes
    to the underlying file. A progress indicator is periodically
    printed during long write operations.
*/
void OutputBit(BitFile* bitFile, int bit);

/*
    Writes a sequence of bits to the output bitstream. Outputs
    the specified number of bits from the given code, buffering
    them until complete bytes can be written to the underlying file. 
    A progress indicator is periodically printed during long write 
    operations.
*/
void OutputBits(BitFile* bitFile, unsigned long code, int count);


/*
    Flushes any remaining buffered bits, closes the output file, and
    the associated BitFile resources. Ensures any partially filled output
    byte is written before the file is closed
*/
void CloseOutputBitFile(BitFile* bitFile);

/*
    Writes the binary representation of a value to a text stream.
    Outputs the specified number of bits as ASCII '0' and '1' characters
    without appending a newline.
*/
void FilePrintBinary(FILE* file, unsigned int code, int bits);

#ifdef __cplusplus
}
#endif