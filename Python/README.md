# IMG-PDF --- Image & PDF Utility Suite

A professional Python command-line toolkit for managing images and PDFs.

**IMG-PDF** provides utilities to:

-   Convert JPG/JPEG/PNG images into high-quality PDF files
-   Merge multiple PDFs with automatic Table of Contents and bookmarks
-   Scan folders and generate batch processing lists
-   Rename folders or files using prefix replacement

The project is designed as a lightweight offline utility. All Python
files are distributed through GitHub Releases:


### Repository:
```md 
https://github.com/Rachakonda-Ravi/IMG-PDF/releases
``` 

------------------------------------------------------------------------

## Features Overview

| Tool | File | Purpose |
|------|------|---------|
| Image to PDF Converter | `img2pdf.py` | Convert images into A4 PDF documents |
| PDF Merge Utility | `PDF_Merge.py` | Merge PDFs with TOC, links and bookmarks |
| Folder Scanner | `Folders_Scan.py` | Export subfolder paths for batch conversion |
| Prefix Rename Tool | `Folders_Rename.py` | Rename files/folders by replacing prefixes |



------------------------------------------------------------------------

# 1. Image to PDF Converter (`img2pdf.py`)

## Features

-   Supports JPG, JPEG and PNG images
-   Converts images into PDF pages
-   A4 page generation
-   Automatic image scaling
-   Preserves aspect ratio
-   Centers images on pages
-   Detects corrupted images before conversion
-   Supports multiple folders in one run
-   Creates detailed logs
-   Adds PDF author metadata

## Installation

Install required dependencies:

``` bash
pip install Pillow reportlab
```

## Run

``` bash
python img2pdf.py
```

## Input Example

Single folder:

    C:\Photos\Vacation

Multiple folders:

    C:\Photos\Trip1,C:\Photos\Trip2,C:\Photos\Trip3

Large batch processing:

1.  Run:

``` bash
python Folders_Scan.py
```

2.  Provide the main folder path.

3.  The program creates:
    subfolders.txt


4.  Use that file as input in `img2pdf.py`.

Example:

    C:\Photos\subfolders.txt

## PDF Naming Modes

The converter supports:

1.  Automatic naming

Example:

    Photos → Photos.pdf

2.  Custom PDF name

Example:

    MyAlbum.pdf

3.  Custom PDF name + author metadata

------------------------------------------------------------------------

# 2. PDF Merge Utility (`PDF_merge.py`)

## Features

-   Merge multiple PDF files
-   Supports PDF folders
-   Supports ZIP files containing PDFs
-   Natural sorting of PDF names
-   Creates automatic Table of Contents pages
-   Adds clickable TOC links
-   Creates PDF bookmarks
-   Adds author metadata
-   Generates logs

## Installation

Install dependencies:

``` bash
pip install PyMuPDF reportlab
```

## Run

``` bash
python PDF_merge.py
```

## Input Options

### Merge from Folder

Select:

    1. Folder containing PDFs

Provide:

    C:\Documents\PDFs

### Merge from ZIP

Select:

    2. ZIP containing PDFs

Provide:

    C:\Downloads\documents.zip

Output files are created inside:

    output/

------------------------------------------------------------------------

# 3. Folder Scanner (`Folders_Scan.py`)

## Purpose

Creates a list of direct subfolders for bulk image conversion.

## Run

``` bash
python Folders_Scan.py
```

Enter:

    C:\Images

Generated file:

    C:\Images\subfolders.txt

This file can be directly provided to `img2pdf.py`.

------------------------------------------------------------------------

# 4. Prefix Rename Tool (`Folders_Rename.py`)

## Purpose

Rename multiple files or folders by replacing the starting prefix.

## Run

``` bash
python Folders_Rename.py
```

Example:

Existing folders:

    OLD_Project1
    OLD_Project2
    OLD_Project3

Replace:

    OLD_

With:

    NEW_

Result:

    NEW_Project1
    NEW_Project2
    NEW_Project3

Supports:

-   Folder renaming
-   File renaming

------------------------------------------------------------------------

# Complete Installation Guide

## Requirements

-   Python 3.8 or newer
-   Windows / Linux / macOS

Check Python:

``` bash
python --version
```

## Install All Dependencies

For the complete toolkit:

``` bash
pip install Pillow reportlab PyMuPDF
```

------------------------------------------------------------------------

# Project Structure

After downloading release files:

    IMG-PDF/

    ├── img2pdf.py
    ├── PDF_merge.py
    ├── Folders_Scan.py
    ├── Folders_Rename.py
    └── output/
        ├── Generated PDFs
        └── Log Files

------------------------------------------------------------------------

# Output Folder

Both PDF utilities create:

    output/

Contains:

-   Generated PDF files
-   Conversion logs
-   Merge logs

------------------------------------------------------------------------

# Recommended Workflow

## Convert Many Images

1.  Run:

``` bash
python Folders_Scan.py
```

2.  Run:

``` bash
python img2pdf.py
```

3.  Provide:
```md
subfolders.txt
```

4.  Generate PDFs.

------------------------------------------------------------------------

## Merge Generated PDFs

1.  Run:

``` bash
python PDF_merge.py
```

2.  Select the PDF folder.

3.  Generate the final combined PDF.

------------------------------------------------------------------------

# Troubleshooting

## Module Not Found Error

Example:

    ModuleNotFoundError: No module named 'PIL'

Install dependencies again:

``` bash
pip install Pillow reportlab PyMuPDF
```

## No Images Found

Check:

-   Folder path is correct
-   Images use JPG/JPEG/PNG extensions

## No PDFs Found

Check:

-   Input folder contains `.pdf` files
-   Files are not corrupted

------------------------------------------------------------------------

# Limitations

-   Command-line interface only
-   Images are scanned only from selected folders
-   Supported image formats:
    -   JPG
    -   JPEG
    -   PNG
-   PDF pages are generated using A4 size

------------------------------------------------------------------------

# Author

### **Rachakonda Ravi**
