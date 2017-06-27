#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>

#define __swap32(value)                                 \
        ((((uint32_t)((value) & 0x000000FF)) << 24) |   \
         (((uint32_t)((value) & 0x0000FF00)) << 8) |    \
         (((uint32_t)((value) & 0x00FF0000)) >> 8) |    \
         (((uint32_t)((value) & 0xFF000000)) >> 24))


struct mem_layout {
	int	line_count;
	int	line_length;
	int	adjacement_banks;
	int	inverse_order;
	int	(*write_line)(struct mem_layout *layout, FILE *ifd, FILE *ofd);
	char *	(*gen_filename)(struct mem_layout *layout, const char *dir, int nfile);
};



void dump_byte(FILE *fd, char byte)
{
	int i;

	for (i = 0; i < 8; i++) {
		if (byte & (1 << i))
			fputc('1', fd);
		else
			fputc('0', fd);
	}
}

void dump_byte_inv(FILE *fd, char byte)
{
	int i;

	for (i = 7; i >= 0; i--) {
		if (byte & (1 << i))
			fputc('1', fd);
		else
			fputc('0', fd);
	}
}

char get_byte_parity(char byte)
{
	int i;
	unsigned char check = 0;

	for (i = 0; i < 8; i++) {
		if (byte & (1 << i))
			check += 1;
	}

	return (check & 0x1);
}

int write_line_mm7705(struct mem_layout *layout, FILE *ifd, FILE *ofd)
{
	int num_bytes = layout->line_length / 9; /* 8 bit data + 1 bit parity*/
	char *srcbuf = alloca(num_bytes);
	int i;

	i = fread(srcbuf, 1, 8, ifd);

	if (num_bytes != i) {
			fprintf(stderr, "Expected to read %d, only got %d\n", num_bytes, i);
			exit(1);
	}


	unsigned char parbyte = 0;

//	for (i = 0; i < num_bytes; i++) {
	for (i = 7; i >= 0; i--) {
		char tmp = srcbuf[i];
		parbyte |= (get_byte_parity(tmp) << i);
	}

	for (i = 0; i < num_bytes; i++)
		dump_byte_inv(ofd, srcbuf[i]);

	dump_byte(ofd, parbyte);

//	fputc('\n', ofd);

	return num_bytes;
}

int write_line_basis(struct mem_layout *layout, FILE *ifd, FILE *ofd)
{
	int num_bytes = layout->line_length / 8;
	unsigned char *srcbuf = alloca(num_bytes);
	int i;

	i = fread(srcbuf, 1, 8, ifd);

	if (num_bytes != i) {
			fprintf(stderr, "Expected to read %d, only got %d\n", num_bytes, i);
			exit(1);
	}

	for (i = num_bytes-1; i >=0 ; i--)
		fprintf(ofd, "%02X", srcbuf[i]);

//	fputc('\n', ofd);

	return num_bytes;
}

char *gen_mm7705_filename(struct mem_layout *layout, const char *dir, int nfile)
{
	char *ret = NULL;

	asprintf(&ret, "%s/rom_hdd_4096_72_mux8_%d_verilog.rcf", dir, nfile);
	return ret;
}

char *gen_basis_filename(struct mem_layout *layout, const char *dir, int nfile)
{
	char *ret = NULL;

	asprintf(&ret, "%s/image_mem64_%d.hex", dir, nfile);
	return ret;
}

struct mem_layout mm7705_rom = {
	.line_count		= 4096,
	.line_length		= 72,
	.adjacement_banks	= 2,
	.inverse_order		= 1,
	.gen_filename		= gen_mm7705_filename,
	.write_line         = write_line_mm7705,
};

struct mem_layout basis_rom_generic = {
	.line_count		    = 131072,
	.line_length		= 32,
	.adjacement_banks	= 2,
	.inverse_order		= 0,
	.gen_filename		= gen_basis_filename,
	.write_line         = write_line_basis,
};

struct mem_layout basis_rom = {
	.line_count		    = 262144,
	.line_length		= 64,
	.adjacement_banks	= 1,
	.inverse_order		= 0,
	.gen_filename		= gen_basis_filename,
	.write_line         = write_line_basis,
};


#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))

int dump_rcf(const char *inputfile, const char *outdir, struct mem_layout *layout)
{
	FILE *ifd = fopen(inputfile, "rb");

	if (!ifd) {
		fprintf(stderr, "Error opening input file\n");
		return -1;
	}

	size_t file_size;
	fseek(ifd, 0L, SEEK_END);
	file_size = ftell(ifd);
	rewind(ifd);

	/* We take extra parity bit into account here */
	int rcf_file_count = DIV_ROUND_UP(file_size + (file_size / 8),
					  (layout->line_count * layout->line_length / 8));

	if (rcf_file_count < layout->adjacement_banks)
		rcf_file_count = layout->adjacement_banks;

	printf("Input file %ld bytes, %d total rcf files\n", file_size, rcf_file_count);

	FILE **ofd = alloca(sizeof(FILE *) * rcf_file_count);
	int i;
	char *filename;

	for (i = 0; i < rcf_file_count; i++) {
		filename = layout->gen_filename(layout, outdir, i);
		if (!filename) {
			fprintf(stderr, "No memory\n");
			exit(1);
		}

		printf("Creating rcf %d/%d: %s\n", i, rcf_file_count, filename);
		ofd[i] = fopen(filename, "w+");
		if (!ofd[i]) {
			fprintf(stderr, "Error opening output file\n");
			exit(1);
		}
		free(filename);
	}

	/* We take extra parity bit into account here */
	int bytes_per_rcf_line = layout->line_length / 8;
	int per_iteration = layout->adjacement_banks * bytes_per_rcf_line;

	printf("Writing rcf files: %d bytes per iteration, %d bytes per rcf line\n", per_iteration, bytes_per_rcf_line);
	int n = 0;
	int fd_offset = 0;
	int *output_started = calloc(rcf_file_count, sizeof(int));

	while (n < file_size) {
		int k;
		for (k = 0; k < layout->adjacement_banks; k++) {
			int dsc = k + fd_offset;
			if (layout->inverse_order)
				dsc = fd_offset + layout->adjacement_banks - k - 1;

			if (output_started[dsc]) /* \n only for first line */
				fputc('\n', ofd[dsc]);
			n += layout->write_line(layout, ifd, ofd[dsc]);
			output_started[dsc] = 1;
		}

		int next = (fd_offset + layout->adjacement_banks) * (bytes_per_rcf_line * layout->line_count);

		if (n >= next) {
			fd_offset += layout->adjacement_banks;
			printf("Selecting next descriptor batch @ offset %d, next %d\n", fd_offset, next);
		}
	}
	free(output_started);
	printf("Total %d bytes processed\n", n);
	for (i = 0; i < rcf_file_count; i++)
		fclose(ofd[i]);
	return 0;
}

static struct option long_options[] = {
	{ "input",  required_argument, 0, 'i' },
	{ "outdir", required_argument, 0, 'o' },
	{ "help",   no_argument,       0, 0   },
	{ 0,	    0,		       0, 0   }
};

void usage(const char *argv0)
{
	printf("BIN2RCF tool. (c) Andrew 'Necromant' Andrianov, RC Module 2016\n");
	printf("Usage: %s --input file.bin --outdir=./rcfs/\n", argv0);
	printf("This is free, WTFPL-licensed software\n");
}

const char *input_file = NULL;
const char *outdir = ".";

int main(int argc, char **argv)
{
	if (argc == 1) {
		usage(argv[0]);
		exit(1);
	}

	while (1) {
		int option_index = 0;
		int c = getopt_long(argc, argv, "i:o:h",
				    long_options, &option_index);
		if (c == -1)
			break;
		switch (c) {
		case 'i':
			input_file = optarg;
			break;
		case 'o':
			outdir = optarg;
			break;
		case 'h':
			usage(argv[0]);
			exit(1);
			break;
		}
	}

	if (!input_file) {
		printf("Missing input file\n");
		usage(argv[0]);
		exit(1);
	}

	dump_rcf(input_file, outdir, &basis_rom);
}
