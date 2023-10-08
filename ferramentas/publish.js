const mqtt = require('mqtt')

protocolo = 'mqtt',
host = '192.168.1.45',
porta = '1883',
clientId = `mqtt_${Math.random().toString(16).slice(3)}`,
connectUrl = `${protocolo}://${host}:${porta}`,

client = mqtt.connect(connectUrl, {
    clientId,
    clean: true,
    connectTimeout: 4000,
    username: 'aplicacaoTeste',
    password: 'public',
    reconnectPeriod:1000,
})
console.log("conectado")

conexao = {
    publica(topico, payload){
        client.publish(topico, payload, {qos: 0, retain: false}, (error) =>{
            if(error) { 
                console.log(error)
            }
        })
    },

    inscreverTopico(topico){
        client.on('connect', () => {
            console.log('Connected')
            client.subscribe(topico, () => {
                console.log(`inscrito no topico ${topico}`)
            })
        })
    },

    log(topico, payload){
        client.on('message', (topico, payload) => {
            console.log('Received Message:', topic, payload.toString())
        })
    }
}
module.exports.conec = conexao