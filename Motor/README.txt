250312
Remove ADC 

基礎設定+
tim2(tim3) {
    TIM2 global interrupt = true

    PSC = APB1_TIM_CLOCK
    ARR = TIM2_AUTORELOAD
    Pulse = 0
}