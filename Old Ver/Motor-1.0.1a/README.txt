250312
ADC Code Test

基礎設定+
tim2(tim3) {
    TIM2 global interrupt = true

    PSC = APB1_TIM_Clock
    ARR = TIM2_AutoReload
    Pulse = 0
}