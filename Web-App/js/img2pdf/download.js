/* =====================================================
   IMG & PDF Utilities

   Download Handler

   Handles:
   - PDF downloads
   - Safe filenames
   - Browser download trigger

   ===================================================== */







function downloadPDF(
    pdfBlob,
    filename="document.pdf"
)
{


    if(
        !pdfBlob
    )
    {


        App.toast(
            "No PDF available",
            "error"
        );


        return;


    }








    const safeName =
        sanitizeFilename(
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









function sanitizeFilename(
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









function downloadBlob(
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
        sanitizeFilename(
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









function getDownloadName(
    name,
    extension=".pdf"
)
{


    let filename =
        name
        ||
        "IMG-PDF";






    filename =
        sanitizeFilename(
            filename
        );







    if(
        !filename.toLowerCase()
        .endsWith(
            extension
        )
    )
    {


        filename += extension;


    }






    return filename;


}