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

    int page_number;

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

    int overwrite_existing;

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

    int failed_loads;
    
    int duplicate_names;

} Statistics;

typedef struct
{
    char filename[MAX_FILENAME];

    char reason[256];

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

/*=========================================================
                    FILE UTILITIES
=========================================================*/

/*---------------------------------------------------------
    Create Logs Folder
---------------------------------------------------------*/
void create_logs_folder(void)
{
    struct stat st = {0};

    if (stat(LOG_FOLDER, &st) == -1)
    {
        if (mkdir(LOG_FOLDER, 0755) == 0)
            printf("Created log folder : %s\n\n", LOG_FOLDER);
        else
            printf("Warning : Unable to create log folder.\n\n");
    }
}

/*---------------------------------------------------------
    Check Whether a File Exists
---------------------------------------------------------*/
int file_exists(const char *filename)
{
    struct stat st;

    return (stat(filename, &st) == 0);
}

/*---------------------------------------------------------
    Get File Size
---------------------------------------------------------*/
long long get_file_size(const char *filename)
{
    struct stat st;

    if (stat(filename, &st) != 0)
        return 0;

    return (long long)st.st_size;
}

/*---------------------------------------------------------
    Convert Bytes into Readable Format
---------------------------------------------------------*/
void format_size(long long bytes, char *buffer)
{
    const char *units[] =
    {
        "B",
        "KB",
        "MB",
        "GB",
        "TB"
    };

    double size = (double)bytes;
    int unit = 0;

    while (size >= 1024.0 && unit < 4)
    {
        size /= 1024.0;
        unit++;
    }

    sprintf(buffer, "%.2f %s", size, units[unit]);
}

/*---------------------------------------------------------
    Get Current Date and Time
---------------------------------------------------------*/
void get_datetime(char *date, char *time_str)
{
    time_t now = time(NULL);

    struct tm *tm_info = localtime(&now);

    strftime(date,
             20,
             "%Y-%m-%d",
             tm_info);

    strftime(time_str,
             20,
             "%H-%M-%S",
             tm_info);
}

/*---------------------------------------------------------
    Generate Log File Names
---------------------------------------------------------*/
void get_log_filename(const char *pdfname,
                      char *log_file,
                      char *error_file)
{
    char date[20];
    char time_str[20];

    get_datetime(date, time_str);

    sprintf(log_file,
            "%s/%s_%s_%s.log",
            LOG_FOLDER,
            pdfname,
            date,
            time_str);

    sprintf(error_file,
            "%s/%s_%s_%s_error.log",
            LOG_FOLDER,
            pdfname,
            date,
            time_str);
}

/*=========================================================
                    SORTING UTILITIES
=========================================================*/

/*---------------------------------------------------------
    Compare Image Names (Ascending)
---------------------------------------------------------*/
int cmp_name(const void *a, const void *b)
{
    const ImageInfo *img1 = (const ImageInfo *)a;
    const ImageInfo *img2 = (const ImageInfo *)b;

    return strcasecmp(img1->filename, img2->filename);
}

/*---------------------------------------------------------
    Compare Image Names (Descending)
---------------------------------------------------------*/
int cmp_reverse_name(const void *a, const void *b)
{
    return cmp_name(b, a);
}

/*---------------------------------------------------------
    Compare File Size (Ascending)
---------------------------------------------------------*/
int cmp_size_asc(const void *a, const void *b)
{
    const ImageInfo *img1 = (const ImageInfo *)a;
    const ImageInfo *img2 = (const ImageInfo *)b;

    if (img1->filesize < img2->filesize)
        return -1;

    if (img1->filesize > img2->filesize)
        return 1;

    return strcasecmp(img1->filename, img2->filename);
}

/*---------------------------------------------------------
    Compare File Size (Descending)
---------------------------------------------------------*/
int cmp_size_desc(const void *a, const void *b)
{
    const ImageInfo *img1 = (const ImageInfo *)a;
    const ImageInfo *img2 = (const ImageInfo *)b;

    if (img1->filesize > img2->filesize)
        return -1;

    if (img1->filesize < img2->filesize)
        return 1;

    return strcasecmp(img1->filename, img2->filename);
}

/*---------------------------------------------------------
    Sort Images
---------------------------------------------------------*/
void sort_images(ImageInfo images[],
                 int count,
                 SortMode mode)
{
    switch (mode)
    {
        case SORT_NAME_ASC:
            qsort(images,
                  count,
                  sizeof(ImageInfo),
                  cmp_name);
            break;

        case SORT_NAME_DESC:
            qsort(images,
                  count,
                  sizeof(ImageInfo),
                  cmp_reverse_name);
            break;

        case SORT_SIZE_ASC:
            qsort(images,
                  count,
                  sizeof(ImageInfo),
                  cmp_size_asc);
            break;

        case SORT_SIZE_DESC:
            qsort(images,
                  count,
                  sizeof(ImageInfo),
                  cmp_size_desc);
            break;

        default:
            qsort(images,
                  count,
                  sizeof(ImageInfo),
                  cmp_name);
    }
}

/*=========================================================
                    IMAGE HANDLING
=========================================================*/

/*---------------------------------------------------------
    Check Supported Image Type
---------------------------------------------------------*/
int is_supported_image(const char *filename)
{
    const char *ext = strrchr(filename, '.');

    if (ext == NULL)
        return -1;

    if (!strcasecmp(ext, ".jpg"))
        return JPEG;

    if (!strcasecmp(ext, ".jpeg"))
        return JPEG;

    if (!strcasecmp(ext, ".png"))
        return PNG;

    return -1;
}

/*---------------------------------------------------------
    Scan Current Directory for Images
---------------------------------------------------------*/
int scan_images(ImageInfo images[],
                Statistics *stats)
{
    DIR *dir;
    struct dirent *entry;

    int count = 0;

    dir = opendir(".");

    if (dir == NULL)
    {
        perror("opendir");
        return 0;
    }

    memset(stats, 0, sizeof(Statistics));

    while ((entry = readdir(dir)) != NULL)
    {
        int type = is_supported_image(entry->d_name);

        if (type == -1)
            continue;

        if (count >= MAX_IMAGES)
            break;

        strncpy(images[count].filename,
                entry->d_name,
                MAX_FILENAME - 1);

        images[count].filename[MAX_FILENAME - 1] = '\0';

        images[count].type = (ImageType)type;

        images[count].filesize =
            get_file_size(entry->d_name);

        images[count].width = 0;

        images[count].height = 0;

        images[count].loaded = 0;

        stats->total_images++;

        stats->total_size += images[count].filesize;

        if (type == JPEG)
            stats->jpg++;
        else
            stats->png++;

        count++;
    }

    closedir(dir);

    return count;
}

/*---------------------------------------------------------
    Print Image List
---------------------------------------------------------*/
void print_image_list(ImageInfo images[],
                      int count,
                      Statistics *stats)
{
    char size[32];

    printf("\n");
    printf("============================================================\n");
    printf("                    IMAGES FOUND\n");
    printf("============================================================\n\n");

    if (count == 0)
    {
        printf("No supported images found.\n\n");
        return;
    }

    printf("%-5s %-40s %-8s %-12s\n",
           "No.",
           "Filename",
           "Type",
           "Size");

    printf("------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        format_size(images[i].filesize,
                    size);

        printf("%-5d %-40s %-8s %-12s\n",
               i + 1,
               images[i].filename,
               images[i].type == JPEG ? "JPEG" : "PNG",
               size);
    }

    printf("------------------------------------------------------------\n");

    format_size(stats->total_size,
                size);

    printf("JPEG Images : %d\n",
           stats->jpg);

    printf("PNG Images  : %d\n",
           stats->png);

    printf("Total Images: %d\n",
           stats->total_images);

    printf("Total Size  : %s\n",
           size);

    printf("============================================================\n\n");
}

/*=========================================================
                    USER INTERFACE
=========================================================*/

/*---------------------------------------------------------
    Get Output PDF Name
---------------------------------------------------------*/
void get_output_filename(ProgramOptions *options)
{
    char outfile[MAX_FILENAME + 10];

    printf("Enter output PDF name (without .pdf): ");
    scanf("%255s", options->output_pdf);

    /* Remove ".pdf" if user entered it */
    char *ext = strrchr(options->output_pdf, '.');

    if (ext && !strcasecmp(ext, ".pdf"))
        *ext = '\0';

    sprintf(outfile, "%s.pdf", options->output_pdf);

    options->overwrite_existing = 0;

    if (file_exists(outfile))
    {
        printf("\nOutput file \"%s\" already exists.\n", outfile);

        if (yes_no_prompt("Overwrite existing file?"))
            options->overwrite_existing = 1;
        else
        {
            printf("\nOperation cancelled.\n");
            exit(EXIT_SUCCESS);
        }
    }
}

/*---------------------------------------------------------
    Read Advanced (Custom) Options
---------------------------------------------------------*/
void get_custom_options(ProgramOptions *options)
{
    int choice;

    printf("\n================ CUSTOM OPTIONS ================\n");

    /*---------------- Sorting ----------------*/
    printf("\nImage Sorting\n");
    printf("1. Name (A-Z)\n");
    printf("2. Name (Z-A)\n");
    printf("3. Size (Small-Large)\n");
    printf("4. Size (Large-Small)\n");

    printf("\nChoice : ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 2:
            options->sort_mode = SORT_NAME_DESC;
            break;

        case 3:
            options->sort_mode = SORT_SIZE_ASC;
            break;

        case 4:
            options->sort_mode = SORT_SIZE_DESC;
            break;

        default:
            options->sort_mode = SORT_NAME_ASC;
    }

    /*---------------- Page Mode ----------------*/
    printf("\nPage Layout\n");
    printf("1. Fit (Maintain Aspect Ratio)\n");
    printf("2. Fill (Crop to Page)\n");

    printf("\nChoice : ");
    scanf("%d", &choice);

    if(choice == 2)
        options->page_mode = PAGE_FILL;
    else
        options->page_mode = PAGE_FIT;

    /*---------------- Metadata ----------------*/
    printf("\nPDF Metadata\n");

    getchar();

    printf("Title    : ");
    fgets(options->metadata.title,
          sizeof(options->metadata.title),
          stdin);
    options->metadata.title[
        strcspn(options->metadata.title,"\n")] = '\0';

    printf("Author   : ");
    fgets(options->metadata.author,
          sizeof(options->metadata.author),
          stdin);
    options->metadata.author[
        strcspn(options->metadata.author,"\n")] = '\0';

    printf("Subject  : ");
    fgets(options->metadata.subject,
          sizeof(options->metadata.subject),
          stdin);
    options->metadata.subject[
        strcspn(options->metadata.subject,"\n")] = '\0';

    printf("Keywords : ");
    fgets(options->metadata.keywords,
          sizeof(options->metadata.keywords),
          stdin);
    options->metadata.keywords[
        strcspn(options->metadata.keywords,"\n")] = '\0';

    /*---------------- Auto Open ----------------*/
    options->open_pdf =
        yes_no_prompt("\nOpen generated PDF automatically?");
}

/*---------------------------------------------------------
    Final Confirmation
---------------------------------------------------------*/
int confirm_proceed(ImageInfo images[],
                    int count,
                    Statistics *stats,
                    ProgramOptions *options)
{
    char size[32];

    format_size(stats->total_size, size);

    printf("\n============================================================\n");
    printf("                 CONVERSION SUMMARY\n");
    printf("============================================================\n");

    printf("Output PDF   : %s.pdf\n",
           options->output_pdf);

    printf("Images Found : %d\n",
           count);

    printf("JPEG Images  : %d\n",
           stats->jpg);

    printf("PNG Images   : %d\n",
           stats->png);

    printf("Total Size   : %s\n",
           size);

    printf("Sorting      : ");

    switch(options->sort_mode)
    {
        case SORT_NAME_ASC:
            printf("Name (A-Z)\n");
            break;

        case SORT_NAME_DESC:
            printf("Name (Z-A)\n");
            break;

        case SORT_SIZE_ASC:
            printf("Size (Small-Large)\n");
            break;

        case SORT_SIZE_DESC:
            printf("Size (Large-Small)\n");
            break;
    }

    printf("Page Mode    : %s\n",
           options->page_mode == PAGE_FIT ?
           "Fit" : "Fill");

    printf("============================================================\n");

    return yes_no_prompt("Proceed with PDF creation?");
}

