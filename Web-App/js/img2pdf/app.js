/* =====================================================
   IMG & PDF Utilities

   Image To PDF Controller

   ===================================================== */



let selectedImages = [];





document.addEventListener(
    "DOMContentLoaded",
    ()=>{


        const input =
            document.getElementById(
                "imageInput"
            );



        const createButton =
            document.getElementById(
                "createPDF"
            );



        const uploadBox =
            document.getElementById(
                "uploadBox"
            );





        if(input)
        {


            input.addEventListener(
                "change",
                handleFiles
            );


        }







        if(uploadBox)
        {


            uploadBox.addEventListener(
                "dragover",
                e=>{


                    e.preventDefault();


                    uploadBox.classList.add(
                        "dragging"
                    );


                }
            );





            uploadBox.addEventListener(
                "dragleave",
                ()=>{


                    uploadBox.classList.remove(
                        "dragging"
                    );


                }
            );







            uploadBox.addEventListener(
                "drop",
                e=>{


                    e.preventDefault();


                    uploadBox.classList.remove(
                        "dragging"
                    );


                    handleFiles(
                        {
                            target:
                            {
                                files:e.dataTransfer.files
                            }
                        }
                    );


                }
            );


        }








        if(createButton)
        {


            createButton.addEventListener(
                "click",
                createPDF
            );


        }





    }
);









async function handleFiles(event)
{


    const files =
        [...event.target.files];




    if(!files.length)
        return;







    selectedImages =
        [];





    for(
        const file of files
    )
    {


        if(
            file.name.toLowerCase().endsWith(".zip")
        )
        {


            const images =
                await readZipImages(
                    file
                );


            selectedImages.push(
                ...images
            );


        }

        else

        {


            if(
                validateImage(file)
            )
            {


                selectedImages.push(
                    file
                );


            }


        }


    }







    selectedImages =
        sortImages(
            selectedImages
        );





    showImages();




    App.toast(
        selectedImages.length +
        " images loaded"
    );



}









function showImages()
{


    const area =
        document.getElementById(
            "detectedArea"
        );



    const list =
        document.getElementById(
            "imageList"
        );



    if(!list)
        return;





    list.innerHTML="";







    selectedImages.forEach(
        (image,index)=>{


            const item =
                document.createElement(
                    "div"
                );



            item.className =
                "image-item";



            item.innerHTML =
            `
            <span>
            ${index+1}
            </span>

            ${image.name}

            `;



            list.appendChild(
                item
            );



        }
    );





    if(area)
        area.style.display="block";




}









async function createPDF()
{


    const button =
        document.getElementById(
            "createPDF"
        );




    if(
        !selectedImages.length
    )
    {


        App.toast(
            "Select images first",
            "error"
        );


        return;

    }






    App.loading(
        button,
        true,
        "Creating PDF..."
    );







    try{


        const name =
            document.getElementById(
                "pdfName"
            ).value
            ||
            "Images";




        const author =
            document.getElementById(
                "authorName"
            ).value
            ||
            "";






        const pdf =
            await generatePDF(
                selectedImages,
                {
                    name,
                    author
                }
            );






        downloadPDF(
            pdf,
            name + ".pdf"
        );




        App.toast(
            "PDF created successfully"
        );



    }


    catch(error)
    {


        console.error(error);



        App.toast(
            "PDF creation failed",
            "error"
        );


    }





    App.loading(
        button,
        false
    );



}