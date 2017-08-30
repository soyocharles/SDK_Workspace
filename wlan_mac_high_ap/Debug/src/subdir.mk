################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/SOYO/Mango_802.11_RefDes_v1.7.1/EDK_Projects/Mango_802.11_RefDes_v1.7.1/SDK_Workspace/svn_src/wlan_mac_high_ap/wlan_exp_node_ap.c \
C:/Users/SOYO/Mango_802.11_RefDes_v1.7.1/EDK_Projects/Mango_802.11_RefDes_v1.7.1/SDK_Workspace/svn_src/wlan_mac_high_ap/wlan_mac_ap.c \
C:/Users/SOYO/Mango_802.11_RefDes_v1.7.1/EDK_Projects/Mango_802.11_RefDes_v1.7.1/SDK_Workspace/svn_src/wlan_mac_high_ap/wlan_mac_ap_uart_menu.c 

OBJS += \
./src/wlan_exp_node_ap.o \
./src/wlan_mac_ap.o \
./src/wlan_mac_ap_uart_menu.o 

C_DEPS += \
./src/wlan_exp_node_ap.d \
./src/wlan_mac_ap.d \
./src/wlan_mac_ap_uart_menu.d 


# Each subdirectory must supply rules for building sources it contributes
src/wlan_exp_node_ap.o: C:/Users/SOYO/Mango_802.11_RefDes_v1.7.1/EDK_Projects/Mango_802.11_RefDes_v1.7.1/SDK_Workspace/svn_src/wlan_mac_high_ap/wlan_exp_node_ap.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O3 -g -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_high_ap\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_high_framework\wlan_exp_ip_udp" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_common_framework\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_high_framework\include" -I../../wlan_bsp_cpu_high/mb_high/include -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_w3_common\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_w3_high\include" -fdata-sections -ffunction-sections -c -fmessage-length=0 -Wclobbered -Wempty-body -Wmissing-field-initializers -Wmissing-parameter-type -Wold-style-declaration -Woverride-init -Wtype-limits -Wuninitialized -Wunused-but-set-parameter -Wformat -Wstrict-aliasing -Wshadow -Wl,--no-relax -I../../wlan_bsp_cpu_high/mb_high/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.40.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '

src/wlan_mac_ap.o: C:/Users/SOYO/Mango_802.11_RefDes_v1.7.1/EDK_Projects/Mango_802.11_RefDes_v1.7.1/SDK_Workspace/svn_src/wlan_mac_high_ap/wlan_mac_ap.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O3 -g -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_high_ap\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_high_framework\wlan_exp_ip_udp" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_common_framework\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_high_framework\include" -I../../wlan_bsp_cpu_high/mb_high/include -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_w3_common\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_w3_high\include" -fdata-sections -ffunction-sections -c -fmessage-length=0 -Wclobbered -Wempty-body -Wmissing-field-initializers -Wmissing-parameter-type -Wold-style-declaration -Woverride-init -Wtype-limits -Wuninitialized -Wunused-but-set-parameter -Wformat -Wstrict-aliasing -Wshadow -Wl,--no-relax -I../../wlan_bsp_cpu_high/mb_high/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.40.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '

src/wlan_mac_ap_uart_menu.o: C:/Users/SOYO/Mango_802.11_RefDes_v1.7.1/EDK_Projects/Mango_802.11_RefDes_v1.7.1/SDK_Workspace/svn_src/wlan_mac_high_ap/wlan_mac_ap_uart_menu.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O3 -g -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_high_ap\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_high_framework\wlan_exp_ip_udp" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_common_framework\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_high_framework\include" -I../../wlan_bsp_cpu_high/mb_high/include -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_w3_common\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_w3_high\include" -fdata-sections -ffunction-sections -c -fmessage-length=0 -Wclobbered -Wempty-body -Wmissing-field-initializers -Wmissing-parameter-type -Wold-style-declaration -Woverride-init -Wtype-limits -Wuninitialized -Wunused-but-set-parameter -Wformat -Wstrict-aliasing -Wshadow -Wl,--no-relax -I../../wlan_bsp_cpu_high/mb_high/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.40.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


