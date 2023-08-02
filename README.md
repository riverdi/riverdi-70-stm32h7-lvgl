# LVGL port for the 7-inch Riverdi STM32 Embedded Displays (STM32H757XIH86)

This repository contains the __Light and Versatile Graphics Library (LVGL)__ port for the __7-inch Riverdi STM32 Embedded Displays__ with *STM32H757XIH86* MCU. It's ready-to-use project template which you can easily import into *STM32CubeIDE* and *STM32CubeMX* (to reconfigure selected peripherals). This project uses latest stable LVGL release (v8.3.8). For more info about LVGL (docs, API, tutorials) please visit [*https://lvgl.io/*](https://lvgl.io/) and [*https://github.com/lvgl/lvgl*](https://github.com/lvgl/lvgl)

![img1](/Docs/img_1.png)

This repository supports all configuration of 7-inch *Riverdi STM32 Embedded Displays*:

* [*RVT70HSSNWC00-B*](https://riverdi.com/product/7-inch-lcd-display-capacitive-touch-panel-optical-bonding-uxtouch-stm32h7-rvt70hssnwc00-b/) - *STM32H757XIH86, Capacitive Touch Panel, Optical bonding, uxTouch*
* [*RVT70HSSNWC00*](https://riverdi.com/product/7-inch-lcd-display-capacitive-touch-panel-air-bonding-uxtouch-stm32h7-rvt70hssnwc00/) - *STM32H757XIH86, Capacitive Touch Panel, Air bonding, uxTouch*
* [*RVT70HSSFWCA0*](https://riverdi.com/product/7-inch-lcd-display-capacitive-touch-panel-air-bonding-atouch-frame-stm32h7-rvt70hssfwca0/) - *STM32H757XIH86, Capacitive Touch Panel, Air bonding, aTouch*
* [*RVT70HSSNWCA0*](https://riverdi.com/product/7-inch-lcd-display-capacitive-touch-panel-air-bonding-atouch-stm32h7-rvt70hssnwca0/) - *STM32H757XIH86, Capacitive Touch Panel, Air bonding, aTouch*
* [*RVT70HSSFWN00*](https://riverdi.com/product/7-inch-lcd-display-stm32h7-frame-rvt70hssfwn00/) - *STM32H757XIH86*
* [*RVT70HSSNWN00*](https://riverdi.com/product/7-inch-lcd-display-stm32h7-rvt70hssnwn00/) - *STM32H757XIH86*

## What is Riverdi STM32 Embedded Display?

[*Riverdi’s Display STM32 Embedded*](https://riverdi.com/product-category/stm32-embedded-displays/?pr=26938) product series are a comprehensive solution for a variety of applications. With high resolution, high brightness, and a host of features, these displays are designed to meet the most demanding requirements.

Riverdi offers a variety of STM32 touch screen displays, including a brand new 5-inch STM32 display modules in its top-tier embedded screen solutions. With the STM32 7-inch LCD and 10.1-inch STM32 TFT display already on the market, the Riverdi LCD Display STM32 Embedded series is the all-in-one HMI solution well-suited for the most demanding applications.

The STM32 Embedded Displays are not just a product, they are an ecosystem based on the industrial-quality LCD-TFT displays with high-brightness, high-resolution, and industrial touchscreen, the performance and interfaces offered by the STM32 microcontrollers, and the comprehensive technical support with free software and libraries.

## 7-inch Riverdi STM32 Embedded Display

Main features of the 7-inch __Riverdi STM32 Embedded Display__:

* 7-inch TFT display
* 1024x600px resolution
* High brightness 800cd/m2
* Full viewing angles (IPS)
* STM32H757XIH6 (2MB Flash, 1MB RAM) microcontroller
* All STM32H7 interfaces
* Industrial projected capacitive touch screen
* Black decorative cover glass
* High quality – zero bad pixels
* 64MB QSPI
* 8MB SDRAM
* TouchGFX and LVGL direct support
* Power Supply: 6V-48V

![img2](/Docs/img_2.png)

* __RS485__
* __RS232__
* __Expansion connector__ - the board offers additional and independent GPIOs over a 40pin, 1.27mm male header. It provides direct access to the GPIOs of MCU STM32, that makes it possible to be easily extended by an addon board for specific application (2x I2C, 1x UART, 1x USART, 1x SPI, 1x USB, 7x PWMs, 2x DACs, 2x ADCs)
* __Fully independent 2x CAN FD__ - applicable in industrial and automotive area
* __Fast-programming SWD port__ - SWD connector allows to program STM32 and QSPI with customer’s applications. Riverdi developed the ST-LINK programming cable that is included in the STM32 Embedded Display sample package (single packing)
* __RiBUS connector__ - in some applications there might be a need to connect a second display in one device. With the STM32 Embedded Display line it is very easy as these displays are equipped with Master RiBUS connector – universal interface to Riverdi’s intelligent displays. In this way, the second display does nots need an external host controller and 2 independent displays can be controlled by one STM32
* __USB__ - can be configured to both Host and Device
* __Haptic feedback driver output__ - a DRV2605L haptic driver on the board (on I2C1) can be used to drive ERM and LRA haptic motors. Enhances feedback and conveys useful information to the users

## Riverdi STM32 Embedded Display unboxing

Have a look what you will get when you will order a sample of STM32 Embedded Display:

[![img3](/Docs/img_3.png)](https://www.youtube.com/watch?v=UwMa3z_OOmQ)

## How to import this project to STM32CubeIDE:

[1] Clone this respository (do not forget about *--recursive* flag!):
```
git clone --recursive https://github.com/riverdi/riverdi-70-stm32h7-lvgl.git
```
[2] Open *STM32CubeIDE* and import project:
```
File => Open Projects from File System... => Directory => Select the "riverdi-70-stm32h7-lvgl/STM32CubeIde" folder => Finish
```
[3] Build the project:
```
Project => Build all
```
[4] Upload the firmware to the *Riverdi STM32 Embedded Display*

## TODO

- performance improvement!
