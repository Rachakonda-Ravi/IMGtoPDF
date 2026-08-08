/* =====================================================
   IMG & PDF Utilities

   PDF Merger Engine

   Handles:
   - Combining multiple PDFs
   - Page copying
   - Metadata
   - Merge options

   Requires:
   PDFLib

   ===================================================== */







async function mergePDFDocuments(
    pdfFiles,
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
        !pdfFiles ||
        !pdfFiles.length
    )
    {


        throw new Error(
            "No PDF files selected"
        );


    }








    const {


        name="Merged_Document",


        author=""


    } = options;








    const mergedPDF =
        await PDFLib.PDFDocument.create();








    /*
        Metadata
    */


    mergedPDF.setTitle(
        name
    );






    if(
        author
    )
    {


        mergedPDF.setAuthor(
            author
        );


    }






    mergedPDF.setCreator(
        "IMG & PDF Utilities"
    );





    mergedPDF.setProducer(
        "IMG & PDF Utilities"
    );









    for(
        const pdfFile of pdfFiles
    )
    {


        await appendPDF(
            mergedPDF,
            pdfFile
        );


    }








    const bytes =
        await mergedPDF.save();







    return new Blob(
        [
            bytes
        ],
        {

            type:
            "application/pdf"

        }
    );



}













async function appendPDF(
    targetPDF,
    sourceFile
)
{


    const sourceBytes =
        await sourceFile.arrayBuffer();








    const sourcePDF =
        await PDFLib.PDFDocument.load(
            sourceBytes
        );








    const pages =
        await targetPDF.copyPages(
            sourcePDF,
            sourcePDF.getPageIndices()
        );








    pages.forEach(
        page =>
        {


            targetPDF.addPage(
                page
            );


        }

    );


}













async function getPDFPageCount(
    file
)
{


    const bytes =
        await file.arrayBuffer();






    const pdf =
        await PDFLib.PDFDocument.load(
            bytes
        );






    return pdf.getPageCount();


}













async function getTotalPages(
    pdfFiles
)
{


    let total = 0;







    for(
        const file of pdfFiles
    )
    {


        total +=
            await getPDFPageCount(
                file
            );


    }







    return total;


}













/*
    Preview merge information

*/


async function getMergeInfo(
    pdfFiles
)
{


    return {


        files:
        pdfFiles.length,


        pages:
        await getTotalPages(
            pdfFiles
        ),


        size:
        pdfFiles.reduce(
            (
                total,
                file
            )=>
            total + file.size,

            0

        )


    };


}