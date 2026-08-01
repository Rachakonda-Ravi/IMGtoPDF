# IMGtoPDF

A lightweight, cross-platform command-line Image to PDF converter written in **C** using the **LibHaru PDF Library**.

## Features

### Default Mode

- Automatic detection of `.jpg`, `.jpeg`, and `.png` images
- Alphabetical image listing
- Image count and total size
- Confirmation before PDF creation
- Duplicate PDF detection
- Automatic log generation
- Error log generation (only if required)
- Final conversion report

### Advanced Mode

Run:

```bash
image2pdf Custom
```

Advanced mode provides:

- PDF Metadata
  - Title
  - Author
  - Subject
  - Keywords
- Page Layout
  - Fit (Maintain Aspect Ratio)
  - Fill (Crop to Page)
- Image Sorting
  - Name (A → Z)
  - Name (Z → A)
  - Size (Small → Large)
  - Size (Large → Small)
- Automatically open the generated PDF

---

## Supported Formats

- JPG
- JPEG
- PNG

---

## Requirements

- GCC
- LibHaru
- libpng

---

## Compilation

### Linux

```bash
gcc image2pdf.c -o image2pdf -lhpdf -lpng
```

### Windows (MinGW)

```bash
gcc image2pdf.c -o image2pdf.exe -lhpdf -lpng
```

---

## Usage

### Default Mode

```bash
./image2pdf
```

### Advanced Mode

```bash
./image2pdf Custom
```

---

## Repository Structure

```text
IMGtoPDF/
├── image2pdf.c
├── README.md
├── LICENSE
└── .gitignore
```

> The `Logs` folder is automatically created when the program runs.

---

## Logs

Every successful conversion creates a log file.

If any image fails to load, an additional error log is generated.

Example:

```text
Logs/
    Album_2026-08-01_20-15-42.log
    Album_2026-08-01_20-15-42_error.log
```

---

## Planned Features

- JPEG / PNG support
- Automatic image detection
- PDF Metadata
- Fit / Fill mode
- Image sorting
- Progress indicator
- Detailed summary report
- Automatic log generation
- Error logging
- Cross-platform support

---

## Author

**Rachakonda Ravi**

---

## Version

Current Version: **2.0**

---

## License

See the **LICENSE** file for licensing information.