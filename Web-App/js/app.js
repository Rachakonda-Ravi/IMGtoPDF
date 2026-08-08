/* =====================================================
   IMG & PDF Utilities
   Global Application Controller

   Handles:

   - Common UI functions
   - Navigation effects
   - Toast notifications
   - Loading states
   - Theme helpers

   ===================================================== */






const App = {






    /*
        Initialize application
    */


    init(){


        this.setupNavigation();


        this.setupAnimations();


        console.log(
            "IMG & PDF Utilities Loaded"
        );


    },









    /*
        Smooth navigation handling
    */


    setupNavigation(){



        const links =
            document.querySelectorAll(
                "a"
            );





        links.forEach(
            link=>{


                link.addEventListener(
                    "click",
                    ()=>{


                        link.classList.add(
                            "clicked"
                        );


                    }
                );


            }
        );



    },









    /*
        Add reveal animations
    */


    setupAnimations(){



        const cards =
            document.querySelectorAll(
                ".tool-card, .info-card, .feature-box, .converter-card, .merge-card"
            );







        cards.forEach(
            (card,index)=>{


                card.style.opacity="0";


                card.style.transform=
                    "translateY(25px)";



                setTimeout(
                    ()=>{


                        card.style.transition=
                        "all .5s ease";


                        card.style.opacity="1";


                        card.style.transform=
                        "translateY(0)";


                    },

                    index * 80

                );


            }
        );



    },









    /*
        Toast notification


        Usage:


        App.toast(
            "PDF Created"
        );

    */


    toast(
        message,
        type="success"
    ){



        let container =
            document.querySelector(
                ".toast-container"
            );





        if(
            !container
        )
        {


            container =
                document.createElement(
                    "div"
                );


            container.className =
                "toast-container";


            document.body.appendChild(
                container
            );


        }







        const toast =
            document.createElement(
                "div"
            );





        toast.className =
            "toast " + type;





        toast.innerHTML =
        `
            <span>
            ${type==="success" ? "✓" : "!"}
            </span>

            ${message}
        `;







        container.appendChild(
            toast
        );








        setTimeout(
            ()=>{


                toast.classList.add(
                    "show"
                );


            },
            20
        );









        setTimeout(
            ()=>{


                toast.classList.remove(
                    "show"
                );


                setTimeout(
                    ()=>{


                        toast.remove();


                    },
                    300
                );


            },

            3000

        );




    },









    /*
        Loading button helper


        Example:


        App.loading(
          button,
          true,
          "Creating..."
        );


    */


    loading(
        button,
        state,
        text=""
    ){



        if(
            !button
        )
            return;







        if(
            state
        )
        {


            button.disabled=true;


            button.dataset.oldText =
                button.textContent;


            button.textContent =
                text
                ||
                "Processing...";



            button.classList.add(
                "loading"
            );



        }

        else

        {


            button.disabled=false;


            button.textContent =
                button.dataset.oldText
                ||
                "Submit";



            button.classList.remove(
                "loading"
            );



        }





    }







};









document.addEventListener(
    "DOMContentLoaded",
    ()=>{


        App.init();


    }
);






window.App =
    App;