import os
import sys
import getpass

from pathlib import Path
from datetime import datetime

from PIL import Image
from reportlab.pdfgen import canvas
from reportlab.lib.pagesizes import A4

# ==============================
# Configuration
# ==============================

BASE_DIR = Path(__file__).parent
OUTPUT_DIR = BASE_DIR / "output"

SUPPORTED_EXTENSIONS = {
    ".jpg",
    ".jpeg",
    ".png"
}

PAGE_WIDTH, PAGE_HEIGHT = A4


# ==============================
# Logging
# ==============================

class Logger:

    def __init__(self):
        OUTPUT_DIR.mkdir(exist_ok=True)

        name = datetime.now().strftime(
            "%d%m%Y_%H%M_Log.txt"
        )

        self.file = OUTPUT_DIR / name

        self.fp = open(
            self.file,
            "w",
            encoding="utf-8"
        )

    def write(self, text=""):
        print(text)
        self.fp.write(text + "\n")

    def close(self):
        self.fp.close()


# ==============================
# Image Scanner
# ==============================

def scan_images(folder):

    images = []

    folder = Path(folder)

    if not folder.exists():
        return []

    for item in folder.iterdir():

        if (
            item.is_file()
            and item.suffix.lower()
            in SUPPORTED_EXTENSIONS
        ):
            images.append(item)

    return sorted(images)



# ==============================
# Image Validation
# ==============================

def check_images(images, logger):

    valid = []
    invalid = []
    print("\nChecking images...\n")

    logger.write("\nChecking images...\n")

    for index, img_path in enumerate(images, 1):

        try:

            with Image.open(img_path) as img:
                img.verify()

            valid.append(img_path)

            logger.write(
                f"{index:04d}. {img_path.name}  OK"
            )

        except Exception:

            invalid.append(img_path)

            logger.write(
                f"{index:04d}. {img_path.name}  ERROR"
            )


    return valid, invalid



# ==============================
# PDF Creation
# ==============================

def create_pdf(images, output_file, logger, author=None):

    pdf = canvas.Canvas(
        str(output_file),
        pagesize=A4
    )
    if author:
        pdf.setAuthor(author)


    processed = 0


    for img_path in images:

        try:

            img = Image.open(img_path)

            img_width, img_height = img.size


            # Maintain aspect ratio

            scale = min(
                PAGE_WIDTH / img_width,
                PAGE_HEIGHT / img_height
            )


            width = img_width * scale
            height = img_height * scale


            x = (
                PAGE_WIDTH - width
            ) / 2


            y = (
                PAGE_HEIGHT - height
            ) / 2


            pdf.drawImage(
                str(img_path),
                x,
                y,
                width,
                height
            )


            pdf.showPage()

            processed += 1


        except Exception as e:

            logger.write(
                f"PDF Error {img_path.name}: {e}"
            )


    pdf.save()


    return processed



# ==============================
# Folder Processing
# ==============================

def process_folder(folder, logger, filename_mode=None, author=None):

    folder = Path(folder)


    logger.write("\n")
    logger.write("=" * 50)
    logger.write(
        f"Folder : {folder}"
    )


    if not folder.exists():

        logger.write(
            "ERROR : Folder does not exist"
        )

        return



    images = scan_images(folder)


    logger.write(
        f"Images Found : {len(images)}"
    )


    if len(images) == 0:

        logger.write(
            "No images found"
        )

        return



    valid, invalid = check_images(
        images,
        logger
    )


    if invalid:

        print("\nInvalid Images:")

        for img in invalid:
            print(
                " -",
                img.name
            )


        choice = input(
            "\nContinue without invalid images? (Y/N): "
        )


        if choice.lower() != "y":

            logger.write(
                "PDF creation cancelled by user"
            )

            return
    
    if filename_mode == "folder":
        filename = folder.name
    else:
        filename = filename_mode


    if not filename.lower().endswith(".pdf"):
        filename += ".pdf"

    output_file = OUTPUT_DIR / filename

    processed = create_pdf(
        valid,
        output_file,
        logger,
        author
    )


    logger.write("\nPDF CREATED")
    logger.write(
        f"Output : {output_file}"
    )

    logger.write(
        f"Processed : {processed}"
    )

    logger.write(
        f"Skipped : {len(invalid)}"
    )



# ==============================
# Main
# ==============================

def main():

    logger = Logger()


    logger.write(
        "Image To PDF Converter"
    )

    logger.write(
        datetime.now().strftime(
            "%d-%m-%Y %H:%M:%S"
        )
    )


    print(
        "\nEnter folder path(s)"
    )

    print(
        "Separate multiple folders using comma"
    )


    folders = input("\nFolder(s) or subfolders.txt path: "
                ).strip().strip('"')


    # If input is a txt file, read folders from it
    if folders.lower().endswith(".txt") and os.path.isfile(folders):

        with open(folders, "r", encoding="utf-8") as f:
            folders = f.read()


    folder_list = [
        x.strip()
        for x in folders.split(",")
        if x.strip()
    ]

    username = getpass.getuser()

    print("\nPDF Creation Settings")
    print("====================")
    print("1. Automatic PDF Naming")
    print("2. Custom PDF Name")
    print("3. Custom PDF Name & Author")
    print("4. Cancel")


    while True:

        choice = input("\nSelect option (1-4): ").strip()

        if choice == "1":
            filename_mode = "folder"
            author = username
            break

        elif choice == "2":
            filename_mode = input("\nEnter PDF name: ").strip()
            author = username
            break

        elif choice == "3":
            filename_mode = input("\nEnter PDF name: ").strip()
            author = input("Enter author name: ").strip()
            break

        elif choice == "4":
            logger.close()
            return

        else:
            print("Invalid choice")

        if len(folder_list) == 0:

            print(
                "No folders entered"
            )

            logger.close()
            return



    for folder in folder_list:

        process_folder(
                folder,
                logger,
                filename_mode,
                author
            )


    logger.write("\nFinished")

    logger.close()


    print(
        "\nDone."
    )


if __name__ == "__main__":

    main()