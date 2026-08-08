/* =====================================================
   IMG & PDF Utilities

   PDF Merge Download Handler

   Handles:
   - Download merged PDF
   - Safe filenames
   - Browser memory cleanup

   ===================================================== */







function downloadMergedPDF(
    pdfBlob,
    filename="Merged_Document.pdf"
)
{


    if(
        !pdfBlob
    )
    {


        App.toast(
            "No merged PDF available",
            "error"
        );


        return;


    }








    const safeName =
        sanitizeMergeFilename(
            filename
        );








    const url =
        URL.createObjectURL(
            pdfBlob
        );








    const link =
        document.createElement(
            "a"
        );








    link.href =
        url;






    link.download =
        safeName.endsWith(
            ".pdf"
        )
        ?
        safeName
        :
        safeName + ".pdf";








    document.body.appendChild(
        link
    );








    link.click();








    document.body.removeChild(
        link
    );








    setTimeout(
        ()=>{


            URL.revokeObjectURL(
                url
            );


        },

        1000

    );







}













function sanitizeMergeFilename(
    filename
)
{


    return filename

        .replace(
            /[<>:"\/\\|?*]/g,
            "_"
        )

        .trim();


}













function downloadMergeBlob(
    blob,
    filename
)
{


    if(
        !blob
    )
    {

        return;

    }








    const url =
        URL.createObjectURL(
            blob
        );







    const link =
        document.createElement(
            "a"
        );






    link.href =
        url;






    link.download =
        sanitizeMergeFilename(
            filename
        );







    document.body.appendChild(
        link
    );






    link.click();







    link.remove();








    setTimeout(
        ()=>{


            URL.revokeObjectURL(
                url
            );


        },

        1000

    );



}













function getMergedFilename(
    name
)
{


    let filename =
        name
        ||
        "Merged_Document";







    filename =
        sanitizeMergeFilename(
            filename
        );







    if(
        !filename
        .toLowerCase()
        .endsWith(
            ".pdf"
        )
    )
    {


        filename += ".pdf";


    }






    return filename;


}