#include "patch.hpp"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>


int patchMemory(uint8_t* file_data)
{
	assert(file_data != NULL);

	int code_shift = READING_INPUT_SEGMENT;
	file_data[code_shift] 	  = COMMAND_NOP;
	file_data[code_shift + 1] = COMMAND_NOP;

	code_shift = BYPASS_SEGMENT;

	file_data[code_shift]     = COMMAND_NOP;
	file_data[code_shift + 1] = COMMAND_CMP_AX_1_BYTE;
	file_data[code_shift + 2] = COMMAND_CMP_AX_2_BYTE;

	return EXIT_SUCCESS;
}


int patchInputFile(char* input_file_name)
{
	assert(input_file_name != NULL);

	FILE* input_file = fopen(input_file_name, "rb");
	if (!input_file)
	{
		fprintf(stderr, "Error reading input file\n");
		return EXIT_FAILURE;
	}

	size_t size_of_file = getFileSize(input_file);

	uint8_t* file_data = (uint8_t*)calloc(size_of_file, sizeof(uint8_t));
	if (file_data == NULL)
	{
		fprintf(stderr, "Failed to calloc memory to variable\n");

		fclose(input_file);
		input_file = NULL;

		return EXIT_FAILURE;
	}

	const size_t bytes_read = fread(file_data,
					sizeof(file_data[0]),
					size_of_file,
					input_file);
	if (bytes_read != size_of_file)
	{
		fprintf(stderr, "Error reading file\n");

		fclose(input_file);
		input_file = NULL;

		free(file_data);
		file_data = NULL;

		return EXIT_FAILURE;
	}

	fclose(input_file);
	input_file = NULL;

	patchMemory(file_data);

	char output_file_name[60] = {};
	sprintf(output_file_name, "CRACKED_%s", input_file_name);

	FILE* output_file = fopen(output_file_name, "wb");
	if (!output_file)
	{
		fprintf(stderr, "Error reading input file\n");

		free(file_data);
		file_data = NULL;

		return EXIT_FAILURE;
	}

	size_t bytes_wrote = fwrite(file_data, 
				    sizeof(file_data[0]),
				    size_of_file,
				    output_file);
	if (bytes_wrote != size_of_file)
	{
		fprintf(stderr, "Error writing file\n");

		fclose(output_file);
		output_file = NULL;

		free(file_data);
		file_data = NULL;

		return EXIT_FAILURE;
	}

	fclose(output_file);
	output_file = NULL;

	free(file_data);
	file_data = NULL;

	printf("Hacked!!!\n");

	return EXIT_SUCCESS;
}


size_t getFileSize(FILE* file) 
{
    assert(file != NULL);

    fseek(file, 0, SEEK_END);
    long int size_of_file = ftell(file);
    rewind(file);

    return (size_t)size_of_file;
}
