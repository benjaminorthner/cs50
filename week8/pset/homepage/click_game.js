

let clickGameBtn = document.getElementById("clickGameBtn");
let clickGamePB = document.getElementById("clickGamePB");

let currentClicks = 0;
let maxClicks = 29;

clickGameBtn.addEventListener('click', function(){

    currentClicks++;

    if (currentClicks <= maxClicks){

        let percentage = Math.round(100 * currentClicks / maxClicks) + "%";
        clickGamePB.innerHTML = percentage
        clickGamePB.style.width = percentage
    }
    else if (currentClicks < maxClicks + 20){
        clickGameBtn.innerHTML = "FINISHED"
    }
    else if (currentClicks < maxClicks + 20 + 40){
        clickGameBtn.innerHTML = "STOP!"
    }
});