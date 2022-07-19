# Fade LED com sinal PWM

Autora: Lesly Montúfar

Definindo intensidade do brilho do led, utilizando um sinal de saída PWM, implementado com um temporizador do STM32.

## Índice 

* [Descrição](#descrição)
* [Desenvolvimento](#desenvolvimento)

## Descrição

Para essa atividade será construída uma aplicação onde o usuário irá controlar o brilho de um LED
(0 a 100%) através de um botão. 

1. O LED deve iniciar apagado e a cada clique no botão o brilho do led
deve aumentar 10%. Caso o valor 100% seja alcançado, o próximo pressionamento do botão leva novamente o brilho para 0%, ou seja, a aplicação irá ciclar indefinidamente. 

2. Adicionalmente, se o usuário ficar com o botão pressionado por 3 segundos o LED deve apagar (0% de brilho).

3. Como sugestão, use o canal 1 do TIM1, pino PA8. Não configure o pino PC13 e coloque um jumper
entre PA8 e PC13.

4. O botão deve ser tratado por interrupção e você não deve usar a função HAL_Delay() ou qualquer
outra que bloqueie o tratamento da interrupção. O deboucing é obrigatório.

## Desenvolvimento



