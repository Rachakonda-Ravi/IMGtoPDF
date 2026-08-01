#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define mkdir(dir, mode) _mkdir(dir)
#else
#include <unistd.h>
#endif

#include <hpdf.h>

#define MAX_IMAGES         1000
#define MAX_FILENAME       260
#define MAX_TITLE          100
#define MAX_AUTHOR         100
#define MAX_SUBJECT        200
#define MAX_KEYWORDS       200
#define PAGE_WIDTH         595.0
#define PAGE_HEIGHT        842.0
#define LOG_FOLDER         "Logs"
#define PROGRAM_NAME       "Image to PDF Converter"
#define PROGRAM_VERSION    "2.0"

typedef enum
{
    JPEG,
    PNG
} ImageType;

typedef enum
{
    SORT_NAME_ASC = 1,
    SORT_NAME_DESC,
    SORT_SIZE_ASC,
    SORT_SIZE_DESC
} SortMode;

typedef enum
{
    FIT = 1,
    FILL
} PageMode;

typedef enum
{
    DEFAULT_MODE,
    CUSTOM_MODE
} ProgramMode;

typedef struct
{
    char filename[MAX_FILENAME];

    ImageType type;

    long long filesize;

    unsigned width;

    unsigned height;

    int loaded;

} ImageInfo;

typedef struct
{
    char title[MAX_TITLE];

    char author[MAX_AUTHOR];

    char subject[MAX_SUBJECT];

    char keywords[MAX_KEYWORDS];

} PDFMetadata;

typedef struct
{
    ProgramMode mode;

    SortMode sort;

    PageMode page_mode;

    int open_pdf;

    char pdf_name[MAX_FILENAME];

    PDFMetadata metadata;

} ProgramOptions;

typedef struct
{
    int total_images;

    int jpg;

    int png;

    int processed;

    int skipped;

    long long total_size;

    double time_taken;

} Statistics;

typedef struct
{
    char filename[MAX_FILENAME];

    char reason[100];

} ErrorEntry;

