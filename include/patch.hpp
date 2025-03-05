#ifndef PATCH_HPP
#define PATCH_HPP

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int READING_INPUT_SEGMENT = 0x5A;
const int BYPASS_SEGMENT	= 0xC0;

const uint8_t COMMAND_NOP	    = 0x90;
const uint8_t COMMAND_CMP_AX_1_BYTE = 0x3B;
const uint8_t COMMAND_CMP_AX_2_BYTE = 0xC0;

size_t getFileSize(FILE* file);
int patchInputFile(char* input_file_name);
int patchMemory(uint8_t* memory);

#endif
