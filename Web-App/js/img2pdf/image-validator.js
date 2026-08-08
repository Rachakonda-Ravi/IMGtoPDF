/* =====================================================
   IMG & PDF Utilities

   Image Validator

   Validates uploaded images before PDF creation

   ===================================================== */






const IMAGE_RULES = {


    /*
        Supported formats
    */

    extensions:[

        "jpg",

        "jpeg",

        "png",

        "webp",

        "bmp"

    ],






    /*
        Maximum file size

        50 MB per image

    */

    maxSize:

    50 * 1024 * 1024



};









function validateImage(file)
{


    if(
        !file
    )
    {

        return false;

    }







    if(
        !isImageExtension(
            file.name
        )
    )
    {


        App.toast(
            `${file.name} is not a supported image`,
            "error"
        );


        return false;


    }







    if(
        file.size >
        IMAGE_RULES.maxSize
    )
    {


        App.toast(
            `${file.name} is too large`,
            "error"
        );


        return false;


    }







    if(
        file.type &&
        !file.type.startsWith(
            "image/"
        )
    )
    {


        App.toast(
            `${file.name} is not an image`,
            "error"
        );


        return false;


    }






    return true;


}









function isImageExtension(filename)
{


    const name =
        filename
        .toLowerCase();






    return IMAGE_RULES.extensions.some(
        ext =>
        name.endsWith(
            "." + ext
        )
    );


}









async function validateImageContent(file)
{


    return new Promise(
        resolve=>{


            const img =
                new Image();





            const url =
                URL.createObjectURL(
                    file
                );






            img.onload =
            ()=>{


                URL.revokeObjectURL(
                    url
                );



                resolve(
                    true
                );


            };








            img.onerror =
            ()=>{


                URL.revokeObjectURL(
                    url
                );



                App.toast(
                    `${file.name} is corrupted`,
                    "error"
                );



                resolve(
                    false
                );


            };






            img.src =
                url;



        }
    );


}









async function validateImages(files)
{


    const validImages=[];







    for(
        const file of files
    )
    {



        if(
            validateImage(
                file
            )
        )
        {


            const valid =
                await validateImageContent(
                    file
                );



            if(valid)
            {


                validImages.push(
                    file
                );


            }



        }



    }






    return validImages;


}








function getImageInfo(file)
{


    return {


        name:file.name,


        size:file.size,


        type:file.type,


        sizeText:
            formatFileSize(
                file.size
            )


    };


}









function formatFileSize(bytes)
{


    if(
        bytes === 0
    )
    {

        return "0 Bytes";

    }





    const units=[

        "Bytes",

        "KB",

        "MB",

        "GB"

    ];





    const index =
        Math.floor(
            Math.log(bytes)
            /
            Math.log(1024)
        );






    return (

        Math.round(
            bytes /
            Math.pow(
                1024,
                index
            )
        )

        +

        " "

        +

        units[index]

    );


}