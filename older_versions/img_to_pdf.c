#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <strings.h>
#include <hpdf.h>
#define MAX_IMAGES 1000

int cmp(const void *a, const void *b)
{
    return strcmp(*(char **)a, *(char **)b);
}

int is_supported_image(const char *filename)
{
    char *ext = strrchr(filename, '.');

    if (!ext)
        return 0;

    return (!strcasecmp(ext, ".jpg") ||
            !strcasecmp(ext, ".jpeg") ||
            !strcasecmp(ext, ".png"));
}

int scan_images(char *images[])
{
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir(".");

    if (!dir)
    {
        perror("opendir");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (is_supported_image(entry->d_name))
        {
            images[count++] = strdup(entry->d_name);

            if (count >= MAX_IMAGES)
                break;
        }
    }

    closedir(dir);

    return count;
}

void print_image_list(char *images[], int count)
{
    int jpg = 0, png = 0;

    printf("\nScanning current directory...\n");
    printf("Found %d supported image(s).\n\n", count);

    printf("Images Found\n");
    printf("------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        char *ext = strrchr(images[i], '.');

        if (!strcasecmp(ext, ".png"))
        {
            printf("%2d. %-35s [PNG]\n", i + 1, images[i]);
            png++;
        }
        else
        {
            printf("%2d. %-35s [JPEG]\n", i + 1, images[i]);
            jpg++;
        }
    }

    printf("------------------------------------------------------\n");
    printf("JPEG Images : %d\n", jpg);
    printf("PNG Images  : %d\n", png);
    printf("Total Images: %d\n", count);
}


int confirm_proceed(void)
{
    char choice;

    while (1)
    {
        printf("\nProceed with PDF creation? (Y/N): ");
        scanf(" %c", &choice);

        if (choice == 'Y' || choice == 'y')
            return 1;

        if (choice == 'N' || choice == 'n')
            return 0;

        printf("Invalid choice. Please enter Y or N.\n");
    }
}

HPDF_Image load_image(HPDF_Doc pdf, const char *filename)
{
    char *ext = strrchr(filename, '.');

    if (!ext)
        return NULL;

    if (!strcasecmp(ext, ".jpg") ||
        !strcasecmp(ext, ".jpeg"))
    {
        return HPDF_LoadJpegImageFromFile(pdf, filename);
    }

    if (!strcasecmp(ext, ".png"))
    {
        return HPDF_LoadPngImageFromFile(pdf, filename);
    }

    return NULL;
}

int create_pdf(char *images[], int count, const char *outfile,
               int *processed, int *skipped)
{
    HPDF_Doc pdf = HPDF_New(NULL, NULL);

    if (!pdf)
    {
        printf("Unable to create PDF.\n");
        return 0;
    }

    *processed = 0;
    *skipped = 0;

    printf("\nCreating PDF...\n\n");

    for (int i = 0; i < count; i++)
    {
        printf("[%d/%d] Processing %-35s", i + 1, count, images[i]);

        HPDF_Image img = load_image(pdf, images[i]);

        if (!img)
        {
            char choice;

            printf("  FAILED\n");
            printf("Unable to load '%s'\n", images[i]);

            while (1)
            {
                printf("Continue without this image? (Y/N): ");
                scanf(" %c", &choice);

                if (choice == 'Y' || choice == 'y')
                {
                    (*skipped)++;
                    break;
                }

                if (choice == 'N' || choice == 'n')
                {
                    HPDF_Free(pdf);
                    return 0;
                }

                printf("Invalid choice.\n");
            }

            continue;
        }

        HPDF_Page page = HPDF_AddPage(pdf);

        HPDF_Page_SetWidth(page, 595);
        HPDF_Page_SetHeight(page, 842);

        HPDF_Page_DrawImage(page, img, 0, 0, 595, 842);

        printf("  OK\n");

        (*processed)++;
    }

    if (HPDF_SaveToFile(pdf, outfile) != HPDF_OK)
    {
        printf("\nError saving PDF.\n");
        HPDF_Free(pdf);
        return 0;
    }

    HPDF_Free(pdf);

    return 1;
}

void print_summary(char *images[], int count,
                   int processed, int skipped,
                   const char *outfile)
{
    int jpg = 0, png = 0;

    for (int i = 0; i < count; i++)
    {
        char *ext = strrchr(images[i], '.');

        if (!strcasecmp(ext, ".png"))
            png++;
        else
            jpg++;
    }

    printf("\n==================================================\n");
    printf("            PDF CREATED SUCCESSFULLY\n");
    printf("==================================================\n");

    printf("Total Images Found : %d\n", count);
    printf("JPEG Images        : %d\n", jpg);
    printf("PNG Images         : %d\n", png);

    printf("\nSuccessfully Added : %d\n", processed);
    printf("Skipped            : %d\n", skipped);

    printf("\nOutput File        : %s\n", outfile);

    printf("==================================================\n");
}


void free_images(char *images[], int count)
{
    for (int i = 0; i < count; i++)
        free(images[i]);
}

int main()
{
    char *images[MAX_IMAGES];
    char pdfname[256];
    char outfile[300];
    int count;
    int processed;
    int skipped;

    printf("Enter output PDF name (without .pdf): ");
    scanf("%255s", pdfname);

    snprintf(outfile, sizeof(outfile), "%s.pdf", pdfname);

    count = scan_images(images);

    if (count <= 0)
    {
        printf("No supported images found.\n");
        return 1;
    }

    qsort(images, count, sizeof(char *), cmp);

    print_image_list(images, count);

    if (!confirm_proceed())
    {
        printf("Operation cancelled.\n");
        free_images(images, count);
        return 0;
    }

    if (create_pdf(images, count, outfile,
                   &processed, &skipped))
    {
        print_summary(images, count,
                      processed, skipped,
                      outfile);
    }
    else
    {
        printf("\nPDF creation aborted.\n");
    }

    free_images(images, count);

    return 0;
}