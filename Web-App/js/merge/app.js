/* =====================================================
   IMG & PDF Utilities

   PDF Merge Controller

   Handles:
   - PDF selection
   - ZIP PDF extraction
   - PDF validation
   - PDF ordering
   - Merge action

   ===================================================== */






let selectedPDFs = [];









document.addEventListener(
    "DOMContentLoaded",
    ()=>{


        const input =
            document.getElementById(
                "pdfInput"
            );



        const mergeButton =
            document.getElementById(
                "mergePDF"
            );



        const uploadBox =
            document.getElementById(
                "pdfUploadBox"
            );







        if(input)
        {


            input.addEventListener(
                "change",
                handlePDFs
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



                    handlePDFs(
                        {
                            target:
                            {
                                files:
                                e.dataTransfer.files
                            }
                        }
                    );


                }
            );



        }








        if(mergeButton)
        {


            mergeButton.addEventListener(
                "click",
                mergePDFs
            );


        }




    }
);













async function handlePDFs(event)
{


    const files =
        [
            ...event.target.files
        ];






    if(
        !files.length
    )
    {

        return;

    }









    selectedPDFs =
        [];








    for(
        const file of files
    )
    {



        if(
            file.name
            .toLowerCase()
            .endsWith(
                ".zip"
            )
        )
        {


            const pdfFiles =
                await readZipPDFs(
                    file
                );



            selectedPDFs.push(
                ...pdfFiles
            );



        }

        else

        {


            if(
                validatePDF(
                    file
                )
            )
            {


                selectedPDFs.push(
                    file
                );


            }


        }



    }







    selectedPDFs =
        sortPDFs(
            selectedPDFs
        );







    showPDFList();







    App.toast(
        selectedPDFs.length +
        " PDF files loaded"
    );



}












function showPDFList()
{


    const area =
        document.getElementById(
            "pdfArea"
        );



    const list =
        document.getElementById(
            "pdfList"
        );






    if(
        !list
    )
    {

        return;

    }







    list.innerHTML="";









    selectedPDFs.forEach(
        (pdf,index)=>
        {


            const item =
                document.createElement(
                    "div"
                );





            item.className =
                "pdf-item";






            item.dataset.index =
                index;







            item.innerHTML =
            `

            <span>
            ${index+1}
            </span>


            📄 ${pdf.name}


            `;






            list.appendChild(
                item
            );




        }

    );







    if(area)
    {


        area.style.display =
            "block";


    }






    enablePDFSorting();


}














async function mergePDFs()
{


    const button =
        document.getElementById(
            "mergePDF"
        );







    if(
        !selectedPDFs.length
    )
    {


        App.toast(
            "Select PDF files first",
            "error"
        );


        return;


    }








    App.loading(
        button,
        true,
        "Merging..."
    );








    try
    {



        const name =
            document.getElementById(
                "outputName"
            )
            .value
            ||
            "Merged_Document";






        const author =
            document.getElementById(
                "authorName"
            )
            .value
            ||
            "";








        const pdf =
            await mergePDFDocuments(
                selectedPDFs,
                {
                    name,
                    author
                }
            );







        downloadMergedPDF(
            pdf,
            name + ".pdf"
        );







        App.toast(
            "PDF merged successfully"
        );



    }


    catch(error)
    {


        console.error(
            error
        );



        App.toast(
            "PDF merge failed",
            "error"
        );


    }







    App.loading(
        button,
        false
    );



}