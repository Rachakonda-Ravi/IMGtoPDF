# 📄 IMG-PDF — Complete Image & PDF Utility Suite

<p align="center">

**A powerful multi-platform toolkit for converting, managing, and processing images and PDF documents.**

</p>

---

# 🚀 Overview

**IMG-PDF** is a complete image and PDF processing ecosystem available in three implementations:

| Platform             | Technology              | Purpose                                 |
| -------------------- | ----------------------- | --------------------------------------- |
| Native Application   | C + libHaru             | High-performance offline PDF generation |
| Command Line Toolkit | Python                  | Advanced image/PDF automation           |
| Web Application      | HTML + CSS + JavaScript | Browser-based private PDF processing    |

The project is designed to be:

* ✅ Offline capable
* ✅ Privacy focused
* ✅ Cross-platform
* ✅ Lightweight
* ✅ Easy to use
* ✅ Open source

---

# ✨ Main Features

## 🖼 Image → PDF Conversion

Convert image collections into high-quality PDF documents.

Supported formats:

* JPG
* JPEG
* PNG
* WEBP (Web version)

Features:

* Multiple image selection
* ZIP image folder support
* Automatic image sorting
* Natural filename sorting
* Drag-and-drop ordering
* Custom PDF filename
* PDF metadata support
* Author information
* A4 page generation
* Aspect ratio preservation
* Image centering
* Quality preservation

Example:

```
Holiday_Photos.zip

├── 001.jpg
├── 002.jpg
├── 003.jpg

        ↓

Holiday_Photos.pdf
```

---

# 📑 PDF Merge

Combine multiple PDF documents into one file.

Features:

* Multiple PDF selection
* PDF folder support
* ZIP PDF extraction
* Natural filename sorting
* Lossless merging
* Automatic Table of Contents
* Clickable TOC links
* PDF bookmarks
* Author metadata

Example:

```
Documents/

├── Chapter1.pdf
├── Chapter2.pdf
├── Chapter3.pdf

        ↓

Complete_Book.pdf
```

---

# 🌐 Web Application

The browser version provides a modern interface for completely local PDF processing.

## Features

* Responsive design
* Dark theme support
* Drag-and-drop workflow
* No server upload
* Runs entirely inside browser
* Privacy-first architecture

## Pages

```
Web-App/

├── index.html

└── pages/

    ├── img-to-pdf.html
    └── pdf-merge.html
```

---

# 🔒 Privacy

The Web Application:

* Does not upload files
* Does not store user documents
* Processes everything locally
* Keeps images and PDFs on the user's device

Your files never leave your computer.

---

# 🖱 Web Image Ordering

Images are arranged using natural sorting.

Example:

Before:

```
1.jpg
10.jpg
11.jpg
2.jpg
```

After:

```
1.jpg
2.jpg
10.jpg
11.jpg
```

Users can manually reorder pages:

```
☰ Cover.jpg
☰ Page1.jpg
☰ Page2.jpg
☰ Appendix.jpg
```

The final order becomes the PDF page order.

---

# ✅ Image Validation

Before PDF generation, images are checked automatically.

Example:

```
Checking images...

✓ 001.jpg
✓ 002.jpg
✓ 003.png
✗ damaged.jpg
✓ 005.jpg
```

Invalid files are reported:

```
2 invalid image(s) found

• damaged.jpg
• broken.png
```

Users can continue without damaged files.

---

# 📦 Multi-PDF ZIP Export

When converting multiple folders:

Instead of downloading:

```
Chapter1.pdf
Chapter2.pdf
Chapter3.pdf
Chapter4.pdf
```

The Web App creates:

```
Book_PDFs.zip

├── Chapter1.pdf
├── Chapter2.pdf
├── Chapter3.pdf
└── Chapter4.pdf
```

Only one download is generated.

---

# 🖥 C Implementation

The C version provides fast native PDF generation using:

**libHaru PDF Library**

Two versions are available:

```
IMG2PDF_Base.c
IMG2PDF_Advanced.c
```

---

# ⚡ IMG2PDF Base

Designed for simple PDF creation.

Features:

* Automatic image scanning
* JPG/JPEG support
* PNG support
* Alphabetical sorting
* A4 PDF generation
* Aspect ratio preservation
* Image centering
* Simple interface

---

# 🚀 IMG2PDF Advanced

A feature-rich converter.

Additional features:

## Image Management

* Supports up to 1000 images
* Displays filename
* Displays image type
* Displays file size
* Shows total image count
* Shows total size

---

## Sorting Options

Available:

```
1. Name A → Z
2. Name Z → A
3. Size Small → Large
4. Size Large → Small
```

---

## Page Layout

Two modes:

### Fit Mode

* Entire image visible
* Maintains aspect ratio
* May contain margins

### Fill Mode

* Full page coverage
* Maintains aspect ratio
* Crops overflow

---

## PDF Metadata

Supports:

* Title
* Author
* Subject
* Keywords

---

## Logging System

Creates:

```
Logs/

├── output_YYYY-MM-DD_HH-MM-SS.log

└── output_YYYY-MM-DD_HH-MM-SS_error.log
```

Logs contain:

* Conversion time
* Output filename
* Images processed
* Failed images
* Image statistics

---

## Progress Display

Example:

```
[ 1/25 ] image01.jpg       OK
[ 2/25 ] image02.png       OK
[ 3/25 ] image03.jpg       Failed
```

---

# 🐍 Python Toolkit

The Python version provides automation tools.

Available utilities:

| Tool            | File              | Purpose                    |
| --------------- | ----------------- | -------------------------- |
| Image Converter | img2pdf.py        | Convert images into PDF    |
| PDF Merge       | PDF_Merge.py      | Merge PDFs                 |
| Folder Scanner  | Folders_Scan.py   | Generate folder lists      |
| Rename Tool     | Folders_Rename.py | Batch rename files/folders |

---

# Python Image Converter

Features:

* JPG/JPEG/PNG support
* A4 PDF generation
* Automatic scaling
* Aspect ratio preservation
* Folder batch processing
* Corrupted image detection
* Logging
* Metadata support

Install:

```bash
pip install Pillow reportlab
```

Run:

```bash
python img2pdf.py
```

---

# Python PDF Merge Utility

Features:

* Merge PDFs
* Merge from folders
* Merge from ZIP files
* Natural sorting
* TOC generation
* Bookmarks
* Metadata

Install:

```bash
pip install PyMuPDF reportlab
```

Run:

```bash
python PDF_merge.py
```

---

# Folder Scanner

Creates a list of folders for batch processing.

Run:

```bash
python Folders_Scan.py
```

Output:

```
subfolders.txt
```

---

# Folder Rename Utility

Rename files/folders using prefix replacement.

Example:

Before:

```
OLD_Project1
OLD_Project2
```

After:

```
NEW_Project1
NEW_Project2
```

Run:

```bash
python Folders_Rename.py
```

---

# 📁 Project Structure

```
IMG-PDF/

│
├── C/

│   ├── IMG2PDF_Base.c
│   └── IMG2PDF_Advanced.c
│
├── Python/

│   ├── img2pdf.py
│   ├── PDF_Merge.py
│   ├── Folders_Scan.py
│   └── Folders_Rename.py
│
├── Web-App/

│   ├── index.html
│   │
│   ├── pages/
│   │   ├── img-to-pdf.html
│   │   └── pdf-merge.html
│   │
│   ├── css/
│   ├── js/
│   └── libs/
│
└── README.md
```

---

# 🛠 Installation

## Clone Repository

```bash
git clone https://github.com/Rachakonda-Ravi/IMG-PDF.git
```

---

# Run Web Application

Start local server:

```bash
python -m http.server 8000
```

Open:

```
http://localhost:8000
```

---

# Compile C Version

Linux:

```bash
gcc IMG2PDF_Base.c -o IMG2PDF_Base -lhpdf

gcc IMG2PDF_Advanced.c -o IMG2PDF_Advanced -lhpdf
```

Windows:

```bash
gcc IMG2PDF_Base.c -o IMG2PDF_Base.exe -lhpdf

gcc IMG2PDF_Advanced.c -o IMG2PDF_Advanced.exe -lhpdf
```

---

# Python Requirements

Python:

```
Python 3.8+
```

Install:

```bash
pip install Pillow reportlab PyMuPDF
```

---

# 📚 Recommended Workflow

## Convert Large Image Collection

1. Scan folders:

```bash
python Folders_Scan.py
```

2. Convert:

```bash
python img2pdf.py
```

---

## Merge Generated PDFs

Run:

```bash
python PDF_merge.py
```

Select:

```
PDF folder
```

Generate:

```
Final_Book.pdf
```

---

# 🧩 Technologies Used

## C

* C Language
* libHaru PDF Library

## Python

* Pillow
* ReportLab
* PyMuPDF

## Web

* HTML5
* CSS3
* JavaScript
* PDF-Lib
* JSZip
* SortableJS

---

# 📌 Limitations

Current limitations:

* C version supports JPG/JPEG/PNG only
* Python version requires installed dependencies
* Web version depends on modern browser support

---

# 📦 Releases

GitHub Releases:

```
https://github.com/Rachakonda-Ravi/IMG-PDF/releases
```

---

# 👨‍💻 Author

## Rachakonda Ravi

GitHub:

```
https://github.com/Rachakonda-Ravi
```

---

# ⭐ Support

If this project is useful:

* Star the repository
* Report issues
* Suggest improvements
* Contribute features

---

**IMG&PDF Util — Convert. Merge. Organize. Privately.**
