# Como rodar?

Este projeto foi desenvolvido na placa [BitDogLab](https://github.com/BitDogLab/), então todas as conexões usadas podem ser vistas
em melhor detalhe lá. De componentes externso foram utilizados o botão B, o joystick (os dois conversores ADC apenas) e o display OLED.

É necessário ter instalado cmake e o [sdk do raspberry pi pico](https://github.com/raspberrypi/pico-sdk)

Ou somente a extensão do VSCode.

Para compilar:

```bash
mkdir build
cd build
cmake ..
make
```

Após buildar o projeto, dê um flash dump do main.uf2 na placa. Pode fazer isso pelo VSCode ou utilizando os scripts bash (para linux somente):

```bash
./check-conn.bash
```

Que deve ter um output semelhante a:

```bash
# dmesg | tail
[ 3256.835254] scsi 1:0:0:0: Direct-Access     RPI      RP2              3    PQ: 0 ANSI: 2
[ 3256.835926] sd 1:0:0:0: Attached scsi generic sg0 type 0
[ 3256.837485] sd 1:0:0:0: [sda] 262144 512-byte logical blocks: (134 MB/128 MiB)
[ 3256.838035] sd 1:0:0:0: [sda] Write Protect is off
[ 3256.838043] sd 1:0:0:0: [sda] Mode Sense: 03 00 00 00
[ 3256.838565] sd 1:0:0:0: [sda] No Caching mode page found
[ 3256.838573] sd 1:0:0:0: [sda] Assuming drive cache: write through
[ 3256.844920]  sda: sda1
[ 3256.845143] sd 1:0:0:0: [sda] Attached SCSI removable disk
[ 3256.971134] [UFW BLOCK] IN=wlp0s20f3 OUT= MAC= SRC=192.168.0.14 DST=239.255.255.250 LEN=635 TOS=0x00 PREC=0x00 TTL=1 ID=39304 DF PROTO=UDP SPT=50917 DPT=3702 LEN=615 
####
# Now use `sudo ./flash.bash ./build/main.uf2 sdd1`, changing sdd1 for whatever interface dmesg shows
```

É possível ver que a placa nesse caso foi "montada" em sda1. Rode então o script `flash.bash` passando o `.uf2` e o local onde a placa foi montada:

```bash
sudo ./flash.bash ./build/main.uf2 sda1
```

E agora o programa já deve estar rodando no seu Raspberry.


# Referências

Repositório de referência:
https://github.com/BitDogLab/BitDogLab-C

Fonte utilizada para o texto 3x5:
https://robey.lag.net/2010/01/23/tiny-monospace-font.html

