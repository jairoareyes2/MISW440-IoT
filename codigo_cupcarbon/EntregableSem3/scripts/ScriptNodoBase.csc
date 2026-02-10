atget id id
set simt 0

data p "hola" id id
send p

loop

wait 1000
inc simt 100

read mens
rdata mens tipo valor1 valor2

if( tipo == "alerta")
   cprint "[" simt " ms] Alerta en: " valor1 "," valor2
end

if(tipo == "critico")
   cprint "[" simt " ms] Nodo descargado en: " valor1 "," valor2
   data p "stop"	
   send p
   wait 1000
   stop
end
