const botao_ventilador = document.getElementById("btn_ventilador");
const botao_lampadaQuarto = document.getElementById("btn_lampQ")
const botao_lampadaMesa = document.getElementById("btn_lampM")
const botao_fechadura = document.getElementById("btn_tranca")

botao_fechadura.addEventListener('click', ()=>{
    console.log("fechadura acionada!")
    fetch('http://192.168.1.37:3000/iot/tranca', {
        method:"POST"
    })
        .then(res=>console.log(res))
})
botao_lampadaMesa.addEventListener('click', ()=>{
    console.log("lampada da mesa acionada!")
    fetch('http://192.168.1.37:3000/iot/lampm', {
        method: "POST"
    })
        .then(res => console.log(res))
})
botao_lampadaQuarto.addEventListener('click', ()=>{
    console.log("lampada do quarto acionada")
    fetch('http://192.168.1.37:3000/iot/lampq',{
        method: "POST"
    })
        .then(res => console.log(res))
})

botao_ventilador.addEventListener('click', function(){
    console.log("ventilador acionado!");
    fetch('http://192.168.1.37:3000/iot/vento',{
        method: "POST",
    })
        .then(res => console.log(res))
})