# BitBottle

BitBottle is a lightweight C library that provides bit-level file I/O utilities.

Standard file operations in C operate on bytes, but many applications require
working with individual bits or groups of bits. BitBottle provides an abstraction
over `FILE*` streams that allows applications to read and write arbitrary numbers
of bits while internally managing byte buffering.

## Features

- Read individual bits from files
- Write individual bits to files
- Read and write arbitrary-length bit sequences
- Automatic byte buffering
- Simple C API
- No external dependencies
- Suitable for compression, serialization, and binary file formats

## Requirements
- C compiler with C17 support
- C standard library

## Project Structure
```
BitBottle/
|-- include/
|   |-- BitBottle.h
|-- source/
|   |-- BitBottle.c
|   |-- ErrorHandling.h
|   |-- ErrorHandling.c
|-- CMakeLists.txt
```
## Building

BitBottle can be built as a static library using CMake.

Example:

```cmake
add_subdirectory(BitBottle)

target_link_libraries(MyApplication PRIVATE
    BitBottle
)
```

After linking the library, include the header:

```c
#include <BitBottle.h>
```

---

# Usage Example

## Writing Bits
```c
#include <BitBottle.h>

int main()
{
    BitFile* file = OpenOutputBitFile("data.bin");

    OutputBit(file, 1);
    OutputBit(file, 0);

    OutputBits(file, 0b10110, 5);

    CloseOutputBitFile(file);
    
    return 0;
}
```

This writes the following bit sequence:
```
1010110
```

The library automatically handles packing these bits into bytes

---

## Reading Bits
```c
#include <BitBottle.h>

int main()
{
    BitFile* file = OpenInputBitFile("data.bin");

    int bit;
    unsigned long value;

    bit = InputBit(file);
    value = InputBits(file, 5);

    CloseInputBitFile(file);

    return 0;
}
```

---

# API Reference

## BitFile
```c
typedef struct BitFile BitFile;
```

Represents a bit-level file stream.

The structure maintains the internal state required to read and write bits,
including the underlying file stream, current bit position, byte buffer, and
operation tracking information.

---

# Input Functions

## OpenInputBitFile
```
BitFile* OpenInputBitFile(char* filename);
```

Opens a file for bit-level input and initializes a `BitFile` structure.

### Parameters

| Parameter | Description |
|-----------|-------------|
| `filename` | Path to the file to open |

### Returns

A pointer to an initialized `BitFile` object.

Returns `NULL` if memory allocation fails.

---

## InputBit

```c
int InputBit(BitFile* bitFile);
```

Reads a single bit from the input stream.

### Returns

Returns either:

```
0
```

or

```
1
```

---

## InputBits

```c
unsigned long InputBits(BitFile* bitFile, int bitCount);
```

Reads multiple bits from the input stream.

### Parameters

| Parameter | Description |
|-----------|-------------|
| `bitFile` | BitFile stream |
| `bitCount` | Number of bits to read |

### Returns

The requested bits packed into an `unsigned long`.

---

## CloseInputBitFile

```c
void CloseInputBitFile(BitFile* bitFile);
```

Closes an input bit stream and releases all associated resources.

---

# Output Functions

## OpenOutputBitFile

```c
BitFile* OpenOutputBitFile(char* filename);
```

Opens a file for bit-level output and initializes a `BitFile` structure.

### Parameters

| Parameter | Description |
|-----------|-------------|
| `filename` | Path to the output file |

### Returns

A pointer to an initialized `BitFile` object.

Returns `NULL` if memory allocation fails.

---

## OutputBit

```c
void OutputBit(BitFile* bitFile, int bit);
```

Writes a single bit to the output stream.

### Parameters

| Parameter | Description |
|-----------|-------------|
| `bitFile` | Output bit stream |
| `bit` | Bit value to write (`0` or `1`) |

---

## OutputBits

```c
void OutputBits(BitFile* bitFile, unsigned long code, int count);
```

Writes multiple bits to the output stream.

### Parameters

| Parameter | Description |
|-----------|-------------|
| `bitFile` | Output bit stream |
| `code` | Value containing the bits to write |
| `count` | Number of bits to write |

Bits are written from the most significant bit toward the least significant
bit.

Example:

```c
OutputBits(file, 0b1011, 4);
```

Writes:

```
1011
```

---

## CloseOutputBitFile

```c
void CloseOutputBitFile(BitFile* bitFile);
```

Flushes any remaining buffered bits, closes the file, and releases the
associated resources.

---

# Utility Functions

## FilePrintBinary

```c
void FilePrintBinary(FILE* file, unsigned int code, int bits);
```

Writes the binary representation of a value as ASCII characters.

Example:

```c
FilePrintBinary(stdout, 13, 8);
```

Output:

```
00001101
```

---

# Bit Ordering

BitBottle processes bits in **most-significant-bit first order**.

For example:

```c
OutputBits(file, 0b1101, 4);
```

writes:

```
1 1 0 1
```

in that order.

This ordering is maintained when reading the data back using `InputBits()`.