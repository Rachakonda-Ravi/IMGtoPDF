import os
import sys
import zipfile
import shutil
import tempfile
import logging
import re
import getpass
import fitz  

from pathlib import Path
from datetime import datetime
from reportlab.pdfgen import canvas
from reportlab.lib.pagesizes import A4


OUTPUT_DIR = Path("output")


# --------------------------------------------------
# Logging
# --------------------------------------------------

def setup_logger():

    OUTPUT_DIR.mkdir(exist_ok=True)

    log_name = datetime.now().strftime("%Y%m%d_%H%M%S") + ".log"
    log_path = OUTPUT_DIR / log_name

    logging.basicConfig(
        filename=log_path,
        level=logging.INFO,
        format="%(asctime)s | %(message)s"
    )

    return log_path


# --------------------------------------------------
# Find and Sort PDFs
# --------------------------------------------------

def natural_sort_key(path):

    return [
        int(x) if x.isdigit() else x.lower()
        for x in re.split(
            r'(\d+)',
            path.stem
        )
    ]


def get_pdf_files(folder):

    logging.info(
        f"Scanning folder: {folder}"
    )

    pdfs = []


    for file in Path(folder).iterdir():

        if file.suffix.lower() == ".pdf":

            pdfs.append(file)



    pdfs.sort(
        key=natural_sort_key
    )


    logging.info(
        f"PDF files found: {len(pdfs)}"
    )


    for index,pdf in enumerate(pdfs,1):

        logging.info(
            f"{index}. {pdf.name}"
        )


    return pdfs



# --------------------------------------------------
# Merge PDFs
# --------------------------------------------------

def merge_pdfs(pdf_files, output_file, author):

    logging.info(
        "Starting PDF merge"
    )


    final = fitz.open()

    pdf_entries = []

    current_page = 1



    # Merge PDFs first

    for index,pdf in enumerate(pdf_files,1):


        logging.info(
            f"Merging {pdf.name}"
        )


        doc = fitz.open(pdf)


        pdf_entries.append(
            {
                "no": index,
                "name": pdf.stem,
                "page": current_page
            }
        )


        final.insert_pdf(doc)


        current_page += doc.page_count


        doc.close()



    logging.info(
        f"Merged pages before TOC: {current_page-1}"
    )



    # Calculate TOC pages

    entries_per_page = 25


    toc_pages = (
        len(pdf_entries)
        + entries_per_page - 1
    ) // entries_per_page



    logging.info(
        f"TOC pages required: {toc_pages}"
    )



    # Insert TOC pages

    for i in range(toc_pages):

        final.insert_page(
            i,
            text=""
        )



    # Update real page numbers

    for item in pdf_entries:

        item["page"] += toc_pages



    # Create TOC pages

    for toc_index in range(toc_pages):


        page = final[toc_index]


        y = 60



        if toc_index == 0:

            page.insert_text(
                (180,y),
                output_file.stem,
                fontsize=18
            )

            y += 35


        page.insert_text(
            (220,y),
            "TABLE OF CONTENTS",
            fontsize=15
        )


        y += 35



        # Table header

        page.insert_text(
            (50,y),
            "No.",
            fontsize=11
        )


        page.insert_text(
            (100,y),
            "PDF Name",
            fontsize=11
        )


        page.insert_text(
            (470,y),
            "Page",
            fontsize=11
        )


        y += 20



        start = (
            toc_index
            * entries_per_page
        )


        end = min(
            start + entries_per_page,
            len(pdf_entries)
        )



        for item in pdf_entries[start:end]:


            page.insert_text(
                (50,y),
                str(item["no"]),
                fontsize=10
            )


            page.insert_text(
                (100,y),
                item["name"],
                fontsize=10
            )


            page.insert_text(
                (470,y),
                str(item["page"]),
                fontsize=10
            )



            # Clickable PDF name

            page.insert_link(
                {
                    "kind":fitz.LINK_GOTO,

                    "from":fitz.Rect(
                        100,
                        y-10,
                        430,
                        y+5
                    ),

                    "page":item["page"]
                }
            )


            y += 22



        logging.info(
            f"TOC page {toc_index+1}: "
            f"entries {start+1}-{end}"
        )



    # Sidebar bookmarks

    bookmarks=[]


    for item in pdf_entries:

        bookmarks.append(
            [
                1,
                item["name"],
                item["page"]
            ]
        )


    final.set_toc(
        bookmarks
    )


    final.set_metadata(
        {
            "title":output_file.stem,
            "author":author
        }
    )



    final.save(
        output_file
    )


    final.close()


    logging.info(
        "PDF merge completed successfully"
    )


# --------------------------------------------------
# ZIP handling
# --------------------------------------------------

def extract_zip(zip_path):

    temp_dir = tempfile.mkdtemp()

    with zipfile.ZipFile(zip_path) as z:

        z.extractall(temp_dir)


    return temp_dir



# --------------------------------------------------
# Author selection
# --------------------------------------------------

def get_author():

    print()

    print("Author options")

    print("1. Use username as author")
    print("2. Enter author name")


    choice = input(
        "Choose (1/2): "
    )


    if choice == "1":

        return getpass.getuser()


    else:

        return input(
            "Enter author name: "
        ).strip()



# --------------------------------------------------
# Main
# --------------------------------------------------

def main():

    log_file = setup_logger()


    print("=" * 45)
    print("        PDF MERGE UTILITY")
    print("=" * 45)


    print()

    print("1. Folder containing PDFs")
    print("2. ZIP containing PDFs")


    choice = input(
        "Enter choice (1/2): "
    )


    pdf_folder = None
    temp_extract = None



    if choice == "1":

        pdf_folder = input(
            "Enter folder path: "
        )


        output_name = input(
            "Enter new PDF name: "
        )


    elif choice == "2":


        zip_file = Path(
            input(
                "Enter ZIP file path: "
            )
        )


        temp_extract = extract_zip(
            zip_file
        )


        pdf_folder = temp_extract


        print()

        print("1. Use ZIP filename")
        print("2. Custom name")


        zip_choice = input(
            "Choose: "
        )


        if zip_choice == "1":

            output_name = zip_file.stem


        else:

            output_name = input(
                "Enter new PDF name: "
            )


    else:

        print(
            "Invalid choice"
        )

        return



    if not output_name.lower().endswith(".pdf"):

        output_name += ".pdf"



    author = get_author()



    pdf_files = get_pdf_files(
        pdf_folder
    )


    if not pdf_files:

        print(
            "No PDF files found"
        )

        return



    output_file = OUTPUT_DIR / output_name



    logging.info(
        "PDF MERGE STARTED"
    )

    logging.info(
        f"Author: {author}"
    )

    logging.info(
        f"Input Folder: {pdf_folder if choice == '1' else 'N/A'}"
    )
    logging.info(
        f"Input ZIP: {zip_file if choice == '2' else 'N/A'}"
    )

    logging.info(
        f"Output: {output_file}"
    )


    logging.info(
        f"Total PDFs: {len(pdf_files)}"
    )


    merge_pdfs(
        pdf_files,
        output_file,
        author
    )


    logging.info(
        "STATUS: SUCCESS"
    )


    if temp_extract:

        shutil.rmtree(
            temp_extract
        )



    print()

    print(
        "Completed successfully"
    )

    print(
        f"Created: {output_file}"
    )

    print(
        f"Log: {log_file}"
    )



if __name__ == "__main__":

    main()