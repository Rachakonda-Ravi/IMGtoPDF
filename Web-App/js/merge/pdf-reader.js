/* =====================================================
   IMG & PDF Utilities

   PDF Reader / Validator

   Handles:
   - PDF validation
   - ZIP PDF extraction
   - PDF information reading

   Requires:
   - JSZip
   - PDFLib

   ===================================================== */







const PDF_RULES = {


    extensions:[

        ".pdf"

    ],





    maxSize:

    100 * 1024 * 1024


};









/*
    Validate PDF file
*/


function validatePDF(file)
{


    if(
        !file
    )
    {

        return false;

    }







    const filename =
        file.name
        .toLowerCase();








    if(
        !filename.endsWith(
            ".pdf"
        )
    )
    {


        App.toast(
            `${file.name} is not a PDF`,
            "error"
        );


        return false;


    }









    if(
        file.size >
        PDF_RULES.maxSize
    )
    {


        App.toast(
            `${file.name} is too large`,
            "error"
        );


        return false;


    }







    return true;


}













/*
    Read PDF metadata

*/


async function readPDFInfo(file)
{


    if(
        typeof PDFLib === "undefined"
    )
    {


        return null;


    }






    try
    {


        const bytes =
            await file.arrayBuffer();





        const pdf =
            await PDFLib.PDFDocument.load(
                bytes
            );








        return {


            name:file.name,


            pages:
            pdf.getPageCount(),



            size:
            formatPDFSize(
                file.size
            )



        };



    }


    catch(error)
    {


        console.error(
            "PDF read error:",
            error
        );



        return null;


    }


}













/*
    Extract PDF files from ZIP

*/


async function readZipPDFs(zipFile)
{


    if(
        typeof JSZip === "undefined"
    )
    {


        throw new Error(
            "ZIP library not loaded"
        );


    }







    const zip =
        await JSZip.loadAsync(
            zipFile
        );







    const pdfFiles=[];









    for(
        const entry of
        Object.values(
            zip.files
        )
    )
    {


        if(
            entry.dir
        )
        {

            continue;

        }






        const name =
            entry.name.toLowerCase();







        if(
            !name.endsWith(
                ".pdf"
            )
        )
        {

            continue;

        }









        try
        {


            const blob =
                await entry.async(
                    "blob"
                );






            const pdf =
                new File(
                    [
                        blob
                    ],
                    getPDFName(
                        entry.name
                    ),
                    {
                        type:
                        "application/pdf"
                    }
                );







            if(
                validatePDF(
                    pdf
                )
            )
            {


                pdfFiles.push(
                    pdf
                );


            }




        }


        catch(error)
        {


            console.error(
                "ZIP PDF error:",
                entry.name,
                error
            );


        }




    }








    return sortPDFs(
        pdfFiles
    );


}












function getPDFName(path)
{


    return path
        .split("/")
        .pop();


}









function formatPDFSize(bytes)
{


    if(
        bytes < 1024
    )
    {


        return bytes +
        " Bytes";


    }







    if(
        bytes < 1024 * 1024
    )
    {


        return (
            bytes /
            1024
        )
        .toFixed(2)
        +
        " KB";


    }







    return (

        bytes /
        (
            1024 *
            1024
        )

    )
    .toFixed(2)
    +
    " MB";


}