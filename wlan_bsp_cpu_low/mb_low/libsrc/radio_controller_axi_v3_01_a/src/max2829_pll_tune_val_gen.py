# Python code to generate arrays of PLL tuning register values for MAX2829

def calc_div_ratio_24(center_freq):
    return (center_freq * 4.0/3.0) / 20.0

def calc_div_ratio_5(center_freq):
    return (center_freq * 4.0/5.0) / 20.0

def calc_div_ratio_int(div_ratio):
    return int(div_ratio)

def calc_div_ratio_frac_as_int(div_ratio):
    return int(div_ratio * 2**16)

def calc_reg34_vals(div_ratio):
    # Each divider ratio is represented as a UFix26_16 (10 integer bits, 16 fractional bits)
    # reg3:
    # [13:12]: 2LSB of fractional part of div ratio
    # [11: 8]: Zeros
    # [ 9: 0]: Integer part (UFix10_0) of div ratio
    # reg4:
    # [13: 0]: 14MSB of fractional part of div ratio

    dr_i = calc_div_ratio_int(div_ratio)
    df_fi = calc_div_ratio_frac_as_int(div_ratio)

    reg3 = (dr_i & 0x3FF) | ((df_fi & 0x3) << 12)
    reg4 = ((df_fi & 0xFFFC) >> 2)

    return (reg3, reg4)

center_freqs_24 = [2412,   2417,   2422,   2427,   2432,   2437,   2442,   2447,   2452,   2457,   2462,   2467,   2472,   2484]
div_ratios_24 = map(calc_div_ratio_24, center_freqs_24)
regs = map(calc_reg34_vals, div_ratios_24)

c_l1 = 'static u16 rc_tuningParams_24GHz_freqs[{0}] = {{ {1} }};'.format(len(center_freqs_24), ', '.join(map(str, center_freqs_24)))
c_l2 = 'static u16 rc_tuningParams_24GHz_reg3[{0}] = {{ {1} }};'.format(len(center_freqs_24), ', '.join(map(hex, [x[0] for x in regs])))
c_l3 = 'static u16 rc_tuningParams_24GHz_reg4[{0}] = {{ {1} }};'.format(len(center_freqs_24), ', '.join(map(hex, [x[1] for x in regs])))

print(c_l1 + '\n' + c_l2 + '\n' + c_l3 + '\n')

center_freqs_5 = [  5180, 5190, 5200, 5220, 5230, 5240, 5260, 5270, 5280, 5300,
                    5310, 5320, 5500, 5510, 5520, 5540, 5550, 5560, 5580, 5590,
                    5600, 5620, 5630, 5640, 5660, 5670, 5680, 5700, 5710, 5720,
                    5745, 5755, 5765, 5785, 5795, 5805, 5825, 5860, 5870, 5875,
                    5880, 5885, 5890, 5865]

div_ratios_5 = map(calc_div_ratio_5, center_freqs_5)
regs = map(calc_reg34_vals, div_ratios_5)

c_l1 = 'static u16 rc_tuningParams_5GHz_freqs[{0}] = {{ {1} }};'.format(len(center_freqs_5), ', '.join(map(str, center_freqs_5)))
c_l2 = 'static u16 rc_tuningParams_5GHz_reg3[{0}] = {{ {1} }};'.format(len(center_freqs_5), ', '.join(map(hex, [x[0] for x in regs])))
c_l3 = 'static u16 rc_tuningParams_5GHz_reg4[{0}] = {{ {1} }};'.format(len(center_freqs_5), ', '.join(map(hex, [x[1] for x in regs])))

print(c_l1 + '\n' + c_l2 + '\n' + c_l3 + '\n')

if 0:
    # Print doxygen-style table of channel indexes and center frequencies
    #  Can be copy/pasted into doxygen comment above radio_controller_setCenterFrequency

    print('2.4GHz <br> Chan | Freq <br> (MHz) | | 5GHz <br> Chan | Freq <br> (MHz)')
    print('--- | ----- | - | --- | -----')

    # Want lines like:
    # '  1 | 2412  |   |   1 | 5180'
    # ' 14 | 2484  |   |  14 | 5510'
    # '    |       |   |  22 | 5620'
    for ii in range(len(center_freqs_5)):
        if(ii < len(center_freqs_24)):
            print('{0} | {1} |  | {0} | {2}'.format(ii+1, center_freqs_24[ii], center_freqs_5[ii]))
        else:
            print('- | - | | {0} | {1}'.format(ii+1, center_freqs_5[ii]))

if 1:
    # Print wiki-style table output, to be copy-pasted into warplab command doc string

    # ||||=  2.4GHz  =||||=  5GHz  =||
    # || Chan || Freq || Chan || Freq ||
    # ||  1 ||    2412 ||  1 ||    5180 ||
    # || || ||  12 ||   5560 ||

    print('                    % ||||=  2.4GHz  =||||=  5GHz  =||')
    print('                    % || Chan || Freq || Chan || Freq ||')

    for ii in range(len(center_freqs_5)):
        if(ii < len(center_freqs_24)):
            print('                    % ||  {0} ||    {1} ||  {0} ||    {2} ||'.format(ii+1, center_freqs_24[ii], center_freqs_5[ii]))
        else:
            print('                    % || || ||  {0} ||    {1} ||'.format(ii+1, center_freqs_5[ii]))

