
�
I%sTime (s): cpu = %s ; elapsed = %s . Memory (MB): peak = %s ; gain = %s
268*common2
create_project: 2

00:00:052

00:00:062	
619.3522	
191.316Z17-268h px� 
�
Command: %s
1870*	planAhead2�
�read_checkpoint -auto_incremental -incremental C:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/utils_1/imports/synth_1/tb_system.dcpZ12-2866h px� 
�
;Read reference checkpoint from %s for incremental synthesis3154*	planAhead2]
[C:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/utils_1/imports/synth_1/tb_system.dcpZ12-5825h px� 
T
-Please ensure there are no constraint changes3725*	planAheadZ12-7989h px� 
e
Command: %s
53*	vivadotcl24
2synth_design -top tb_system -part xc7s100fgga676-2Z4-113h px� 
:
Starting synth_design
149*	vivadotclZ4-321h px� 
z
@Attempting to get a license for feature '%s' and/or device '%s'
308*common2
	Synthesis2	
xc7s100Z17-347h px� 
j
0Got license for feature '%s' and/or device '%s'
310*common2
	Synthesis2	
xc7s100Z17-349h px� 

VNo compile time benefit to using incremental synthesis; A full resynthesis will be run2353*designutilsZ20-5440h px� 
�
�Flow is switching to default flow due to incremental criteria not met. If you would like to alter this behaviour and have the flow terminate instead, please set the following parameter config_implementation {autoIncr.Synth.RejectBehavior Terminate}2229*designutilsZ20-4379h px� 
o
HMultithreading enabled for synth_design using a maximum of %s processes.4828*oasys2
2Z8-7079h px� 
a
?Launching helper process for spawning children vivado processes4827*oasysZ8-7078h px� 
M
#Helper process launched with PID %s4824*oasys2
5208Z8-7075h px� 
�
%s*synth2v
tStarting Synthesize : Time (s): cpu = 00:00:05 ; elapsed = 00:00:05 . Memory (MB): peak = 1098.469 ; gain = 466.473
h px� 
�
synthesizing module '%s'%s4497*oasys2
	tb_system2
 2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
38@Z8-6157h px� 
�
display: %s251*oasys2
Starting testbench...2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
698@Z8-251h px� 
�
!system %s call '%s' not supported38048*oasys2

function2
time2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
718@Z8-11581h px� 
�
<%s inside initial block, initial block items will be ignored4742*oasys2
wait statement is2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
738@Z8-6896h px� 
�
+always block has no event control specified85*oasys2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
108@Z8-85h px� 
�
synthesizing module '%s'%s4497*oasys2
rgb_to_gray2
 2]
YC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/grayscale_converter.v2
38@Z8-6157h px� 
X
%s
*synth2@
>	Parameter MEM_FILE bound to: input_image.mem - type: string 
h p
x
� 
R
%s
*synth2:
8	Parameter TOTAL_BYTES bound to: 10800 - type: integer 
h p
x
� 
�
display: %s251*oasys2*
(Loading RGB image from 'input_image.mem'2]
YC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/grayscale_converter.v2
228@Z8-251h px� 
�
,$readmem data file '%s' is read successfully3426*oasys2
input_image.mem2]
YC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/grayscale_converter.v2
238@Z8-3876h px� 
�
display: %s251*oasys2!
mem[0]=xx, mem[1]=xx, mem[2]=xx2]
YC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/grayscale_converter.v2
248@Z8-251h px� 
�
-case statement is not full and has no default155*oasys2]
YC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/grayscale_converter.v2
398@Z8-155h px� 
�
'done synthesizing module '%s'%s (%s#%s)4495*oasys2
rgb_to_gray2
 2
02
12]
YC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/grayscale_converter.v2
38@Z8-6155h px� 
�
synthesizing module '%s'%s4497*oasys2
median_filter2
 2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
38@Z8-6157h px� 
E
%s
*synth2-
+	Parameter W bound to: 60 - type: integer 
h p
x
� 
E
%s
*synth2-
+	Parameter H bound to: 60 - type: integer 
h p
x
� 
�
'done synthesizing module '%s'%s (%s#%s)4495*oasys2
median_filter2
 2
02
12W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
38@Z8-6155h px� 
�
!system %s call '%s' not supported38048*oasys2

function2
time2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1088@Z8-11581h px� 
�
!system %s call '%s' not supported38048*oasys2

function2
time2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1138@Z8-11581h px� 
�
!system %s call '%s' not supported38048*oasys2

function2
time2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1238@Z8-11581h px� 
�
!system %s call '%s' not supported38048*oasys2

function2
time2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1248@Z8-11581h px� 
�
!system %s call '%s' not supported38048*oasys2

function2
time2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1288@Z8-11581h px� 
�
!system %s call '%s' not supported38048*oasys2

function2
time2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1318@Z8-11581h px� 
�
'done synthesizing module '%s'%s (%s#%s)4495*oasys2
	tb_system2
 2
02
12L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
38@Z8-6155h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
	ready_reg2]
YC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/grayscale_converter.v2
368@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
window_reg[0]2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
298@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
window_reg[1]2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
298@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
window_reg[2]2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
298@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
window_reg[3]2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
298@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
window_reg[4]2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
298@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
window_reg[5]2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
298@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
window_reg[6]2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
298@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
window_reg[7]2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
298@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
window_reg[8]2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
298@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
lb2_col_reg2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
508@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
lb2_col_1_reg2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
518@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
lb2_col_2_reg2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
528@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
lb1_col_reg2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
538@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
lb1_col_1_reg2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
548@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
lb1_col_2_reg2W
SC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/median_filter.v2
558@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
gray_first_output_cycle_reg2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1088@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
first_gray_output_seen_reg2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1078@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
gray_pixel_count_reg2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1128@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
gray_last_output_cycle_reg2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1138@Z8-6014h px� 
_
+Unused sequential element %s was removed. 
4326*oasys2
output_mem_regZ8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
first_output_cycle_reg2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1238@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
filt_first_output_cycle_reg2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1248@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
first_output_seen_reg2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1228@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
filt_last_output_cycle_reg2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1288@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
last_output_cycle_reg2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1318@Z8-6014h px� 
�
+Unused sequential element %s was removed. 
4326*oasys2
output_index_reg2L
HC:/Users/zolil/Verilog_modules2/Verilog_modules2.srcs/sources_1/new/tb.v2
1198@Z8-6014h px� 
�
%s*synth2v
tFinished Synthesize : Time (s): cpu = 00:00:06 ; elapsed = 00:00:06 . Memory (MB): peak = 1242.258 ; gain = 610.262
h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
�
%s*synth2�
Finished Constraint Validation : Time (s): cpu = 00:00:07 ; elapsed = 00:00:07 . Memory (MB): peak = 1242.258 ; gain = 610.262
h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
D
%s
*synth2,
*Start Loading Part and Timing Information
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
9
%s
*synth2!
Loading part: xc7s100fgga676-2
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
�
%s*synth2�
�Finished Loading Part and Timing Information : Time (s): cpu = 00:00:07 ; elapsed = 00:00:07 . Memory (MB): peak = 1242.258 ; gain = 610.262
h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
E
Loading part %s157*device2
xc7s100fgga676-2Z21-403h px� 
o
3inferred FSM for state register '%s' in module '%s'802*oasys2
	state_reg2
rgb_to_grayZ8-802h px� 
~
%s
*synth2f
d---------------------------------------------------------------------------------------------------
h p
x
� 
z
%s
*synth2b
`                   State |                     New Encoding |                Previous Encoding 
h p
x
� 
~
%s
*synth2f
d---------------------------------------------------------------------------------------------------
h p
x
� 
y
%s
*synth2a
_                  iSTATE |                              001 |                               00
h p
x
� 
y
%s
*synth2a
_                 iSTATE0 |                              010 |                               01
h p
x
� 
y
%s
*synth2a
_                 iSTATE1 |                              100 |                               10
h p
x
� 
~
%s
*synth2f
d---------------------------------------------------------------------------------------------------
h p
x
� 
�
Gencoded FSM with state register '%s' using encoding '%s' in module '%s'3353*oasys2
	state_reg2	
one-hot2
rgb_to_grayZ8-3354h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
�
%s*synth2�
�Finished RTL Optimization Phase 2 : Time (s): cpu = 00:00:09 ; elapsed = 00:00:08 . Memory (MB): peak = 1242.258 ; gain = 610.262
h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
:
%s
*synth2"
 Start RTL Component Statistics 
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
9
%s
*synth2!
Detailed RTL Component Info : 
h p
x
� 
(
%s
*synth2
+---Adders : 
h p
x
� 
F
%s
*synth2.
,	   2 Input   32 Bit       Adders := 1     
h p
x
� 
F
%s
*synth2.
,	   2 Input   14 Bit       Adders := 1     
h p
x
� 
+
%s
*synth2
+---Registers : 
h p
x
� 
H
%s
*synth20
.	               32 Bit    Registers := 1     
h p
x
� 
H
%s
*synth20
.	               16 Bit    Registers := 2     
h p
x
� 
H
%s
*synth20
.	                8 Bit    Registers := 125   
h p
x
� 
H
%s
*synth20
.	                1 Bit    Registers := 2     
h p
x
� 
'
%s
*synth2
+---Muxes : 
h p
x
� 
F
%s
*synth2.
,	   2 Input    8 Bit        Muxes := 64    
h p
x
� 
F
%s
*synth2.
,	   3 Input    1 Bit        Muxes := 4     
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
=
%s
*synth2%
#Finished RTL Component Statistics 
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
6
%s
*synth2
Start Part Resource Summary
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
q
%s
*synth2Y
WPart Resources:
DSPs: 160 (col length:80)
BRAMs: 240 (col length: RAMB18 80 RAMB36 40)
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
9
%s
*synth2!
Finished Part Resource Summary
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
E
%s
*synth2-
+Start Cross Boundary and Area Optimization
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
H
&Parallel synthesis criteria is not met4829*oasysZ8-7080h px� 
�
ESequential element (%s) is unused and will be removed from module %s.3332*oasys2#
!gray_inst/FSM_onehot_state_reg[2]2
	tb_systemZ8-3332h px� 
�
ESequential element (%s) is unused and will be removed from module %s.3332*oasys2#
!gray_inst/FSM_onehot_state_reg[1]2
	tb_systemZ8-3332h px� 
�
ESequential element (%s) is unused and will be removed from module %s.3332*oasys2#
!gray_inst/FSM_onehot_state_reg[0]2
	tb_systemZ8-3332h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
�
%s*synth2�
�Finished Cross Boundary and Area Optimization : Time (s): cpu = 00:00:38 ; elapsed = 00:00:38 . Memory (MB): peak = 1465.207 ; gain = 833.211
h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
�
%s*synth2�
�---------------------------------------------------------------------------------
Start ROM, RAM, DSP, Shift Register and Retiming Reporting
h px� 
l
%s*synth2T
R---------------------------------------------------------------------------------
h px� 
;
%s*synth2#
!
ROM: Preliminary Mapping Report
h px� 
W
%s*synth2?
=+------------+------------+---------------+----------------+
h px� 
X
%s*synth2@
>|Module Name | RTL Object | Depth x Width | Implemented As | 
h px� 
W
%s*synth2?
=+------------+------------+---------------+----------------+
h px� 
X
%s*synth2@
>|rgb_to_gray | mem        | 16384x8       | LUT            | 
h px� 
X
%s*synth2@
>|rgb_to_gray | p_0_out    | 16384x8       | LUT            | 
h px� 
X
%s*synth2@
>+------------+------------+---------------+----------------+

h px� 
�
%s*synth2�
�---------------------------------------------------------------------------------
Finished ROM, RAM, DSP, Shift Register and Retiming Reporting
h px� 
l
%s*synth2T
R---------------------------------------------------------------------------------
h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
4
%s
*synth2
Start Timing Optimization
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
�
%s*synth2
}Finished Timing Optimization : Time (s): cpu = 00:00:38 ; elapsed = 00:00:38 . Memory (MB): peak = 1465.207 ; gain = 833.211
h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
3
%s
*synth2
Start Technology Mapping
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
�
%s*synth2~
|Finished Technology Mapping : Time (s): cpu = 00:00:38 ; elapsed = 00:00:38 . Memory (MB): peak = 1465.207 ; gain = 833.211
h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
-
%s
*synth2
Start IO Insertion
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
?
%s
*synth2'
%Start Flattening Before IO Insertion
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
B
%s
*synth2*
(Finished Flattening Before IO Insertion
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
6
%s
*synth2
Start Final Netlist Cleanup
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
9
%s
*synth2!
Finished Final Netlist Cleanup
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
�
%s*synth2x
vFinished IO Insertion : Time (s): cpu = 00:00:41 ; elapsed = 00:00:42 . Memory (MB): peak = 1587.090 ; gain = 955.094
h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
=
%s
*synth2%
#Start Renaming Generated Instances
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
�
%s*synth2�
�Finished Renaming Generated Instances : Time (s): cpu = 00:00:41 ; elapsed = 00:00:42 . Memory (MB): peak = 1587.090 ; gain = 955.094
h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
:
%s
*synth2"
 Start Rebuilding User Hierarchy
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
�
%s*synth2�
�Finished Rebuilding User Hierarchy : Time (s): cpu = 00:00:41 ; elapsed = 00:00:42 . Memory (MB): peak = 1587.090 ; gain = 955.094
h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
9
%s
*synth2!
Start Renaming Generated Ports
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
�
%s*synth2�
�Finished Renaming Generated Ports : Time (s): cpu = 00:00:41 ; elapsed = 00:00:42 . Memory (MB): peak = 1587.090 ; gain = 955.094
h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
;
%s
*synth2#
!Start Handling Custom Attributes
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
�
%s*synth2�
�Finished Handling Custom Attributes : Time (s): cpu = 00:00:41 ; elapsed = 00:00:42 . Memory (MB): peak = 1587.090 ; gain = 955.094
h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
8
%s
*synth2 
Start Renaming Generated Nets
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
�
%s*synth2�
�Finished Renaming Generated Nets : Time (s): cpu = 00:00:41 ; elapsed = 00:00:42 . Memory (MB): peak = 1587.090 ; gain = 955.094
h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
9
%s
*synth2!
Start Writing Synthesis Report
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
/
%s
*synth2

Report BlackBoxes: 
h p
x
� 
8
%s
*synth2 
+-+--------------+----------+
h p
x
� 
8
%s
*synth2 
| |BlackBox name |Instances |
h p
x
� 
8
%s
*synth2 
+-+--------------+----------+
h p
x
� 
8
%s
*synth2 
+-+--------------+----------+
h p
x
� 
/
%s*synth2

Report Cell Usage: 
h px� 
+
%s*synth2
+-+-----+------+
h px� 
+
%s*synth2
| |Cell |Count |
h px� 
+
%s*synth2
+-+-----+------+
h px� 
+
%s*synth2
+-+-----+------+
h px� 
3
%s
*synth2

Report Instance Areas: 
h p
x
� 
<
%s
*synth2$
"+------+---------+-------+------+
h p
x
� 
<
%s
*synth2$
"|      |Instance |Module |Cells |
h p
x
� 
<
%s
*synth2$
"+------+---------+-------+------+
h p
x
� 
<
%s
*synth2$
"|1     |top      |       |     0|
h p
x
� 
<
%s
*synth2$
"+------+---------+-------+------+
h p
x
� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
�
%s*synth2�
�Finished Writing Synthesis Report : Time (s): cpu = 00:00:41 ; elapsed = 00:00:42 . Memory (MB): peak = 1587.090 ; gain = 955.094
h px� 
l
%s
*synth2T
R---------------------------------------------------------------------------------
h p
x
� 
a
%s
*synth2I
GSynthesis finished with 0 errors, 0 critical warnings and 40 warnings.
h p
x
� 
�
%s
*synth2�
Synthesis Optimization Runtime : Time (s): cpu = 00:00:41 ; elapsed = 00:00:42 . Memory (MB): peak = 1587.090 ; gain = 955.094
h p
x
� 
�
%s
*synth2�
�Synthesis Optimization Complete : Time (s): cpu = 00:00:41 ; elapsed = 00:00:42 . Memory (MB): peak = 1587.090 ; gain = 955.094
h p
x
� 
B
 Translating synthesized netlist
350*projectZ1-571h px� 
�
I%sTime (s): cpu = %s ; elapsed = %s . Memory (MB): peak = %s ; gain = %s
268*common2
Netlist sorting complete. 2

00:00:002

00:00:002

1588.1252
0.000Z17-268h px� 
K
)Preparing netlist for logic optimization
349*projectZ1-570h px� 
Q
)Pushed %s inverter(s) to %s load pin(s).
98*opt2
02
0Z31-138h px� 
�
I%sTime (s): cpu = %s ; elapsed = %s . Memory (MB): peak = %s ; gain = %s
268*common2
Netlist sorting complete. 2

00:00:002

00:00:002

1708.5942
0.000Z17-268h px� 
l
!Unisim Transformation Summary:
%s111*project2'
%No Unisim elements were transformed.
Z1-111h px� 
V
%Synth Design complete | Checksum: %s
562*	vivadotcl2

180b8faeZ4-1430h px� 
C
Releasing license: %s
83*common2
	SynthesisZ17-83h px� 

G%s Infos, %s Warnings, %s Critical Warnings and %s Errors encountered.
28*	vivadotcl2
272
402
02
0Z4-41h px� 
L
%s completed successfully
29*	vivadotcl2
synth_designZ4-42h px� 
�
I%sTime (s): cpu = %s ; elapsed = %s . Memory (MB): peak = %s ; gain = %s
268*common2
synth_design: 2

00:00:422

00:00:442

1708.5942

1083.812Z17-268h px� 
�
 The %s '%s' has been generated.
621*common2

checkpoint2M
KC:/Users/zolil/Verilog_modules2/Verilog_modules2.runs/synth_1/tb_system.dcpZ17-1381h px� 
�
Executing command : %s
56330*	planAhead2]
[report_utilization -file tb_system_utilization_synth.rpt -pb tb_system_utilization_synth.pbZ12-24828h px� 
\
Exiting %s at %s...
206*common2
Vivado2
Mon May 26 22:45:44 2025Z17-206h px� 


End Record