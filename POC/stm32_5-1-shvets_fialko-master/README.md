# Lab work 5: STM32
# Directed light and Binary counter
> Authors (team): Shvets Anastasiia, Fialko Yaryna<br>

## Prerequisites

* STM32F411E-DISCO    (real device)
* STM32CubeIDE    (application)

## Results
#### Directed light
After you flash your STM32, you will see he leds blinking in circle order.<br>
You can press the blue button to change its direction.

![img.png](images/directed_lights.gif) <br>

#### Binary counter
After you flash your STM32 and construct the scheme, you have to press the button to see that something is happening.<br> 
The number of times button been pressed will appear on the led bar in binary format.

![img.png](images/counter.gif) <br>


## Configuration
> in STM32CubeIDE

### Directed light
###### Leds

To apply PWM we should choose *TIM4_CHx* in leds (PD12, PD13, PD14, PD15 pins) options.<br>
![img.png](images/img3.png) <br>
<br>
And then in `Timers` choose `TIM4` and set the settings as follows:
![img.png](images/img4.png) <br>
Pay attention to **Clock Source** and **Channelx**.<br>
<br>
![img.png](images/img5.png) <br>
Pay attention to **Counter Period**.

###### Button interrupt
Also, to make the button able to interrupt choose in PA0 pin options *GPIO_EXTI0*.<br>
![img.png](images/img6.png)<br>
<br>
In `GPIO` choose PA0 to config it, and set **GPIO mode** as *Interrupt with Rising/Falling edge trigger*,<br> and also set **User Label** as *"USR_BUTTON"*.<br>
![img.png](images/img7.png)<br>
<br>
In `GPIO` go to `NVIC` section and enable interrupt on *EXTI line0*.
![img.png](images/img8.png) <br>
<br>

### Binary counter
###### Leds
Set all of PE6 - PE15 pins as *GPIO_Output*.
![img.png](images/img2.png) <br>

###### Button interrupt
Also, to make the button able to interrupt choose in PA1 pin options *GPIO_EXTI1*.<br>
![img_1.png](images/img_1.png) <br>
<br>
In `GPIO` choose PA1 to config it, and set **GPIO mode** as *Interrupt with Rising/Falling edge trigger*.<br>
![img.png](images/img.png) <br>
<br>
In `GPIO` go to `NVIC` section and enable interrupt on *EXTI line1*. <br>
![img.png](images/img1.png) <br>
<br>


### The scheme for binary counter
![img.png](images/scheme.PNG) <br>




## Additional tasks
#### Directed light
* Task 1: Use PWM to make leds dim.
* Task 2: Use interrupts.
#### Binary counter
* Task 1: Use interrupts.


