# Image to PDF Converter

A Python-based command-line application that converts multiple **JPEG and PNG images** into PDF documents.

The tool scans selected folders, validates images, creates A4-sized PDF pages, maintains image aspect ratio, and generates execution logs for every conversion session.

---

# Features

## Common Features

* Converts multiple images into PDF format
* Supports:

  * JPG
  * JPEG
  * PNG
* Processes one or multiple folders
* Validates images before conversion
* Creates one image per PDF page
* Maintains image aspect ratio
* Centers images on A4 pages
* Generates execution logs
* Handles invalid images safely

---

# Image Management

## Automatic Image Detection

The application automatically scans selected folders and detects supported image files.

Supported formats:

| Format | Supported |
| ------ | --------- |
| JPG    | ✔         |
| JPEG   | ✔         |
| PNG    | ✔         |
| GIF    | ✘         |
| BMP    | ✘         |
| TIFF   | ✘         |
| WEBP   | ✘         |

---

## Image Validation

Before PDF creation, every image is checked.

Validation includes:

* Detecting corrupted images
* Confirming image readability
* Separating valid and invalid files
* Reporting failed images

Example:

```
Checking images...

0001. image01.jpg  OK
0002. image02.png  OK
0003. damaged.jpg  ERROR
```

If invalid images are detected, the user can choose:

* Continue without invalid images
* Cancel PDF generation

---

# PDF Generation Features

## Page Layout

Generated PDFs use:

* A4 page size
* One image per page
* Automatic image scaling
* Aspect ratio preservation
* Center alignment

Images are resized to fit the page without distortion.

---

## PDF Naming Options

The application provides multiple output options.

### Automatic PDF Naming

* Uses the source folder name as PDF filename
* Adds current username as PDF author

Example:

```
Photos/
```

Creates:

```
Photos.pdf
```

---

### Custom PDF Name

Allows users to specify their own PDF filename.

Example:

```
Vacation_Album.pdf
```

---

### Custom PDF Name and Author

Allows entering:

* PDF filename
* Author information

---

# Multi Folder Processing

Multiple folders can be converted in a single execution.

Example:

```
C:\Images\Invoices,C:\Images\Reports
```

Each folder is processed separately.

---

# Logging System

The application automatically creates an output folder:

```
output/
```

Logs contain:

* Conversion date and time
* Folder information
* Image validation results
* Generated PDF path
* Number of processed images
* Number of skipped images
* Processing errors

Example:

```
output/

├── Documents.pdf
└── 07082026_1534_Log.txt
```

---

# Error Handling

The application handles:

* Missing folders
* Empty folders
* Invalid image files
* PDF generation errors
* User cancellation

All processing information is recorded in log files.

---

# Project Structure

```
ImageToPDF/
│
├── img2pdf.py
│
├── README.md
│
└── output/
    ├── Generated PDFs
    └── Log Files
```

---

# Requirements

## Software Requirements

* Python 3.8 or higher

## Python Libraries

Required packages:

| Package   | Purpose                      |
| --------- | ---------------------------- |
| Pillow    | Image loading and validation |
| ReportLab | PDF creation                 |

---

# Installation

## Step 1 - Install Dependencies

Run:

```bash
pip install Pillow reportlab
```

---

## Step 2 - Verify Project Files

Ensure the project contains:

```
img2pdf.py
README.md
```

---

# How To Run

Execute:

```bash
python img2pdf.py
```

---

# Usage Guide

## Step 1 - Enter Image Folder(s)

The application displays:

```
Enter folder path(s)

Separate multiple folders using comma

Folder:
```

Example:

```
C:\Photos\Trip,C:\Photos\Documents
```

---

## Step 2 - Image Scanning

The program scans folders and displays:

```
Images Found : 25
```

---

## Step 3 - Validate Images

Images are checked automatically.

Example:

```
0001. photo01.jpg OK
0002. photo02.png OK
```

If invalid images are found:

```
Continue without invalid images? (Y/N):
```

Choose:

```
Y
```

to continue.

---

## Step 4 - Select PDF Creation Mode

Choose:

```
1. Automatic PDF Naming
2. Custom PDF Name
3. Custom PDF Name & Author
4. Cancel Operation
```

---

## Step 5 - PDF Creation

After successful conversion:

```
PDF CREATED

Output : output/example.pdf
Processed : 25
Skipped : 0
```

---

# Example Workflow

Input:

```
Vacation/

├── image01.jpg
├── image02.png
└── image03.jpeg
```

Run:

```bash
python img2pdf.py
```

Select:

```
1. Automatic PDF Naming
```

Output:

```
output/
├── 07082026_1543_Log.txt
└── Vacation.pdf
```

---

# Limitations

* Only JPG, JPEG, and PNG formats are supported
* Images are scanned only from selected folders
* Subfolders are not scanned automatically
* PDF pages are fixed to A4 size
* Command-line interface only

---

# Author

**Rachakonda Ravi**
---