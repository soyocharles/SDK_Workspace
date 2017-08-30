################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/SOYO/Mango_802.11_RefDes_v1.7.1/EDK_Projects/Mango_802.11_RefDes_v1.7.1/SDK_Workspace/svn_src/wlan_w3_common/w3_common.c \
C:/Users/SOYO/Mango_802.11_RefDes_v1.7.1/EDK_Projects/Mango_802.11_RefDes_v1.7.1/SDK_Workspace/svn_src/wlan_w3_common/w3_mac_time_util.c \
C:/Users/SOYO/Mango_802.11_RefDes_v1.7.1/EDK_Projects/Mango_802.11_RefDes_v1.7.1/SDK_Workspace/svn_src/wlan_w3_common/w3_sysmon_util.c \
C:/Users/SOYO/Mango_802.11_RefDes_v1.7.1/EDK_Projects/Mango_802.11_RefDes_v1.7.1/SDK_Workspace/svn_src/wlan_w3_common/w3_userio_util.c 

OBJS += \
./wlan_platform_common/w3_common.o \
./wlan_platform_common/w3_mac_time_util.o \
./wlan_platform_common/w3_sysmon_util.o \
./wlan_platform_common/w3_userio_util.o 

C_DEPS += \
./wlan_platform_common/w3_common.d \
./wlan_platform_common/w3_mac_time_util.d \
./wlan_platform_common/w3_sysmon_util.d \
./wlan_platform_common/w3_userio_util.d 


# Each subdirectory must supply rules for building sources it contributes
wlan_platform_common/w3_common.o: C:/Users/SOYO/Mango_802.11_RefDes_v1.7.1/EDK_Projects/Mango_802.11_RefDes_v1.7.1/SDK_Workspace/svn_src/wlan_w3_common/w3_common.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O3 -g -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_low_dcf\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_w3_common\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_w3_low\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_common_framework\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_low_framework\include" -I../../wlan_bsp_cpu_low/mb_low/include -fdata-sections -ffunction-sections  -c -fmessage-length=0 -Wclobbered -Wempty-body -Wmissing-field-initializers -Wmissing-parameter-type -Wold-style-declaration -Woverride-init -Wtype-limits -Wuninitialized -Wunused-but-set-parameter -Wformat -Wstrict-aliasing -Wshadow -Wl,--no-relax -I../../wlan_bsp_cpu_low/mb_low/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v8.40.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '

wlan_platform_common/w3_mac_time_util.o: C:/Users/SOYO/Mango_802.11_RefDes_v1.7.1/EDK_Projects/Mango_802.11_RefDes_v1.7.1/SDK_Workspace/svn_src/wlan_w3_common/w3_mac_time_util.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O3 -g -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_low_dcf\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_w3_common\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_w3_low\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_common_framework\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_low_framework\include" -I../../wlan_bsp_cpu_low/mb_low/include -fdata-sections -ffunction-sections  -c -fmessage-length=0 -Wclobbered -Wempty-body -Wmissing-field-initializers -Wmissing-parameter-type -Wold-style-declaration -Woverride-init -Wtype-limits -Wuninitialized -Wunused-but-set-parameter -Wformat -Wstrict-aliasing -Wshadow -Wl,--no-relax -I../../wlan_bsp_cpu_low/mb_low/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v8.40.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '

wlan_platform_common/w3_sysmon_util.o: C:/Users/SOYO/Mango_802.11_RefDes_v1.7.1/EDK_Projects/Mango_802.11_RefDes_v1.7.1/SDK_Workspace/svn_src/wlan_w3_common/w3_sysmon_util.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O3 -g -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_low_dcf\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_w3_common\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_w3_low\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_common_framework\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_low_framework\include" -I../../wlan_bsp_cpu_low/mb_low/include -fdata-sections -ffunction-sections  -c -fmessage-length=0 -Wclobbered -Wempty-body -Wmissing-field-initializers -Wmissing-parameter-type -Wold-style-declaration -Woverride-init -Wtype-limits -Wuninitialized -Wunused-but-set-parameter -Wformat -Wstrict-aliasing -Wshadow -Wl,--no-relax -I../../wlan_bsp_cpu_low/mb_low/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v8.40.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '

wlan_platform_common/w3_userio_util.o: C:/Users/SOYO/Mango_802.11_RefDes_v1.7.1/EDK_Projects/Mango_802.11_RefDes_v1.7.1/SDK_Workspace/svn_src/wlan_w3_common/w3_userio_util.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O3 -g -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_low_dcf\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_w3_common\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_w3_low\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_common_framework\include" -I"C:\Users\SOYO\Mango_802.11_RefDes_v1.7.1\EDK_Projects\Mango_802.11_RefDes_v1.7.1\SDK_Workspace\svn_src\wlan_mac_low_framework\include" -I../../wlan_bsp_cpu_low/mb_low/include -fdata-sections -ffunction-sections  -c -fmessage-length=0 -Wclobbered -Wempty-body -Wmissing-field-initializers -Wmissing-parameter-type -Wold-style-declaration -Woverride-init -Wtype-limits -Wuninitialized -Wunused-but-set-parameter -Wformat -Wstrict-aliasing -Wshadow -Wl,--no-relax -I../../wlan_bsp_cpu_low/mb_low/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v8.40.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


