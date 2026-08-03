# IMGtoPDF

A lightweight, cross-platform **command-line Image to PDF converter** written in **C** using the **LibHaru PDF Library**.

## Features

### Default Mode

```bash
./image2pdf
```

- Automatically detects `.jpg`, `.jpeg`, and `.png`
- Alphabetical image listing
- Image count and total size
- Confirmation before PDF creation
- Duplicate PDF detection
- Automatic conversion logs
- Error log generation (only if required)
- Final conversion summary

### Advanced Mode

```bash
./image2pdf Custom
```

or

```bash
./image2pdf "D:\Photos"
```

Additional features:

- PDF Metadata (Title, Author, Subject, Keywords)
- Fit / Fill page layout
- Image sorting (Name/Size Ascending & Descending)
- Automatically open generated PDF
- Table of Contents generation
- Split PDFs by subfolder

---

# Command Line Interface

| Command | Description |
|---------|-------------|
| `image2pdf` | Default mode using current directory |
| `image2pdf -o Album.pdf` | Default mode without prompts. Creates `Album.pdf`; exits if it already exists. |
| `image2pdf "D:\Photos"` | Advanced mode using specified directory |
| `image2pdf "D:\Photos" -o Album.pdf` | Advanced mode with specified output name |
| `image2pdf "D:\Photos" --split` | Creates one PDF for each immediate subfolder |
| `image2pdf "D:\Photos" --toc` | Creates one PDF with section titles and a table of contents |

## Planned CLI Options

### `-o <filename>`
Creates the PDF without interactive prompts. If the output file already exists, the program reports the conflict and exits without overwriting.

### `--split`
Scans the immediate subfolders of the source directory and generates one PDF per subfolder.

### `--toc`
Creates a single PDF. Every immediate subfolder becomes a titled section and an automatically generated table of contents.

## Example Folder

```text
Photos/
├── Beach/
├── Mountains/
└── Food/
```

`--toc` produces one PDF with a Contents page and section titles.

`--split` produces:

```text
Beach.pdf
Mountains.pdf
Food.pdf
```

## Console Preview

```text
========================================
            IMGtoPDF v2.1
========================================

Scanning directory...

Found Images : 128
Total Size   : 846 MB

Converting...
Page 58 / 128
███████████████████░░░░ 78%

Conversion Completed Successfully
```

## Requirements

- GCC
- LibHaru
- libpng

## Build

Linux

```bash
gcc image2pdf.c -o image2pdf -lhpdf -lpng
```

Windows (MinGW)

```bash
gcc image2pdf.c -o image2pdf.exe -lhpdf -lpng
```

## Logs

Successful conversions generate a log file. Failed image loads additionally generate an error log.

## Roadmap

- Recursive directory scanning
- PDF bookmarks
- Password-protected PDFs
- EXIF auto-rotation
- OCR searchable PDFs
- Contact sheets
- GUI version

## Author

**Rachakonda Ravi**
