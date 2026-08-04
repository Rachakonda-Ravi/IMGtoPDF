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
#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#include <direct.h>
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
#define MAX_ERRORS        MAX_IMAGES

typedef enum
{
    IMAGE_UNKNOWN = -1,
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
    PAGE_FIT,

    PAGE_FILL

} PageMode;

typedef enum
{
    DEFAULT_MODE,
    ADVANCED_MODE
} ProgramMode;

typedef struct
{
    char name[MAX_FILENAME];
    char path[MAX_FILENAME];
} SubFolderInfo;

typedef struct
{
    char filename[MAX_FILENAME];

    char fullpath[512];

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

    char output_pdf[MAX_FILENAME];

    char source_directory[MAX_FILENAME];

    int output_from_cli;

    int split_mode;

    SortMode sort_mode;

    PageMode page_mode;

    int open_pdf;

    int overwrite_existing;

    PDFMetadata metadata;

} ProgramOptions;

typedef struct
{
    int total_images;

    int converted_images;

    int failed_loads;

    int jpg;

    int png;

    long long total_size;

} Statistics;

typedef struct
{
    char filename[MAX_FILENAME];

    char reason[256];

} ErrorEntry;

typedef struct
{
    ProgramOptions *options;

    Statistics stats;

    ErrorEntry errors[MAX_ERRORS];

    int error_count;

    char log_file[512];

    char error_file[512];

    ImageInfo images[MAX_IMAGES];

    int image_count;

} ProgramContext;

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

int scan_subfolders(const char *root,
                    SubFolderInfo folders[]);

int folder_has_images(const char *directory);

int file_exists(const char *filename);

long long get_file_size(const char *filename);

void format_size(long long bytes, char *buffer);

void get_datetime(char *date, char *time);

int convert_directory(const char *directory,
                      ProgramContext *ctx);

void get_log_filename(const char *pdfname,
                      char *log_file,
                      char *error_file);

/*--------------------- Image Handling ----------------------*/
ImageType is_supported_image(const char *filename);

int scan_images(const char *directory,
                ImageInfo images[],
                Statistics *stats);

void sort_images(ImageInfo images[],
                 int count,
                 SortMode mode);

void print_image_list(ImageInfo images[],
                      int count,
                      Statistics *stats);

/*--------------------- User Interface ----------------------*/
void get_output_filename(ProgramOptions *options);

void get_advanced_options(ProgramOptions *options);

void parse_command_line(int argc,
                        char *argv[],
                        ProgramOptions *options);

int create_split_pdfs(const char *root_directory,
                      ProgramOptions *options);
/*--------------------- PDF Functions -----------------------*/
int create_pdf(ImageInfo images[],
               int count,
               ProgramOptions *options,
               Statistics *stats,
               ErrorEntry errors[],
               int *error_count,
               const char *log_file);

int prepare_output_filename(ProgramOptions *options);

void open_pdf(const char *filename);

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

/*---------------------- PDF FOUNDATION ----------------------------*/
HPDF_Doc create_pdf_document(void);

void apply_pdf_metadata(HPDF_Doc pdf,
                        const ProgramOptions *options);

HPDF_Page create_page(HPDF_Doc pdf);

HPDF_Image load_image_file(HPDF_Doc pdf,
                           const ImageInfo *image);

void calculate_fit(float img_w,
                   float img_h,
                   float page_w,
                   float page_h,
                   float *draw_x,
                   float *draw_y,
                   float *draw_w,
                   float *draw_h);

void calculate_fill(float img_w,
                    float img_h,
                    float page_w,
                    float page_h,
                    float *draw_x,
                    float *draw_y,
                    float *draw_w,
                    float *draw_h);
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
    if (mode == ADVANCED_MODE)
    {
        printf("Mode : Advanced\n\n");
        printf("Advanced options enabled.\n\n");
    }
    else
    {
        printf("Mode : Default\n\n");

        printf("For advanced options, run:\n\n");
        printf("    ./image2pdf ADVANCED\n\n");
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
            return EXIT_SUCCESS;

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

/*---------------------------------------------------------
    Parse Command Line Arguments
---------------------------------------------------------*/
void parse_command_line(int argc,
                        char *argv[],
                        ProgramOptions *options)
{
    /* Defaults */

    options->mode = DEFAULT_MODE;

    strcpy(options->source_directory, ".");

    options->output_from_cli = 0;

    options->split_mode = 0;

    for (int i = 1; i < argc; i++)
    {
        if (!strcasecmp(argv[i], "ADVANCED"))
        {
            options->mode = ADVANCED_MODE;
        }

        else if (!strcmp(argv[i], "--split"))
        {
            options->split_mode = 1;
            options->mode = ADVANCED_MODE;
        }

        else if (!strcmp(argv[i], "-o"))
        {
            if (i + 1 < argc)
            {
                strncpy(options->output_pdf,
                        argv[++i],
                        MAX_FILENAME - 1);

                options->output_pdf[MAX_FILENAME - 1] = '\0';

                char *ext = strrchr(options->output_pdf, '.');

                if (ext && !strcasecmp(ext, ".pdf"))
                    *ext = '\0';

                options->output_from_cli = 1;
            }
        }

        else
        {
            strncpy(options->source_directory,
                    argv[i],
                    MAX_FILENAME - 1);

            options->source_directory[MAX_FILENAME - 1] = '\0';

            options->mode = ADVANCED_MODE;
        }
    }
}

/*=========================================================
                    FILE UTILITIES
=========================================================*/


/*---------------------------------------------------------
    Directory conversion
---------------------------------------------------------*/

int convert_directory(const char *directory,
                      ProgramContext *ctx)
{
    ProgramOptions *options = ctx->options;

     Statistics stats = {0};

    ErrorEntry errors[MAX_ERRORS];

    int error_count = 0;

    char log_file[512];

    char error_file[512];

    
    ctx->image_count =
        scan_images(directory,
                    ctx->images,
                    &ctx->stats);

    print_image_list(ctx->images,
                 ctx->image_count,
                 &ctx->stats);

    create_pdf(ctx->images,
           ctx->image_count,
           options,
           &ctx->stats,
           ctx->errors,
           &ctx->error_count,
           ctx->log_file);

    if (ctx->image_count == 0)
    {
        printf("\nNo supported images found.\n");
        return EXIT_SUCCESS;
    }

    sort_images(ctx->images,
                ctx->image_count,
                options->sort_mode);

    print_image_list(ctx->images,
                     ctx->image_count,
                     &ctx->stats);

    if (options->mode == ADVANCED_MODE)
    {
        get_custom_options(options);
    }

    if (!prepare_output_filename(options))
        return EXIT_FAILURE;

    if (!options->output_from_cli)
    {
        get_output_filename(options);
    }

    get_log_filename(options->output_pdf,
                     ctx->log_file,
                     ctx->error_file);

    if (!confirm_conversion())
    {
        printf("\nCancelled.\n");
        return EXIT_SUCCESS;
    }

    create_pdf(ctx->images,
               ctx->image_count,
               options,
               &ctx->stats,
               ctx->errors,
               &ctx->error_count,
               ctx->log_file);

    write_log(ctx->options,
            ctx->log_file,
            &ctx->stats);

    if (ctx->error_count > 0)
    {
        write_error_log(ctx->error_file,
                        ctx->errors,
                        ctx->error_count);
    }

    print_summary(ctx->options,
            ctx->log_file,
            &ctx->stats);

    return EXIT_SUCCESS;
}   

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
    Check If a Subfolder Exists
---------------------------------------------------------*/
int scan_subfolders(const char *root,
                    SubFolderInfo folders[])
{
    DIR *dir;
    struct dirent *entry;

    int count = 0;

    dir = opendir(root);

    if (!dir)
        return EXIT_SUCCESS;

    while ((entry = readdir(dir)) != NULL)
    {

        if (!strcmp(entry->d_name, ".") ||
            !strcmp(entry->d_name, ".."))
            continue;

        char fullpath[MAX_FILENAME];

        snprintf(fullpath,
                 sizeof(fullpath),
                 "%s/%s",
                 root,
                 entry->d_name);

        struct stat st;

        if (stat(fullpath, &st) == 0 &&
            S_ISDIR(st.st_mode))
        {
            strncpy(folders[count].name,
                    entry->d_name,
                    MAX_FILENAME - 1);

            strncpy(folders[count].path,
                    fullpath,
                    MAX_FILENAME - 1);

            count++;
        }
    }

    closedir(dir);

    return count;
}

int folder_has_images(const char *directory)
{
    DIR *dir;
    struct dirent *entry;

    printf("\nScanning folder: %s\n", directory);

    dir = opendir(directory);

    if (!dir)
    {
        printf("Cannot open folder!\n");
        return EXIT_SUCCESS;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (!strcmp(entry->d_name, ".") ||
            !strcmp(entry->d_name, ".."))
            continue;

        if (is_supported_image(entry->d_name))
        {
            closedir(dir);
            return 1;
        }
    }

    printf("   -> No Images\n");

    closedir(dir);

    return EXIT_SUCCESS;
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
        return EXIT_SUCCESS;

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

    snprintf(log_file,
         512,
         "%s/%s_%s_%s.log",
         LOG_FOLDER,
         pdfname,
         date,
         time_str);

    snprintf(error_file,
         512,
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
ImageType is_supported_image(const char *filename)
{
    const char *ext = strrchr(filename, '.');

    if (ext == NULL)
        return IMAGE_UNKNOWN;

    if (!strcasecmp(ext, ".jpg") ||
        !strcasecmp(ext, ".jpeg"))
        return JPEG;

    if (!strcasecmp(ext, ".png"))
        return PNG;

    return -1;
}

/*---------------------------------------------------------
    Scan Current Directory for Images
---------------------------------------------------------*/
int scan_images(const char *directory,
                ImageInfo images[],
                Statistics *stats)
{
    DIR *dir;
    struct dirent *entry;
    char fullpath[512];
    int count = 0;

    dir = opendir(directory);
    if (dir == NULL)
    {
        perror("opendir");
        return EXIT_SUCCESS;
    }

    memset(stats, 0, sizeof(Statistics));

    while ((entry = readdir(dir)) != NULL)
    {
        ImageType type = is_supported_image(entry->d_name);

        if (type == IMAGE_UNKNOWN)
            continue;

        if (count >= MAX_IMAGES)
            break;

        strncpy(images[count].filename,
            entry->d_name,
            MAX_FILENAME - 1);
        snprintf(images[count].fullpath,
            sizeof(images[count].fullpath),
            "%s/%s",
            directory,
            entry->d_name);

        images[count].filename[MAX_FILENAME - 1] = '\0';

        images[count].type = type;

        snprintf(fullpath,
            sizeof(fullpath),
            "%s/%s",
            directory,
            entry->d_name);

        images[count].filesize =
            get_file_size(fullpath);
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
int prepare_output_filename(ProgramOptions *options)
{
    if (options->output_from_cli)
    {
        char outfile[MAX_FILENAME + 10];

        snprintf(outfile,
                 sizeof(outfile),
                 "%s.pdf",
                 options->output_pdf);

        if (!options->overwrite_existing &&
            file_exists(outfile))
        {
            printf("\nOutput file \"%s\" already exists.\n",
                   outfile);

            return EXIT_SUCCESS;
        }

        return 1;
    }

    get_output_filename(options);

    return 1;
}

void get_output_filename(ProgramOptions *options)
{
    char outfile[MAX_FILENAME + 10];

    printf("Enter output PDF name (without .pdf): ");
    scanf("%255s", options->output_pdf);

    /* Remove ".pdf" if user entered it */
    char *ext = strrchr(options->output_pdf, '.');

    if (ext && !strcasecmp(ext, ".pdf"))
        *ext = '\0';

    snprintf(outfile,
         sizeof(outfile),
         "%s.pdf",
         options->output_pdf);

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
    Read Advanced Options
---------------------------------------------------------*/
void get_advanced_options(ProgramOptions *options)
{
    int choice;

    printf("\n================ ADVANCED OPTIONS ================\n");

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

/*=========================================================
                    PDF FOUNDATION
=========================================================*/

/*---------------------------------------------------------
    Create PDF Document
---------------------------------------------------------*/
HPDF_Doc create_pdf_document(void)
{
    HPDF_Doc pdf;

    pdf = HPDF_New(NULL, NULL);

    if (pdf == NULL)
    {
        printf("\nError : Unable to create PDF document.\n");
        return NULL;
    }

    HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);

    return pdf;
}

/*---------------------------------------------------------
    Apply PDF Metadata
---------------------------------------------------------*/
void apply_pdf_metadata(HPDF_Doc pdf,
                        const ProgramOptions *options)
{
    if (pdf == NULL)
        return;

    if (strlen(options->metadata.title) > 0)
        HPDF_SetInfoAttr(pdf,
                         HPDF_INFO_TITLE,
                         options->metadata.title);

    if (strlen(options->metadata.author) > 0)
        HPDF_SetInfoAttr(pdf,
                         HPDF_INFO_AUTHOR,
                         options->metadata.author);

    if (strlen(options->metadata.subject) > 0)
        HPDF_SetInfoAttr(pdf,
                         HPDF_INFO_SUBJECT,
                         options->metadata.subject);

    if (strlen(options->metadata.keywords) > 0)
        HPDF_SetInfoAttr(pdf,
                         HPDF_INFO_KEYWORDS,
                         options->metadata.keywords);
}

/*---------------------------------------------------------
    Create Standard A4 Page
---------------------------------------------------------*/
HPDF_Page create_page(HPDF_Doc pdf)
{
    HPDF_Page page;

    page = HPDF_AddPage(pdf);

    if (page == NULL)
        return NULL;

    HPDF_Page_SetWidth(page, PAGE_WIDTH);
    HPDF_Page_SetHeight(page, PAGE_HEIGHT);

    return page;
}

/*---------------------------------------------------------
    Load Image (JPEG / PNG)
---------------------------------------------------------*/
HPDF_Image load_image_file(HPDF_Doc pdf,
                           const ImageInfo *image)
{
    if (image == NULL)
        return NULL;

    switch (image->type)
    {
        case JPEG:
            return HPDF_LoadJpegImageFromFile(pdf,
                                              image->fullpath);

        case PNG:
            return HPDF_LoadPngImageFromFile(pdf,
                                             image->fullpath);

        default:
            return NULL;
    }
}

/*---------------------------------------------------------
    Calculate FIT Dimensions
---------------------------------------------------------*/
void calculate_fit(float img_w,
                   float img_h,
                   float page_w,
                   float page_h,
                   float *draw_x,
                   float *draw_y,
                   float *draw_w,
                   float *draw_h)
{
    float scale_x = page_w / img_w;
    float scale_y = page_h / img_h;
    float scale = (scale_x < scale_y) ? scale_x : scale_y;

    *draw_w = img_w * scale;
    *draw_h = img_h * scale;

    *draw_x = (page_w - *draw_w) / 2.0f;
    *draw_y = (page_h - *draw_h) / 2.0f;
}

/*---------------------------------------------------------
    Calculate FILL Dimensions
---------------------------------------------------------*/
void calculate_fill(float img_w,
                    float img_h,
                    float page_w,
                    float page_h,
                    float *draw_x,
                    float *draw_y,
                    float *draw_w,
                    float *draw_h)
{
    float scale_x = page_w / img_w;
    float scale_y = page_h / img_h;
    float scale = (scale_x > scale_y) ? scale_x : scale_y;

    *draw_w = img_w * scale;
    *draw_h = img_h * scale;

    *draw_x = (page_w - *draw_w) / 2.0f;
    *draw_y = (page_h - *draw_h) / 2.0f;
}

/*=========================================================
                        PDF ENGINE
=========================================================*/

/*---------------------------------------------------------
    Create Split PDFs (One PDF per Subfolder)
---------------------------------------------------------*/

int create_split_pdfs(const char *root_directory,
                      ProgramOptions *options)
{
    SubFolderInfo folders[256];
    int folder_count;

    folder_count = scan_subfolders(root_directory, folders);

    printf("\nFound %d folders\n", folder_count);

    if (folder_count == 0)
    {
        printf("\nNo subfolders found.\n");
        return EXIT_SUCCESS;
    }

    for (int i = 0; i < folder_count; i++)
    {
        ProgramOptions folder_options = *options;

        if (!folder_has_images(folders[i].path))
        {
            printf("\nSkipping \"%s\" (No supported images)\n",
                   folders[i].name);
            continue;
        }

        printf("\n============================================================\n");
        printf("Converting Folder : %s\n", folders[i].name);
        printf("============================================================\n");

        strncpy(folder_options.output_pdf,
                folders[i].name,
                MAX_FILENAME - 1);

        folder_options.output_pdf[MAX_FILENAME - 1] = '\0';

        folder_options.output_from_cli = 1;

        convert_directory(folders[i].path,
                          &folder_options);
    }

    return EXIT_SUCCESS;
}

/*---------------------------------------------------------
    Add One Image to PDF
---------------------------------------------------------*/
int add_image_to_pdf(HPDF_Doc pdf,
                     ImageInfo *image,
                     ProgramOptions *options,
                     ErrorEntry errors[],
                     int *error_count)
{
    HPDF_Page page;
    HPDF_Image himg;

    float img_w;
    float img_h;

    float x, y;
    float w, h;

    page = create_page(pdf);

    if(page == NULL)
        return EXIT_SUCCESS;

    himg = load_image_file(pdf, image);

    if(himg == NULL)
    {
        if (*error_count < MAX_ERRORS)
        {
            strncpy(errors[*error_count].filename,
                    image->fullpath,
                    MAX_FILENAME - 1);
                
            errors[*error_count].filename[MAX_FILENAME - 1] = '\0';

            strncpy(errors[*error_count].reason,
                    "Unable to load image.",
                    sizeof(errors[*error_count].reason) - 1);
                
            errors[*error_count].reason[
                sizeof(errors[*error_count].reason) - 1] = '\0';
            
            (*error_count)++;
        }

        image->loaded = 0;

        printf("\nFailed to load : %s\n",
               image->fullpath);

        if(!yes_no_prompt("Continue with remaining images?"))
            return -1;

        return EXIT_SUCCESS;
    }

    image->loaded = 1;

    img_w = (float)HPDF_Image_GetWidth(himg);
    img_h = (float)HPDF_Image_GetHeight(himg);

    image->width = img_w;
    image->height = img_h;

    if(options->page_mode == PAGE_FIT)
    {
        calculate_fit(img_w,
                      img_h,
                      PAGE_WIDTH,
                      PAGE_HEIGHT,
                      &x,
                      &y,
                      &w,
                      &h);
    }
    else
    {
        calculate_fill(img_w,
                       img_h,
                       PAGE_WIDTH,
                       PAGE_HEIGHT,
                       &x,
                       &y,
                       &w,
                       &h);
    }

    HPDF_Page_DrawImage(page,
                        himg,
                        x,
                        y,
                        w,
                        h);

    return 1;
}

/*---------------------------------------------------------
    Create Complete PDF
---------------------------------------------------------*/
int create_pdf(ImageInfo images[],
               int count,
               ProgramOptions *options,
               Statistics *stats,
               ErrorEntry errors[],
               int *error_count,
               const char *log_file)
{
    HPDF_Doc pdf;

    char outfile[MAX_FILENAME + 10];

    int result;

    pdf = create_pdf_document();

    if(pdf == NULL)
        return EXIT_SUCCESS;

    apply_pdf_metadata(pdf,
                       options);

    snprintf(outfile,
         sizeof(outfile),
         "%s.pdf",
         options->output_pdf);

    printf("\nCreating PDF...\n\n");

    for(int i = 0; i < count; i++)
    {
        printf("[%3d/%3d] %-40s",
               i + 1,
               count,
               images[i].filename);

        fflush(stdout);

        result = add_image_to_pdf(pdf,
                                  &images[i],
                                  options,
                                  errors,
                                  error_count);

        if(result == -1)
        {
            printf(" Aborted\n");

            HPDF_Free(pdf);

            return EXIT_SUCCESS;
        }

        if(result == 0)
        {
            printf(" Failed\n");

            stats->failed_loads++;

            continue;
        }

        printf(" OK\n");

        stats->converted_images++;
    }

    if(HPDF_SaveToFile(pdf,
                       outfile) != HPDF_OK)
    {
        printf("\nUnable to save PDF.\n");

        HPDF_Free(pdf);

        return EXIT_SUCCESS;
    }

    HPDF_Free(pdf);

    printf("\nPDF created successfully.\n");

    printf("Output : %s\n",
           outfile);

    if(options->open_pdf)
    open_pdf(outfile);

    return 1;
}

/*---------------------------------------------------------
    Open Generated PDF
---------------------------------------------------------*/
void open_pdf(const char *filename)
{
#ifdef _WIN32

    ShellExecute(NULL,
                 "open",
                 filename,
                 NULL,
                 NULL,
                 SW_SHOWNORMAL);

#elif __APPLE__

    char command[512];

    snprintf(command,
             sizeof(command),
             "open \"%s\"",
             filename);

    system(command);

#else

    char command[512];

    snprintf(command,
             sizeof(command),
             "xdg-open \"%s\" >/dev/null 2>&1 &",
             filename);

    system(command);

#endif
}

/*=========================================================
                        LOGGING
=========================================================*/

/*---------------------------------------------------------
    Write Conversion Log
---------------------------------------------------------*/
void write_log(const ProgramOptions *options,
               const Statistics *stats,
               const char *log_file)
{
    FILE *fp;
    char size[32];
    char date[20];
    char time_str[20];

    fp = fopen(log_file, "w");

    if(fp == NULL)
    {
        printf("Unable to create log file.\n");
        return;
    }

    get_datetime(date, time_str);

    format_size(stats->total_size, size);

    fprintf(fp,
            "============================================================\n");

    fprintf(fp,
            "               IMAGE TO PDF CONVERTER LOG\n");

    fprintf(fp,
            "============================================================\n\n");

    fprintf(fp,"Date               : %s\n",date);
    fprintf(fp,"Time               : %s\n\n",time_str);

    fprintf(fp,"Output PDF         : %s.pdf\n",
            options->output_pdf);

    fprintf(fp,"Images Found       : %d\n",
            stats->total_images);

    fprintf(fp,"Images Converted   : %d\n",
            stats->converted_images);

    fprintf(fp,"Images Failed      : %d\n",
            stats->failed_loads);

    fprintf(fp,"JPEG Images        : %d\n",
            stats->jpg);

    fprintf(fp,"PNG Images         : %d\n",
            stats->png);

    fprintf(fp,"Total Image Size   : %s\n",
            size);

    fprintf(fp,"\nConversion Completed Successfully.\n");

    fclose(fp);
}

/*---------------------------------------------------------
    Write Error Log
---------------------------------------------------------*/
void write_error_log(const ErrorEntry errors[],
                     int error_count,
                     const char *error_file)
{
    FILE *fp;

    if(error_count == 0)
        return;

    fp = fopen(error_file,"w");

    if(fp == NULL)
        return;

    fprintf(fp,
            "============================================================\n");

    fprintf(fp,
            "                    ERROR REPORT\n");

    fprintf(fp,
            "============================================================\n\n");

    for(int i=0;i<error_count;i++)
    {
        fprintf(fp,
                "%3d. %-40s : %s\n",
                i+1,
                errors[i].filename,
                errors[i].reason);
    }

    fclose(fp);
}

/*=========================================================
                    FINAL REPORT
=========================================================*/

/*---------------------------------------------------------
    Print Final Summary
---------------------------------------------------------*/
void print_summary(const ProgramOptions *options,
                   const Statistics *stats,
                   const char *log_file)
{
    char size[32];

    format_size(stats->total_size,size);

    printf("\n");
    printf("============================================================\n");
    printf("                  CONVERSION COMPLETE\n");
    printf("============================================================\n\n");

    printf("Output PDF        : %s.pdf\n",
           options->output_pdf);

    printf("Images Converted  : %d\n",
           stats->converted_images);

    printf("Images Failed     : %d\n",
           stats->failed_loads);

    printf("JPEG Images       : %d\n",
           stats->jpg);

    printf("PNG Images        : %d\n",
           stats->png);

    printf("Total Image Size  : %s\n",
           size);

    printf("Log File          : %s\n",
           log_file);

    printf("\n============================================================\n");
}

/*=========================================================
                        CLEANUP
=========================================================*/

/*---------------------------------------------------------
    Cleanup
---------------------------------------------------------*/
void cleanup(ImageInfo images[],
             int count)
{
    (void)images;
    (void)count;

    /* Reserved for future cleanup.
       Keeping this function allows easy expansion later,
       such as freeing dynamically allocated memory,
       deleting temporary files, etc. */
}

/*=========================================================
                            MAIN
=========================================================*/

int main(int argc, char *argv[])
{
    ProgramOptions options;

    ProgramContext ctx;

    memset(&ctx, 0, sizeof(ctx));

    ctx.options = &options;
    initialize_options(&options);

    parse_command_line(argc,
                       argv,
                       &options);

    print_header();

    print_mode_info(options.mode);

    create_logs_folder();

    if (options.split_mode)
    {
        return create_split_pdfs(options.source_directory,
                                 &options);
    }

    return convert_directory(options.source_directory,
                         &ctx);
}