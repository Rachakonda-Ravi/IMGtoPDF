/* =====================================================
   IMG-PDF
   Library Loader

   Checks required libraries
   and starts application modules

   ===================================================== */





const AppLoader = {



    libraries:
    {

        pdf:
            typeof PDFLib !== "undefined",


        zip:
            typeof JSZip !== "undefined",


        sortable:
            typeof Sortable !== "undefined"


    },









    check(){



        const missing = [];





        if(
            !this.libraries.pdf
        )
        {

            missing.push(
                "pdf-lib"
            );

        }







        if(
            !this.libraries.zip
        )
        {

            missing.push(
                "JSZip"
            );

        }







        if(
            !this.libraries.sortable
        )
        {

            missing.push(
                "SortableJS"
            );

        }







        if(
            missing.length > 0
        )
        {


            console.warn(
                "Missing libraries:",
                missing
            );


            return false;


        }







        console.log(
            "All libraries loaded"
        );



        return true;



    },









    start(){



        this.check();





        /*
            Modules initialize
            themselves through
            DOMContentLoaded

        */



        console.log(
            "IMG-PDF ready"
        );



    }



};








document.addEventListener(
    "DOMContentLoaded",
    ()=>{


        AppLoader.start();



    }
);