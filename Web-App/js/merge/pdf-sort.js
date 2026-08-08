/* =====================================================
   IMG & PDF Utilities

   PDF Sorting Controller

   Handles:
   - Natural filename sorting
   - Drag & drop PDF ordering
   - Manual PDF movement
   - Final merge sequence

   ===================================================== */







let pdfOrder = [];









/*
    Natural PDF sorting


    Example:

    File1.pdf
    File2.pdf
    File10.pdf


    Correct order:

    File1.pdf
    File2.pdf
    File10.pdf

*/



function sortPDFs(files)
{


    if(
        !Array.isArray(files)
    )
    {

        return [];

    }







    return files.sort(
        (a,b)=>
        naturalPDFCompare(
            a.name,
            b.name
        )
    );


}









function naturalPDFCompare(a,b)
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









/*
    Enable drag ordering

    Uses Sortable.js

*/


function enablePDFSorting()
{


    const container =
        document.getElementById(
            "pdfList"
        );






    if(
        !container
        ||
        typeof Sortable === "undefined"
    )
    {

        return;

    }








    new Sortable(
        container,
        {


            animation:200,



            ghostClass:
            "pdf-dragging",





            onEnd:
            ()=>{


                updatePDFOrder();


            }



        }

    );


}













function updatePDFOrder()
{


    const container =
        document.getElementById(
            "pdfList"
        );






    if(
        !container
    )
    {

        return;

    }







    const items =
        [
            ...container.children
        ];







    const newOrder =
        items.map(
            item =>
            item.dataset.index
        );








    pdfOrder =
        newOrder.map(
            index =>
            selectedPDFs[index]
        );








    if(
        pdfOrder.length
    )
    {


        selectedPDFs =
            pdfOrder;


    }




}













/*
    Move PDF manually


    Example:

    movePDF(0,3)

*/


function movePDF(
    from,
    to
)
{


    if(
        from < 0 ||
        to < 0 ||
        from >= selectedPDFs.length ||
        to >= selectedPDFs.length
    )
    {

        return;

    }









    const pdf =
        selectedPDFs.splice(
            from,
            1
        )[0];







    selectedPDFs.splice(
        to,
        0,
        pdf
    );








    showPDFList();


}













function reversePDFs()
{


    selectedPDFs.reverse();



    showPDFList();


}













function resetPDFOrder()
{


    pdfOrder=[];


}













function getSortedPDFs()
{


    return selectedPDFs;


}