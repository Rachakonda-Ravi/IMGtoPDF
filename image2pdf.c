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

    float width;

    float height;

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
/*=========================================================
                    FUNCTION PROTOTYPES
=========================================================*/

/*-------------------- General Utilities --------------------*/
void print_header(void);
void print_mode_info(ProgramMode mode);

int yes_no_prompt(const char *message);
void pause_program(void);

/*---------------------- File Utilities ---------------------*/
void create_logs_folder(void);

int file_exists(const char *filename);

long long get_file_size(const char *filename);

void format_size(long long bytes, char *buffer);

void get_datetime(char *date, char *time);

void get_log_filename(const char *pdfname,
                      char *log_file,
                      char *error_file);

/*--------------------- Image Handling ----------------------*/
int is_supported_image(const char *filename);

int scan_images(ImageInfo images[],
                Statistics *stats);

void sort_images(ImageInfo images[],
                 int count,
                 SortMode mode);

void print_image_list(ImageInfo images[],
                      int count,
                      Statistics *stats);

/*--------------------- User Interface ----------------------*/
void get_output_filename(ProgramOptions *options);

void get_custom_options(ProgramOptions *options);

int confirm_proceed(void);

/*--------------------- PDF Functions -----------------------*/
int create_pdf(ImageInfo images[],
               int count,
               ProgramOptions *options,
               Statistics *stats,
               ErrorEntry errors[],
               int *error_count);

/*---------------------- Log Functions ----------------------*/
void write_log(const ProgramOptions *options,
               const Statistics *stats,
               const char *log_file);

void write_error_log(const ErrorEntry errors[],
                     int error_count,
                     const char *error_file);

/*---------------------- Report Functions -------------------*/
void print_summary(const ProgramOptions *options,
                   const Statistics *stats,
                   const char *log_file);

/*---------------------- Cleanup ----------------------------*/
void cleanup(ImageInfo images[],
             int count);

/*---------------------- Sorting ----------------------------*/
int cmp_name(const void *a, const void *b);

int cmp_reverse_name(const void *a, const void *b);

int cmp_size_asc(const void *a, const void *b);

int cmp_size_desc(const void *a, const void *b);

/*=========================================================
                    GENERAL UTILITIES
=========================================================*/

/*---------------------------------------------------------
    Print Program Header
---------------------------------------------------------*/
void print_header(void)
{
    printf("\n");
    printf("============================================================\n");
    printf("                %s\n", PROGRAM_NAME);
    printf("                    Version %s\n", PROGRAM_VERSION);
    printf("============================================================\n\n");
}

/*---------------------------------------------------------
    Display Program Mode Information
---------------------------------------------------------*/
void print_mode_info(ProgramMode mode)
{
    if (mode == CUSTOM_MODE)
    {
        printf("Mode : Custom\n\n");
        printf("Advanced options enabled.\n\n");
    }
    else
    {
        printf("Mode : Default\n\n");

        printf("For advanced options, run:\n\n");
        printf("    image2pdf Custom\n\n");
    }
}

/*---------------------------------------------------------
    Yes / No Prompt
---------------------------------------------------------*/
int yes_no_prompt(const char *message)
{
    char choice;

    while (1)
    {
        printf("%s (Y/N): ", message);

        scanf(" %c", &choice);

        if (choice == 'Y' || choice == 'y')
            return 1;

        if (choice == 'N' || choice == 'n')
            return 0;

        printf("Invalid choice. Please enter Y or N.\n\n");
    }
}

/*---------------------------------------------------------
    Pause Program
---------------------------------------------------------*/
void pause_program(void)
{
#ifdef _WIN32
    system("pause");
#else
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
#endif
}