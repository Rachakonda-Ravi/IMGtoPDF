# Image to PDF Converter

A C-based command-line application that converts multiple JPEG and PNG images into a single PDF using the **libHaru PDF Library**.

The project contains two versions:

- **IMG2PDF_Base.c** – Simple, beginner-friendly converter.
- **IMG2PDF_Advanced.c** – Feature-rich version with customization, logging, metadata, and enhanced user interaction.

---

# Features

## Common Features (Available in Both Versions)

- Converts multiple images into a single PDF
- Supports:
  - JPG
  - JPEG
  - PNG
- Automatically scans the current directory for supported images
- One image per PDF page
- Maintains image quality
- Creates A4-sized PDF pages
- Centers images on each page
- Cross-platform support:
  - Windows
  - Linux
  - macOS
- Uses the libHaru PDF library

---

# IMG2PDF_Base Features

Designed for quick and simple PDF generation.

### Image Handling

- Automatic image detection
- Alphabetical image ordering
- JPEG and PNG support

### PDF Generation

- Creates a PDF using all detected images
- Maintains image aspect ratio
- Simple user interface
- Fast execution

### User Interaction

- Enter output PDF filename
- Confirmation before conversion
- Basic success/failure messages

---

# IMG2PDF_Advanced Features

Includes everything from the Base version plus several advanced capabilities.

## Image Management

- Automatic image scanning
- Supports up to **1000 images**
- Displays:
  - Image filename
  - Image type
  - Individual file size
- Displays totals:
  - Total images
  - JPEG count
  - PNG count
  - Total size

---

## Image Sorting

Choose how images are ordered before PDF creation.

Available options:

1. Name (A → Z)
2. Name (Z → A)
3. Size (Small → Large)
4. Size (Large → Small)

---

## Page Layout Options

### Fit Mode

- Preserves aspect ratio
- Entire image remains visible
- Empty margins may appear

### Fill Mode

- Preserves aspect ratio
- Fills the entire page
- Crops excess portions if necessary

---

## PDF Metadata

Optional metadata fields:

- Title
- Author
- Subject
- Keywords

These become part of the generated PDF properties.

---

## Output File Management

- Prompts for output filename
- Automatically appends `.pdf`
- Detects existing files
- Asks before overwriting

---

## Automatic PDF Opening

After successful conversion you can choose to:

- Open automatically (Windows/macOS/Linux)
- Leave closed

---

## Logging

Automatically creates a **Logs/** folder.

Generates:

```
Logs/
    output_YYYY-MM-DD_HH-MM-SS.log
```

The log contains:

- Date
- Time
- Output filename
- Images found
- Images converted
- Failed images
- JPEG count
- PNG count
- Total image size

---

## Error Logging

If any image cannot be loaded, an error log is generated.

Example:

```
Logs/
    output_YYYY-MM-DD_HH-MM-SS_error.log
```

Contains:

- Failed filename
- Reason for failure

---

## Error Recovery

If an image cannot be loaded:

- Shows the filename
- Records the error
- Lets the user choose whether to:
  - Continue conversion
  - Cancel conversion

---

## Conversion Preview

Before creating the PDF the program displays:

- Output filename
- Image count
- JPEG count
- PNG count
- Total size
- Sorting method
- Page mode

User must confirm before conversion begins.

---

## Progress Display

During conversion:

```
[ 1/25] image01.jpg              OK
[ 2/25] image02.png              OK
[ 3/25] image03.jpg              Failed
```

Progress is shown for every image.

---

## Summary Report

After completion:

- Output PDF
- Images converted
- Images failed
- JPEG count
- PNG count
- Total image size
- Log file location

---

# Project Structure

```
IMG-PDF/
│
├── IMG2PDF_Base.c
├── IMG2PDF_Advanced.c
├── LICENSE
└── README.md
```

---

# Requirements

- GCC / Clang / MSVC
- libHaru PDF Library

---

# Compilation

Linux

```bash
gcc IMG2PDF_Base.c -o IMG2PDF_Base -lhpdf

gcc IMG2PDF_Advanced.c -o IMG2PDF_Advanced -lhpdf
```

Windows (MinGW)

```bash
gcc IMG2PDF_Base.c -o IMG2PDF_Base.exe -lhpdf

gcc IMG2PDF_Advanced.c -o IMG2PDF_Advanced.exe -lhpdf
```

---

# How to Use

## Step 1

Place the executable inside a folder containing your images.

Example:

```
Vacation/
    image1.jpg
    image2.png
    image3.jpg
    IMG2PDF_Advanced.exe
```

---

## Step 2

Run the program.

Linux

```bash
./IMG2PDF_Base 
```

```bash
./IMG2PDF_Advanced
```

Windows


```bash
IMG2PDF_Base.exe
```

```bash
IMG2PDF_Advanced.exe
```

---

## Step 3

Enter an output PDF name.

Example:

```
VacationAlbum
```

Produces:

```
VacationAlbum.pdf
```
---
For Base version jump to Step 8
---

## Step 4

Choose image sorting.

Example:

```
1
```

Sorts alphabetically.

---

## Step 5

Choose page layout.

```
1 = Fit

2 = Fill
```

---

## Step 6

(Optional)

Enter PDF metadata.

Example:

```
Title

Vacation 2025

Author

Ravi

Subject

Trip Photos

Keywords

Vacation Travel Family
```

---

## Step 7

Choose whether to automatically open the generated PDF.

```
Y
```

or

```
N
```

---

## Step 8

Review the conversion summary and confirm.

```
Proceed with PDF creation? (Y/N)
```

---

## Step 9

The program converts all images and creates:

```
VacationAlbum.pdf
```

If enabled:

- Opens the PDF automatically
- Creates log files

---

# Supported Image Formats

| Format | Supported |
|----------|-----------|
| JPG | ✔ |
| JPEG | ✔ |
| PNG | ✔ |
| GIF | ✘ |
| BMP | ✘ |
| TIFF | ✘ |
| WEBP | ✘ |

---

# Version Comparison

| Feature | Base | Advanced |
|----------|:---:|:--------:|
| JPEG Support | ✔ | ✔ |
| PNG Support | ✔ | ✔ |
| Automatic Image Scan | ✔ | ✔ |
| Aspect Ratio Preservation | ✔ | ✔ |
| Image Sorting | ✘ | ✔ |
| Fit / Fill Layout | ✘ | ✔ |
| PDF Metadata | ✘ | ✔ |
| Conversion Preview | ✘ | ✔ |
| Progress Display | ✘ | ✔ |
| Conversion Log | ✘ | ✔ |
| Error Log | ✘ | ✔ |
| Automatic PDF Opening | ✘ | ✔ |
| Overwrite Confirmation | ✘ | ✔ |
| Error Recovery | ✘ | ✔ |
| Summary Report | Basic | Detailed |

---

# Author

**Rachakonda Ravi**

---
