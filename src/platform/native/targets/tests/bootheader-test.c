#include <stdint.h>

#include <rumboot/boot.h>
#include <rumboot/printf.h>
#include <algo/crc32.h>
#include <rumboot/rumboot.h>

#define NUM_CORES 1


enum bootheader_err_code {
  BOOTHEADER_OK,
  BOOTHEADER_CHECK_HEADER_ERR,
  BOOTHEADER_CHECK_DATA_ERR,
  BOOTHEADER_IMG_EXEC_ERR,
};

enum bootheader_err_code test_rumboot_bootimage_check_header(struct rumboot_bootheader *hdr) {

  int numb  = 3;
  int i;
  struct rumboot_bootheader* wrong_hdr[numb] ;
  void *data;

  for(i = 0; i < numb; i++) {
      wrong_hdr[i] = rumboot_malloc(sizeof (*wrong_hdr[i]) + hdr->num_cores * sizeof (hdr->entry_point[0])); //allocation memory
      *wrong_hdr[i] = *hdr; //initialization
  }

  i = 0;
  wrong_hdr[i++]->magic        = 0xbebebeee;
  wrong_hdr[i++]->num_cores    = 0;
  wrong_hdr[i++]->header_crc32 = 0;
  enum bootheader_err_code errs[numb];

  for(i = 0; i < numb; i++) {

    if(rumboot_bootimage_check_header(wrong_hdr[i], &data) < 0)
      errs[i] = BOOTHEADER_CHECK_HEADER_ERR;
    else
      errs[i] = BOOTHEADER_OK;
  }

  for(i = 0; i < numb; i++) {

    if(errs[i] != BOOTHEADER_CHECK_HEADER_ERR)
      return BOOTHEADER_OK;
  }

  for(i = 0; i < numb; i++)
    rumboot_free(wrong_hdr[i]);


  return BOOTHEADER_CHECK_HEADER_ERR;
}

enum bootheader_err_code test_rumboot_bootimage_check_data(struct rumboot_bootheader *hdr) {

  int numb  = 2;
  int i;
  struct rumboot_bootheader* wrong_hdr[numb];

  for(i = 0; i < numb; i++) {
    wrong_hdr[i] = rumboot_malloc(sizeof (*wrong_hdr[i]) + hdr->num_cores * sizeof (hdr->entry_point[0])); //allocation memory
    *wrong_hdr[i] = *hdr; //initialization
  }

  i = 0;
  wrong_hdr[i++]->datalen      = 0;
  wrong_hdr[i++]->data_crc32   = 0;

  enum bootheader_err_code errs[numb];

  for(i = 0; i < numb; i++) {

    if(rumboot_bootimage_check_data(wrong_hdr[i]) < 0)
      errs[i] = BOOTHEADER_CHECK_DATA_ERR;
    else
      errs[i] = BOOTHEADER_OK;
  }

  for(i = 0; i < numb; i++) {

    if(errs[i] != BOOTHEADER_CHECK_DATA_ERR)
      return BOOTHEADER_OK;
  }

  for(i = 0; i < numb; i++)
    rumboot_free(wrong_hdr[i]);

  return BOOTHEADER_CHECK_DATA_ERR;
}

enum bootheader_err_code test_rumboot_bootimage_exec(struct rumboot_bootheader *hdr) {



  return BOOTHEADER_IMG_EXEC_ERR;
}


int main() {

  rumboot_printf("Test bootheader.\n");

  struct rumboot_bootheader* hdr;
  hdr = rumboot_malloc(sizeof (*hdr) + NUM_CORES * sizeof (hdr->entry_point[0]));
  hdr->magic = RUMBOOT_HEADER_MAGIC;
  hdr->version = RUMBOOT_HEADER_VERSION;
  hdr->num_cores = NUM_CORES;

  if(test_rumboot_bootimage_check_header(hdr) != BOOTHEADER_CHECK_HEADER_ERR)
      goto free_memory;

  rumboot_printf("Test of the checking header pass\n");

  if(test_rumboot_bootimage_check_data(hdr) != BOOTHEADER_CHECK_DATA_ERR)
    goto free_memory;

  rumboot_printf("Test of the checking data pass\n");

  return 0;

  free_memory:
    rumboot_free(hdr);
    return -1;
}
