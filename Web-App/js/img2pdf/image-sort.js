/* =====================================================
   IMG & PDF Utilities

   Image Sorting Controller

   Handles:
   - Natural filename sorting
   - Drag and drop ordering
   - Page sequence management

   ===================================================== */







let imageOrder = [];









/*
    Sort images naturally

    Example:

    image1.jpg
    image2.jpg
    image10.jpg

    Correct order:
    
    image1
    image2
    image10

*/



function sortImages(images)
{


    if(
        !Array.isArray(images)
    )
    {

        return [];

    }







    return images.sort(
        (a,b)=>
        naturalSort(
            a.name,
            b.name
        )
    );


}









function naturalSort(a,b)
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
    Enable drag sorting

    Uses:
    Sortable.js

*/



function enableImageSorting()
{


    const container =
        document.getElementById(
            "imageList"
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
            "image-dragging",



            onEnd:
            function()
            {


                updateImageOrder();


            }


        }

    );



}









function updateImageOrder()
{


    const container =
        document.getElementById(
            "imageList"
        );



    if(
        !container
    )
    {

        return;

    }







    const items =
        [...container.children];







    const newOrder =
        items.map(
            item =>
            item.dataset.index
        );







    imageOrder =
        newOrder.map(
            index =>
            selectedImages[index]
        );







    if(
        imageOrder.length
    )
    {


        selectedImages =
            imageOrder;


    }



}









function resetImageOrder()
{


    imageOrder=[];


}









function moveImage(
    from,
    to
)
{


    if(
        from < 0 ||
        to < 0 ||
        from >= selectedImages.length ||
        to >= selectedImages.length
    )
    {

        return;

    }







    const item =
        selectedImages.splice(
            from,
            1
        )[0];







    selectedImages.splice(
        to,
        0,
        item
    );




    showImages();



}









function reverseImages()
{


    selectedImages.reverse();


    showImages();


}









function getSortedImages()
{


    return selectedImages;


}
