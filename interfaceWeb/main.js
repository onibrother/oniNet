const express= require('express')
const app = express()
const publish = require('./ferramentas/publish')

app.use('/', express.static('public'))

app.post('/iot/:dispositivo', (req,res)=>{
    switch(req.params.dispositivo){
        case 'vento':
            console.log("comando para o ventilador")
            res.send("ventilador acionado")
            conexao.publica('mesa', 'vento')
            break
        case 'lampq':
            console.log("comando para a lampada do quarto")
            res.send("lampada acionada")
            conexao.publica('tranca', 'lamp')
            break
        case 'lampm':
            console.log("commando para a lampada da mesa")
            res.send("lampada da mesa acionada")
            conexao.publica('mesa', 'lamp')
            break
        case 'tranca':
            console.log("commando para a fechadura")
            res.send("fechadura do quarto acionada")
            conexao.publica('tranca', 'porta')
            break
    }   
})

app.listen(3000, ()=>{
    console.log('servico rodando!')
})