################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
drivers/buttons.obj: /home/bigbywolf/ti/TivaWare_C_Series-2.1.3.156/examples/boards/ek-tm4c123gxl/drivers/buttons.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/bigbywolf/ti/ccsv7/tools/compiler/ti-cgt-arm_17.6.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="/home/bigbywolf/ti/ccsv7/tools/compiler/ti-cgt-arm_17.6.0.STS/include" --include_path="/home/bigbywolf/Workspace/ble_rover/ble_rover" --include_path="/home/bigbywolf/ti/TivaWare_C_Series-2.1.3.156/examples/boards/ek-tm4c123gxl" --include_path="/home/bigbywolf/ti/TivaWare_C_Series-2.1.3.156" --include_path="/home/bigbywolf/ti/TivaWare_C_Series-2.1.3.156/third_party" --include_path="/home/bigbywolf/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/include" --include_path="/home/bigbywolf/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS" --include_path="/home/bigbywolf/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power=all --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="drivers/buttons.d" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

drivers/rgb.obj: /home/bigbywolf/ti/TivaWare_C_Series-2.1.3.156/examples/boards/ek-tm4c123gxl/drivers/rgb.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/bigbywolf/ti/ccsv7/tools/compiler/ti-cgt-arm_17.6.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="/home/bigbywolf/ti/ccsv7/tools/compiler/ti-cgt-arm_17.6.0.STS/include" --include_path="/home/bigbywolf/Workspace/ble_rover/ble_rover" --include_path="/home/bigbywolf/ti/TivaWare_C_Series-2.1.3.156/examples/boards/ek-tm4c123gxl" --include_path="/home/bigbywolf/ti/TivaWare_C_Series-2.1.3.156" --include_path="/home/bigbywolf/ti/TivaWare_C_Series-2.1.3.156/third_party" --include_path="/home/bigbywolf/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/include" --include_path="/home/bigbywolf/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS" --include_path="/home/bigbywolf/ti/TivaWare_C_Series-2.1.3.156/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power=all --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="drivers/rgb.d" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


