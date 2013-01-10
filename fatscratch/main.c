#include <stdio.h>

#include "fat_access.h"
#include "fat_filelib.h"


//==================
// local vars

//static char imgFileName[64];
FILE* imgFile;
// current working directory
FL_DIR cwd;


/* ///// test packing behavior
struct dir_packed {
  uint8 Name[11];
  uint8 Attr;
  uint8 NTRes;
  uint8 CrtTimeTenth;
  uint8 CrtTime[2];
  uint8 CrtDate[2];
  uint8 LstAccDate[2];
  uint16 FstClusHI;
  uint8 WrtTime[2];
  uint8 WrtDate[2];
  uint16 FstClusLO;
  uint32 FileSize;
}  __attribute__((__packed__));

struct dir_not {
  uint8 Name[11];
  uint8 Attr;
  uint8 NTRes;
  uint8 CrtTimeTenth;
  uint8 CrtTime[2];
  uint8 CrtDate[2];
  uint8 LstAccDate[2];
  uint16 FstClusHI;
  uint8 WrtTime[2];
  uint8 WrtDate[2];
  uint16 FstClusLO;
  uint32 FileSize; 
};

struct dir_packed dir;
int adx, ady;
int dif;
uint32 barf;
*/


//=============================
/// local funcs

static int test_files(void);

static int media_init(const char* imgFileName) {
  imgFile = fopen(imgFileName, "r");  
  if (imgFile == 0) {
    printf("\r\n error opening image file.");
  }
  //  printf("\r\n image file pointer: %16x\r\n", (unsigned long int)imgFile);
  return 1;
}

static int media_read(unsigned long sector, unsigned char *buffer, unsigned long sector_count) {
  unsigned long i;

  for (i=0;i<sector_count;i++) {
      // ...
      // Add platform specific sector (512 bytes) read code here
      //..
    fseek(imgFile, sector * 512, 0);
    fread(buffer, 1, 512, imgFile);
    sector ++;
    buffer += 512;
  }

  return 1;
}

int media_write(unsigned long sector, unsigned char *buffer, unsigned long sector_count) {
  unsigned long i;

  for (i=0;i<sector_count;i++) {
      // ...
      // Add platform specific sector (512 bytes) write code here
      //..
    fseek(imgFile, sector * 512, 0);
    fwrite(buffer, 1, 512, imgFile);
    sector ++;
    buffer += 512;
  }

  return 1;
}

int main(int argc, char** argv) {
  FL_FILE *file;
  
  if(argc < 2) {
    printf("\r\n error: expected filename argument.");
    return 1;
  }

  /// check packing:
  //   printf("\r\n packed size: %d \r\n", (int)sizeof(struct dir_packed));
  //   printf("\r\n unpacked size: %d \r\n", (int)sizeof(struct dir_not));

   //// something is fucked with the packing behavior.
   //// print address of each member in struct dir_entry... 
   /*
   adx = &(dir);
   printf("\r\n dir start: %08x", adx);
   ady = &(dir.Name); dif = ady - adx;
   printf("\r\n mem addr: %08x, size: %d", ady, dif); adx = ady;
   ady = &(dir.Attr); dif = ady - adx;
   printf("\r\n mem addr: %08x, size: %d", ady, dif); adx = ady;
   ady = &(dir.NTRes); dif = ady - adx;
   printf("\r\n mem addr: %08x, size: %d", ady, dif); adx = ady;
   ady = &(dir.CrtTimeTenth); dif = ady - adx;
   printf("\r\n mem addr: %08x, size: %d", ady, dif); adx = ady;
   ady = &(dir.CrtTime); dif = ady - adx;
   printf("\r\n mem addr: %08x, size: %d", ady, dif); adx = ady;
   ady = &(dir.CrtDate); dif = ady - adx;
   printf("\r\n mem addr: %08x, size: %d", ady, dif); adx = ady;
   ady = &(dir.LstAccDate); dif = ady - adx;
   printf("\r\n mem addr: %08x, size: %d", ady, dif); adx = ady;
   ady = &(dir.FstClusHI); dif = ady - adx;
   printf("\r\n mem addr: %08x, size: %d", ady, dif); adx = ady;
   ady = &(dir.WrtTime); dif = ady - adx;
   printf("\r\n mem addr: %08x, size: %d", ady, dif); adx = ady;
   ady = &(dir.WrtDate); dif = ady - adx;
   printf("\r\n mem addr: %08x, size: %d", ady, dif); adx = ady;
   ady = &(dir.FstClusLO); dif = ady - adx;
   printf("\r\n mem addr: %08x, size: %d", ady, dif); adx = ady;
   ady = &(dir.FileSize); dif = ady - adx;
   printf("\r\n mem addr: %08x, size: %d", ady, dif); adx = ady;

   printf("\r\n last member size: %d", sizeof(dir.FileSize) );
   printf("\r\n uhhh.. size of uint32?? :  %d\r\n", sizeof(uint32));
   printf("\r\n uhhh.. size of uint16?? :  %d\r\n", sizeof(uint16));
   printf("\r\n uhhh.. size of uint8?? :  %d\r\n", sizeof(uint8));
   */

  // Initialise media
  media_init(argv[1]);

  // Initialise File IO Library
  fl_init();

  // Attach media access functions to library
  if (fl_attach_media((fn_diskio_read)media_read, (fn_diskio_write)media_write) != FAT_INIT_OK)
    {
      printf("ERROR: Media attach failed\n");
      return 0; 
    }

  // List root directory
  fl_listdirectory("/");

  // Create File
  file = fl_fopen("/file.bin", "w");
  if (file)
    {
      // Write some data
      unsigned char data[] = { 1, 2, 3, 4 };
      if (fl_fwrite(data, 1, sizeof(data), file) != sizeof(data))
	printf("ERROR: Write file failed\n");
    }
  else
    printf("ERROR: Create file failed\n");

  // Close file
  fl_fclose(file);

  // Delete File
  if (fl_remove("/file.bin") < 0)
    printf("ERROR: Delete file failed\n");

  // List root directory
  fl_listdirectory("/");

  test_files();
  
  fl_shutdown();

  return 0;
}


//// test function
  /// traverse directory tree and print contents of each file
static int test_files(void) {
  FL_DIR dirstat;
  struct fs_dir_ent dirent;
  
  void* fp;
  unsigned long int i;
  int data;
  
  char path[64];
  
  if( fl_opendir("/one", &dirstat) ) {
    while (fl_readdir(&dirstat, &dirent) == 0) {
      // FIXME
      //      fp = fl_fopen(dirent.filename, "r");
      strcpy(path, "/one/");
      strncat(path, dirent.filename, 58);
      fp = fl_fopen(path, "r");
      
      printf( "file path: %s ; ", path );
      printf( "file pointer: %08x ; ", (long unsigned int)fp );
      printf( "name: %s ; cluster: 0x%08x \r\n", dirent.filename, (long unsigned int)dirent.cluster );
      printf("\r\n");
      for(i=0; i < dirent.size; i++) {
	//	fl_fgetc(fp);
	fl_fread(&data, 1, 1, fp);
	printf("%04x (%c) \r\n", data, (char)data);	
      }
      printf("\r\n");

    }
    return 0;
  }
  return 1;
  
}
