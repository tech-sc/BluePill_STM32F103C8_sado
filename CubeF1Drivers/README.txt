CMSIS v1.7.0

・CubeF1Driversフォルダ
	オリジナルのSTM32Cube_FW_F1_V1.7.0/Driversフォルダと同等レベル.
	但し、以下のフォルダだけを抽出した.

・CMSIS/Device
	ST/STM32F1xx/Source/Templates配下
		armフォルダ、gccフォルダはコピーせず.
		iarフォルダをコピー.
		system_stm32f1xx.cをコピー.
	ST/STM32F1xx/Includeをコピー

・CMSIS/Lib
	GCCはコピーせず.
	ARMフォルダをコピー.

・CMSIS/Include
	全てコピー.

・STM32F1xx_HAL_Driver/Inc
	全てコピー.

・STM32F1xx_HAL_Driver/Src
	全てコピー.

●ターゲットMCU(STM32F103C8)の関連ファイルを抽出.
	CubeF1Drivers/CMSIS\Device/ST\STM32F1xx/Include/stm32f1xx.hを参照して
	STM32F103C8は、STM32F103xB に属するようだ.

・startup_stm32f103xb.s
	CubeF1Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/iarフォルダから
	CubeF1Driversフォルダ配下にコピー.
	git mv CMSIS/Device/ST/STM32F1xx/Source/Templates/iar/startup_stm32f103xb.s .

・stm32f103xb_flash.icf
	CubeF1Drivers\CMSIS\Device\ST\STM32F1xx\Source\Templates\iar\linkerフォルダから
	CubeF1Driversフォルダ配下にコピー.
	git mv CMSIS/Device/ST/STM32F1xx/Source/Templates/iar/linker/stm32f103xb_flash.icf .

・system_stm32f1xx.c
	CubeF1Drivers\CMSIS\Device\ST\STM32F1xx\Source\Templatesフォルダから
	CubeF1Driversフォルダ配下に移動.
	git mv CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c .

・stm32_assert.h
	CubeF1Drivers\STM32F1xx_HAL_Driver\Inc\stm32_assert_template.hから
	CubeF3Driversフォルダ配下にリネーム移動.
	git mv STM32F1xx_HAL_Driver/Inc/stm32_assert_template.h stm32_assert.h

・stm32f3xx_hal_conf.h
	CubeF1Drivers\STM32F1xx_HAL_Driver\Inc\stm32f1xx_hal_conf_template.hから
	CubeF1Driversフォルダ配下にリネーム移動.
	git mv STM32F1xx_HAL_Driver/Inc/stm32f1xx_hal_conf_template.h stm32f3xx_hal_conf.h
