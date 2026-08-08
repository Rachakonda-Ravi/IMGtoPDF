/* =====================================================
   IMG & PDF Utilities

   PDF Generator

   Creates PDF from images using pdf-lib

   Features:
   - Multiple images
   - Auto page sizing
   - JPEG / PNG support
   - Author metadata
   - Title metadata

   Requires:
   PDFLib

   ===================================================== */







async function generatePDF(
    images,
    options = {}
)
{


    if(
        typeof PDFLib === "undefined"
    )
    {


        throw new Error(
            "PDF library not loaded"
        );


    }








    if(
        !images ||
        !images.length
    )
    {


        throw new Error(
            "No images provided"
        );


    }








    const {

        name="Images",

        author=""

    } = options;








    const pdfDoc =
        await PDFLib.PDFDocument.create();







    /*
        Metadata
    */


    pdfDoc.setTitle(
        name
    );



    if(
        author
    )
    {


        pdfDoc.setAuthor(
            author
        );


    }






    pdfDoc.setCreator(
        "IMG & PDF Utilities"
    );



    pdfDoc.setProducer(
        "IMG & PDF Utilities"
    );









    for(
        const imageFile of images
    )
    {


        await addImagePage(
            pdfDoc,
            imageFile
        );


    }







    const pdfBytes =
        await pdfDoc.save();







    return new Blob(
        [
            pdfBytes
        ],
        {
            type:
            "application/pdf"
        }
    );



}














async function addImagePage(
    pdfDoc,
    imageFile
)
{


    const bytes =
        await imageFile.arrayBuffer();






    let image;






    const type =
        imageFile.type
        ||
        getMimeType(
            imageFile.name
        );









    if(
        type === "image/png"
    )
    {


        image =
            await pdfDoc.embedPng(
                bytes
            );


    }

    else

    {


        image =
            await pdfDoc.embedJpg(
                bytes
            );


    }









    const width =
        image.width;





    const height =
        image.height;








    /*
        Keep original aspect ratio

    */


    const page =
        pdfDoc.addPage(
            [
                width,
                height
            ]
        );









    page.drawImage(
        image,
        {

            x:0,

            y:0,

            width,

            height

        }
    );




}














function resizeImageToPage(
    imageWidth,
    imageHeight,
    pageWidth,
    pageHeight
)
{


    const ratio =
        Math.min(

            pageWidth /
            imageWidth,

            pageHeight /
            imageHeight

        );







    return {


        width:
        imageWidth *
        ratio,



        height:
        imageHeight *
        ratio


    };


}









async function generatePDFPreview(
    images
)
{


    const blob =
        await generatePDF(
            images
        );




    return URL.createObjectURL(
        blob
    );


}









function getPDFPageCount(
    images
)
{


    return images.length;


}