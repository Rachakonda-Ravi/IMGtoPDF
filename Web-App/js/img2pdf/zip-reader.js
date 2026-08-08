/* =====================================================
   IMG & PDF Utilities

   ZIP Image Reader

   Reads image files from ZIP folders

   Requires:
   JSZip

   ===================================================== */






async function readZipImages(zipFile)
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







    const images = [];








    const imageExtensions = [

        ".jpg",

        ".jpeg",

        ".png",

        ".webp",

        ".bmp"

    ];









    const files =
        Object.values(
            zip.files
        );








    for(
        const entry of files
    )
    {



        if(
            entry.dir
        )
        {
            continue;
        }






        const filename =
            entry.name.toLowerCase();







        const isImage =
            imageExtensions.some(
                ext =>
                filename.endsWith(ext)
            );







        if(
            !isImage
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






            const imageFile =
                new File(
                    [
                        blob
                    ],
                    getFileName(
                        entry.name
                    ),
                    {
                        type:
                        blob.type
                        ||
                        getMimeType(
                            filename
                        )
                    }
                );







            images.push(
                imageFile
            );



        }


        catch(error)
        {


            console.error(
                "ZIP image error:",
                entry.name,
                error
            );


        }




    }








    return images;


}













function getFileName(path)
{


    return path
        .split("/")
        .pop();


}









function getMimeType(filename)
{


    if(
        filename.endsWith(".png")
    )
    {

        return "image/png";

    }





    if(
        filename.endsWith(".webp")
    )
    {

        return "image/webp";

    }






    if(
        filename.endsWith(".bmp")
    )
    {

        return "image/bmp";

    }






    return "image/jpeg";


}













/*
    Sort ZIP images naturally

    Example:

    page1.jpg
    page2.jpg
    page10.jpg


    becomes:

    page1
    page2
    page10

*/



function sortZipImages(images)
{


    return images.sort(
        (a,b)=>
        naturalCompare(
            a.name,
            b.name
        )
    );


}









function naturalCompare(a,b)
{


    return a.localeCompare(
        b,
        undefined,
        {
            numeric:true,
            sensitivity:"base"
        }
    );


}