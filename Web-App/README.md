# 📄 IMG & PDF Utilities

## Overview

IMG & PDF Utilities is a multi-platform PDF processing toolkit designed
for creating and managing PDF documents easily.

The project contains:

-   Native C implementation
-   Python implementation
-   Browser-based Web Application

The Web version performs all processing locally inside the browser.

------------------------------------------------------------------------

## Features

### 🖼 Image → PDF

Convert images into PDF documents.

Supported formats:

-   JPG / JPEG
-   PNG
-   WEBP
-   BMP

Features:

-   Multiple image selection
-   ZIP image folder support
-   Automatic image sorting
-   Drag and drop ordering
-   Custom PDF filename
-   Author metadata
-   Local browser processing

------------------------------------------------------------------------

## 📦 ZIP Folder Support

Example:

    Manga.zip

     ├── 001.jpg
     ├── 002.jpg
     └── 003.jpg

Output:

    Manga.pdf

------------------------------------------------------------------------

## 📑 PDF Merge

Combine multiple PDF files into one document.

Features:

-   Multiple PDF selection
-   ZIP PDF extraction
-   Drag and drop ordering
-   Natural filename sorting
-   Custom output name
-   Author metadata
-   Lossless PDF merging

------------------------------------------------------------------------

## 🌐 Web Application

The Web App includes:

-   Responsive interface
-   Modern UI
-   Dark theme support
-   Drag and drop workflow
-   No file upload to servers
-   Privacy-focused processing

Pages:

-   Home
-   Image → PDF
-   PDF Merge

------------------------------------------------------------------------

## Project Structure

    IMG-PDF/

    ├── C/
    ├── Python/
    └── Web-App/

        ├── index.html

        ├── pages/
        │   ├── img-to-pdf.html
        │   └── pdf-merge.html

        ├── css/
        ├── js/
        └── libs/

------------------------------------------------------------------------

## Technologies

### Web

-   HTML5
-   CSS3
-   JavaScript
-   PDF-Lib
-   JSZip
-   Sortable.js

------------------------------------------------------------------------

## Privacy

The Web version:

-   Does not upload files
-   Runs completely in the browser
-   Keeps documents on the user's device

------------------------------------------------------------------------

## Installation

Clone repository:

    git clone https://github.com/Rachakonda-Ravi/IMG-PDF.git

Run Web App:

    python -m http.server 8000

Open:

    http://localhost:8000

------------------------------------------------------------------------

## Releases

https://github.com/Rachakonda-Ravi/IMG-PDF/releases

------------------------------------------------------------------------

## Author

Rachakonda Ravi

GitHub: https://github.com/Rachakonda-Ravi

------------------------------------------------------------------------

⭐ Star the repository if you find it useful.
