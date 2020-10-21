window.addEventListener("load", startup, false);

function startup() {
    let headerColorPicker = document.querySelector('#header-color');
    let textColorPicker = document.querySelector('#text-color');
    
    headerColorPicker.addEventListener("change", changeHeader, false);
    textColorPicker.addEventListener("change", changeText, false);
}

function changeHeader(e) {
    let header = document.querySelector("h1");
    header.style.color = e.target.value;
    
}

function changeText(e) {
    let main = document.querySelector("p");
    main.style.color = e.target.value;
}