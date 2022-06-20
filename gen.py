import argparse
import xml.etree.ElementTree as ET
import os
from jinja2 import Environment, FileSystemLoader
import math
import sys
import array as arr 

context = {}
print ('ACTGraph (Courtesy: Jinja 2.0)...')

context['id'] = "P0"
context['XWARE'] = sys.argv[1]
context['EVALUATION_TYPE'] = sys.argv[2]
context['NUM_PEs'] = int(sys.argv[3])
context['TESTKERNEL'] = sys.argv[4]

if context['NUM_PEs'] == 3: 
    context['NUM_PEs'] = 24

awspath = '/home/centos/src/project_data/oj2zf/ActsOfAGraph/gen.py'
is_aws_path = os.path.isfile(awspath)  
print(is_aws_path) 
# quit()
###
 
###

context['VECTOR_SIZE'] = 8
context['VECTOR2_SIZE'] = 16
context['DUMMY'] = 0
context['NUM_PEs_seq'] = []
for i in range (0,context['NUM_PEs']):
		context['NUM_PEs_seq'].append(i)
context['DATAWIDTH'] = 512
context['NUMBITSINKVPAIR'] = 64
context['KERNELNAME'] = "ACTS"
context['NUMCOMPUTEUNITS'] = context['NUM_PEs']
    
print ('Generating sources... ')
print ('XWARE: ' + str(context['XWARE']))
print ('NUM_PEs: ' + str(context['NUM_PEs']))
print ('EVALUATION_TYPE: ' + str(context['EVALUATION_TYPE']))
print ('VECTOR_SIZE: ' + str(context['VECTOR_SIZE']))
print ('TESTKERNEL: ' + str(context['TESTKERNEL']))

###

if ((context['XWARE'] == "HW") or (context['XWARE'] == "SWEMU")):
    context['classname__top_usrcv_nudstv'] = ""
    context['classname__top_usrcv_nudstv'] = ""
    context['classname__processedgesu'] = ""
    context['classname__processedges'] = ""
    context['classname__partitionupdates'] = ""
    context['classname__reduceupdates'] = ""
    context['classname__mem_access'] = ""
    context['classname__mem_access'] = ""
    context['classname__mem_convert_and_access'] = "" 
    context['classname__acts_util'] = ""
    context['classname__algofuncs'] = ""
    context['classname__acts'] = ""
    context['classname__acts_merge'] = ""
    context['classname__acts_merge'] = ""
else:
    context['classname__top_usrcv_nudstv'] = "acts_all::"
    context['classname__top_usrcv_nudstv'] = "acts_all::"
    context['classname__processedges_and_reduceupdates'] = "acts_all::"
    context['classname__processedgesu'] = "acts_all::"
    context['classname__processedges'] = "acts_all::"
    context['classname__partitionupdates'] = "acts_all::"
    context['classname__reduceupdates'] = "acts_all::"
    context['classname__mem_access'] = "acts_all::"
    context['classname__mem_access'] = "acts_all::" 
    context['classname__mem_convert_and_access'] = "acts_all::" 
    context['classname__acts_util'] = "acts_all::"
    context['classname__algofuncs'] = "acts_all::"
    context['classname__acts'] = "acts_all::"
    context['classname__acts_merge'] = "acts_all::"
    context['classname__acts_merge'] = "acts_all::"

###

# cwd = os.getcwd()
# print (cwd)
# if (cwd=="/home/oj2zf/Documents/ActsOfAGraph/ztest_portal/"):
    # print ('gen.py: YYYYYYYYYYYYYYYEEEEEEEEEESSSSSSSSSSSSSSSS...')
    # relref="../"
# else:
    # print ('gen.py: NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO...')
    # relref=""
# relref="../"
relref=""

o_path0=relref+"acts/acts.cpp"
o_path1=relref+"acts/acts.cpp"
o_path2=relref+"acts/acts3.cpp"
o_path3=relref+"acts/acts3.cpp"
o_path4=relref+"include/common.h"
o_path5=relref+"include/common.h"
o_path6=relref+"include/common.h"
o_path7=relref+"include/common.h"
o_path8=relref+"acts/trad_gp.cpp"
o_path9=relref+"acts/trad_gp.h"
o_path10=relref+"acts/top_usrcv_nudstv.cpp"
o_path11=relref+"acts/top_usrcv_nudstv.cpp"
o_path12=relref+"acts/top_usrcv_nudstv.cpp"
o_path13=relref+"acts/top_usrcv_nudstv.cpp"
o_path14=relref+"acts/processedges.cpp"
o_path15=relref+"acts/processedges.cpp"
o_path16=relref+"acts/processedges.cpp"
o_path17=relref+"acts/acts.cpp"
o_path18=relref+"acts/reduceupdates.cpp"
o_path19=relref+"acts/reduceupdates.cpp"
o_path20=relref+"acts/acts_util.cpp"
o_path21=relref+"acts/algo_funcs.cpp"
o_path22=relref+"acts/reduceupdates.cpp"
o_path23=relref+"acts/reduceupdates.cpp"
o_path24=relref+"acts/acts.cpp"
o_path25=relref+"acts/acts.cpp"
o_path26=relref+"acts/partitionupdates.cpp"
o_path27=relref+"acts/partitionupdates.cpp"
o_path28=relref+"acts/acts.cpp"
o_path29=relref+"acts/acts.cpp"
o_path30=relref+"acts/mem_access.cpp"
o_path31=relref+"acts/mem_access.cpp"
o_path32=relref+"acts/acts_merge.cpp"
o_path33=relref+"acts/acts_merge.cpp"
o_path34=relref+"acts/acts_all.cpp"
o_path35=relref+"acts/acts_all.h"
o_path36=relref+"acts/mem_convert_and_access.cpp"
o_path37=relref+"acts/mem_convert_and_access.cpp"
o_path100=relref+"acts/top_usrcv_nudstv_mf.h"
o_path101=relref+"acts/top_usrcv_nudstv_mf.h"
o_path102=relref+"acts/processedges_mf.h"
o_path103=relref+"acts/acts_mf.h"
o_path104=relref+"acts/reduceupdates_mf.h"
o_path105=relref+"acts/acts_util_mf.h"
o_path106=relref+"acts/reduceupdates_mf.h"
o_path107=relref+"acts/partitionupdates_mf.h"
o_path108=relref+"acts/processedges_mf.h"
o_path109=relref+"acts/mem_access_mf.h"
o_path110=relref+"acts/acts_merge_mf.h"
o_path111=relref+"acts/mem_convert_and_access_mf.h"
o_path112=relref+"acts/acts3_mf.h"
o_path113=relref+"acts/algo_funcs_mf.h"
o_path200=relref+"acts/acts_allP0.cpp"
o_path201=relref+"acts/acts_allP0.h"
o_path202=relref+"acts/acts_allP1.cpp"
o_path203=relref+"acts/acts_allP1.h"
o_path204=relref+"acts/acts_allP2.cpp"
o_path205=relref+"acts/acts_allP2.h"
o_path206=relref+"acts/acts_allS.cpp"
o_path207=relref+"acts/acts_allS.h"

out_path0=os.path.abspath(o_path0)
out_path1=os.path.abspath(o_path1)
out_path2=os.path.abspath(o_path2)
out_path3=os.path.abspath(o_path3)
out_path4=os.path.abspath(o_path4)
out_path5=os.path.abspath(o_path5)
out_path6=os.path.abspath(o_path6)
out_path7=os.path.abspath(o_path7)
out_path8=os.path.abspath(o_path8)
out_path9=os.path.abspath(o_path9)
out_path10=os.path.abspath(o_path10)
out_path11=os.path.abspath(o_path11)
out_path12=os.path.abspath(o_path12)
out_path13=os.path.abspath(o_path13)
out_path14=os.path.abspath(o_path14)
out_path15=os.path.abspath(o_path15)
out_path16=os.path.abspath(o_path16)
out_path17=os.path.abspath(o_path17)
out_path18=os.path.abspath(o_path18)
out_path19=os.path.abspath(o_path19)
out_path20=os.path.abspath(o_path20)
out_path21=os.path.abspath(o_path21)
out_path22=os.path.abspath(o_path22)
out_path23=os.path.abspath(o_path23)
out_path24=os.path.abspath(o_path24)
out_path25=os.path.abspath(o_path25)
out_path26=os.path.abspath(o_path26)
out_path27=os.path.abspath(o_path27)
out_path28=os.path.abspath(o_path28)
out_path29=os.path.abspath(o_path29)
out_path30=os.path.abspath(o_path30)
out_path31=os.path.abspath(o_path31)
out_path32=os.path.abspath(o_path32)
out_path33=os.path.abspath(o_path33)
out_path34=os.path.abspath(o_path34)
out_path35=os.path.abspath(o_path35)
out_path36=os.path.abspath(o_path36)
out_path37=os.path.abspath(o_path37)
out_path100=os.path.abspath(o_path100)
out_path101=os.path.abspath(o_path101)
out_path102=os.path.abspath(o_path102)
out_path103=os.path.abspath(o_path103)
out_path104=os.path.abspath(o_path104)
out_path105=os.path.abspath(o_path105)
out_path106=os.path.abspath(o_path106)
out_path107=os.path.abspath(o_path107)
out_path108=os.path.abspath(o_path108)
out_path109=os.path.abspath(o_path109)
out_path110=os.path.abspath(o_path110)
out_path111=os.path.abspath(o_path111)
out_path112=os.path.abspath(o_path112)
out_path113=os.path.abspath(o_path113)
out_path200=os.path.abspath(o_path200)
out_path201=os.path.abspath(o_path201)
out_path202=os.path.abspath(o_path202)
out_path203=os.path.abspath(o_path203)
out_path204=os.path.abspath(o_path204)
out_path205=os.path.abspath(o_path205)
out_path206=os.path.abspath(o_path206)
out_path207=os.path.abspath(o_path207)

templ_path0=relref+"acts_templates"
templ_path1=relref+"acts_templates"
templ_path2=relref+"acts_templates"
templ_path3=relref+"acts_templates"
templ_path4=relref+"include/"
templ_path5=relref+"include/"
templ_path6=relref+"include/"
templ_path7=relref+"include/"
templ_path8=relref+"acts_templates"
templ_path9=relref+"acts_templates"
templ_path10=relref+"acts_templates"
templ_path11=relref+"acts_templates"
templ_path12=relref+"acts_templates"
templ_path13=relref+"acts_templates"
templ_path14=relref+"acts_templates"
templ_path15=relref+"acts_templates"
templ_path16=relref+"acts_templates"
templ_path17=relref+"acts_templates"
templ_path18=relref+"acts_templates"
templ_path19=relref+"acts_templates"
templ_path20=relref+"acts_templates"
templ_path21=relref+"acts_templates"
templ_path22=relref+"acts_templates"
templ_path23=relref+"acts_templates"
templ_path24=relref+"acts_templates"
templ_path25=relref+"acts_templates"
templ_path26=relref+"acts_templates"
templ_path27=relref+"acts_templates"
templ_path28=relref+"acts_templates"
templ_path29=relref+"acts_templates"
templ_path30=relref+"acts_templates"
templ_path31=relref+"acts_templates"
templ_path32=relref+"acts_templates"
templ_path33=relref+"acts_templates"
templ_path34=relref+"acts_templates"
templ_path35=relref+"acts_templates"
templ_path36=relref+"acts_templates"
templ_path37=relref+"acts_templates"
templ_path100=relref+"acts_templates"
templ_path101=relref+"acts_templates"
templ_path102=relref+"acts_templates"
templ_path103=relref+"acts_templates"
templ_path104=relref+"acts_templates"
templ_path105=relref+"acts_templates"
templ_path106=relref+"acts_templates"
templ_path107=relref+"acts_templates"
templ_path108=relref+"acts_templates"
templ_path109=relref+"acts_templates"
templ_path110=relref+"acts_templates"
templ_path111=relref+"acts_templates"
templ_path112=relref+"acts_templates"
templ_path113=relref+"acts_templates"
templ_path200=relref+"acts_templates"
templ_path201=relref+"acts_templates"
templ_path202=relref+"acts_templates"
templ_path203=relref+"acts_templates"
templ_path204=relref+"acts_templates"
templ_path205=relref+"acts_templates"
templ_path206=relref+"acts_templates"
templ_path207=relref+"acts_templates"

###
context['1_seq'] = []
for i in range (0,1):
		context['1_seq'].append(i)

context['VECTOR_SIZE_seq'] = []
for i in range (0,(context['VECTOR_SIZE'])):
		context['VECTOR_SIZE_seq'].append(i)
        
context['VECTOR2_SIZE_seq'] = []
for i in range (0,(context['VECTOR2_SIZE'])):
		context['VECTOR2_SIZE_seq'].append(i)
		
context['VECTOR_SIZEPLUS1_seq'] = []
for i in range (1,(context['VECTOR_SIZE'])):
		context['VECTOR_SIZEPLUS1_seq'].append(i)

###
context['VECTOR2_SIZE_F_seq'] = []
for i in range (0,8):
		context['VECTOR2_SIZE_F_seq'].append(i)
        
context['VECTOR2_SIZE_S_seq'] = []
for i in range (8,context['VECTOR2_SIZE']):
		context['VECTOR2_SIZE_S_seq'].append(i)

###
# context['NUM_PARTITIONS_seq'] = []
# for i in range (0,(context['NUM_PARTITIONS'])):
		# context['NUM_PARTITIONS_seq'].append(i)
context['BRAMwidth_seq'] = []
for i in range (0,16):
		context['BRAMwidth_seq'].append(i)

context['64_seq'] = []
for i in range (0,64):
		context['64_seq'].append(i)
        
context['32_seq'] = []
for i in range (0,32):
		context['32_seq'].append(i)
        
context['24_seq'] = []
for i in range (0,24):
		context['24_seq'].append(i)
        
context['16_seq'] = []
for i in range (0,16):
		context['16_seq'].append(i)
        
context['15_seq'] = []
for i in range (0,15):
		context['15_seq'].append(i)
        
context['12_seq'] = []
for i in range (0,12):
		context['12_seq'].append(i)
        
context['T_val'] = 13
context['T_seq'] = []
for i in range (0,(context['T_val'])):
		context['T_seq'].append(i)
context['T_charseq'] = []
for i in range (0,(context['T_val'])):
		c = chr(ord('A') + i)
		context['T_charseq'].append(c)
        
context['Tplus2_seq'] = []
for i in range (0,(context['T_val']+2)):
		context['Tplus2_seq'].append(i)
context['Tplus2_charseq'] = []
for i in range (0,(context['T_val']+2)):
		c = chr(ord('A') + i)
		context['Tplus2_charseq'].append(c)
        
context['10_seq'] = []
for i in range (0,10):
		context['10_seq'].append(i)
context['10_charseq'] = []
for i in range (0,10):
		c = chr(ord('A') + i)
		context['10_charseq'].append(c)
        
context['8_seq'] = []
for i in range (0,8):
		context['8_seq'].append(i)
        
context['7_seq'] = []
for i in range (0,7):
		context['7_seq'].append(i)
        
context['6_seq'] = []
for i in range (0,6):
		context['6_seq'].append(i)
        
context['5_seq'] = []
for i in range (0,5):
		context['5_seq'].append(i)
        
context['4_seq'] = []
for i in range (0,4):
		context['4_seq'].append(i)
        
context['3_seq'] = []
for i in range (0,3):
		context['3_seq'].append(i)
        
context['2_seq'] = []
for i in range (0,2):
		context['2_seq'].append(i)
        
context['1_seq'] = []
for i in range (0,1):
		context['1_seq'].append(i)
        
context['k_seq'] = []
for i in range (0,2):
		context['k_seq'].append(i)
 
env0 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path0)), trim_blocks=True, lstrip_blocks=True)
env1 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path1)), trim_blocks=True, lstrip_blocks=True)
env2 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path2)), trim_blocks=True, lstrip_blocks=True)
env3 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path3)), trim_blocks=True, lstrip_blocks=True)
env4 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path4)), trim_blocks=True, lstrip_blocks=True)
env5 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path5)), trim_blocks=True, lstrip_blocks=True)
env6 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path6)), trim_blocks=True, lstrip_blocks=True)
env7 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path7)), trim_blocks=True, lstrip_blocks=True)
env8 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path8)), trim_blocks=True, lstrip_blocks=True)
env9 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path9)), trim_blocks=True, lstrip_blocks=True)
env10 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path10)), trim_blocks=True, lstrip_blocks=True)
env11 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path11)), trim_blocks=True, lstrip_blocks=True)
env12 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path12)), trim_blocks=True, lstrip_blocks=True)
env13 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path13)), trim_blocks=True, lstrip_blocks=True)
env14 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path14)), trim_blocks=True, lstrip_blocks=True)
env15 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path15)), trim_blocks=True, lstrip_blocks=True)
env16 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path16)), trim_blocks=True, lstrip_blocks=True)
env17 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path17)), trim_blocks=True, lstrip_blocks=True)
env18 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path18)), trim_blocks=True, lstrip_blocks=True)
env19 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path19)), trim_blocks=True, lstrip_blocks=True)
env20 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path20)), trim_blocks=True, lstrip_blocks=True)
env21 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path21)), trim_blocks=True, lstrip_blocks=True)
env22 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path22)), trim_blocks=True, lstrip_blocks=True)
env23 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path23)), trim_blocks=True, lstrip_blocks=True)
env24 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path24)), trim_blocks=True, lstrip_blocks=True)
env25 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path25)), trim_blocks=True, lstrip_blocks=True)
env26 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path26)), trim_blocks=True, lstrip_blocks=True)
env27 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path27)), trim_blocks=True, lstrip_blocks=True)
env28 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path28)), trim_blocks=True, lstrip_blocks=True)
env29 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path29)), trim_blocks=True, lstrip_blocks=True)
env30 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path30)), trim_blocks=True, lstrip_blocks=True)
env31 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path31)), trim_blocks=True, lstrip_blocks=True)
env32 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path32)), trim_blocks=True, lstrip_blocks=True)
env33 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path33)), trim_blocks=True, lstrip_blocks=True)
env34 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path34)), trim_blocks=True, lstrip_blocks=True)
env35 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path35)), trim_blocks=True, lstrip_blocks=True)
env36 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path36)), trim_blocks=True, lstrip_blocks=True)
env37 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path37)), trim_blocks=True, lstrip_blocks=True)
env100 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path100)), trim_blocks=True, lstrip_blocks=True)
env101 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path101)), trim_blocks=True, lstrip_blocks=True)
env102 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path102)), trim_blocks=True, lstrip_blocks=True)
env103 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path103)), trim_blocks=True, lstrip_blocks=True)
env104 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path104)), trim_blocks=True, lstrip_blocks=True)
env105 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path105)), trim_blocks=True, lstrip_blocks=True)
env106 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path106)), trim_blocks=True, lstrip_blocks=True)
env107 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path107)), trim_blocks=True, lstrip_blocks=True)
env108 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path108)), trim_blocks=True, lstrip_blocks=True)
env109 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path109)), trim_blocks=True, lstrip_blocks=True)
env110 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path110)), trim_blocks=True, lstrip_blocks=True)
env111 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path111)), trim_blocks=True, lstrip_blocks=True)
env112 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path112)), trim_blocks=True, lstrip_blocks=True)
env113 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path113)), trim_blocks=True, lstrip_blocks=True)
env200 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path200)), trim_blocks=True, lstrip_blocks=True)
env201 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path201)), trim_blocks=True, lstrip_blocks=True)
env202 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path202)), trim_blocks=True, lstrip_blocks=True)
env203 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path203)), trim_blocks=True, lstrip_blocks=True)
env204 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path204)), trim_blocks=True, lstrip_blocks=True)
env205 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path205)), trim_blocks=True, lstrip_blocks=True)
env206 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path206)), trim_blocks=True, lstrip_blocks=True)
env207 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path207)), trim_blocks=True, lstrip_blocks=True)

env0.globals.update(zip=zip)
env1.globals.update(zip=zip)
env2.globals.update(zip=zip)
env3.globals.update(zip=zip)
env4.globals.update(zip=zip)
env5.globals.update(zip=zip)
env6.globals.update(zip=zip)
env7.globals.update(zip=zip)
env8.globals.update(zip=zip)
env9.globals.update(zip=zip)
env10.globals.update(zip=zip)
env11.globals.update(zip=zip)
env12.globals.update(zip=zip)
env13.globals.update(zip=zip)
env14.globals.update(zip=zip)
env15.globals.update(zip=zip)
env16.globals.update(zip=zip)
env17.globals.update(zip=zip)
env18.globals.update(zip=zip)
env19.globals.update(zip=zip)
env20.globals.update(zip=zip)
env21.globals.update(zip=zip)
env22.globals.update(zip=zip)
env23.globals.update(zip=zip)
env24.globals.update(zip=zip)
env25.globals.update(zip=zip)
env26.globals.update(zip=zip)
env27.globals.update(zip=zip)
env28.globals.update(zip=zip)
env29.globals.update(zip=zip)
env30.globals.update(zip=zip)
env31.globals.update(zip=zip)
env32.globals.update(zip=zip)
env33.globals.update(zip=zip)
env34.globals.update(zip=zip)
env35.globals.update(zip=zip)
env36.globals.update(zip=zip)
env37.globals.update(zip=zip)
env100.globals.update(zip=zip)
env101.globals.update(zip=zip)
env102.globals.update(zip=zip)
env103.globals.update(zip=zip)
env104.globals.update(zip=zip)
env105.globals.update(zip=zip)
env106.globals.update(zip=zip)
env107.globals.update(zip=zip)
env108.globals.update(zip=zip)
env109.globals.update(zip=zip)
env110.globals.update(zip=zip)
env111.globals.update(zip=zip)
env112.globals.update(zip=zip)
env113.globals.update(zip=zip)
env200.globals.update(zip=zip)
env201.globals.update(zip=zip)
env202.globals.update(zip=zip)
env203.globals.update(zip=zip)
env204.globals.update(zip=zip)
env205.globals.update(zip=zip)
env206.globals.update(zip=zip)
env207.globals.update(zip=zip)

template0 = env0.get_template('acts.template')
template1 = env1.get_template('acts.template')
template2 = env2.get_template('acts3.template')
template3 = env3.get_template('acts3.template')
template4 = env4.get_template('common_h.template')
template5 = env5.get_template('common_h.template')
template6 = env6.get_template('common_h.template')
template7 = env7.get_template('common_h.template')
template8 = env8.get_template('trad_gp.template')
template9 = env9.get_template('trad_gp_mf.template')
template10 = env10.get_template('top_usrcv_nudstv.template')
template11 = env11.get_template('top_usrcv_nudstv.template')
template12 = env12.get_template('top_usrcv_nudstv.template')
template13 = env13.get_template('top_usrcv_nudstv.template')
template14 = env14.get_template('processedges.template')
template15 = env15.get_template('processedges.template')
template16 = env16.get_template('processedges.template')
template17 = env17.get_template('acts.template')
template18 = env18.get_template('reduceupdates.template')
template19 = env19.get_template('reduceupdates.template')
template20 = env20.get_template('acts_util.template')
template21 = env21.get_template('algo_funcs.template')
template22 = env22.get_template('reduceupdates.template')
template23 = env23.get_template('reduceupdates.template')
template24 = env24.get_template('acts.template')
template25 = env25.get_template('acts.template')
template26 = env26.get_template('partitionupdates.template')
template27 = env27.get_template('partitionupdates.template')
template28 = env28.get_template('acts.template')
template29 = env29.get_template('acts.template')
template30 = env30.get_template('mem_access.template')
template31 = env31.get_template('mem_access.template')
template32 = env32.get_template('acts_merge.template')
template33 = env33.get_template('acts_merge.template')
template34 = env34.get_template('acts_all.template')
template35 = env35.get_template('acts_all_h.template')
template36 = env36.get_template('mem_convert_and_access.template')
template37 = env37.get_template('mem_convert_and_access.template')
template100 = env100.get_template('top_usrcv_nudstv_mf.template')
template101 = env101.get_template('top_usrcv_nudstv_mf.template')
template102 = env102.get_template('processedges_mf.template')
template103 = env103.get_template('acts_mf.template')
template104 = env104.get_template('reduceupdates_mf.template')
template105 = env105.get_template('acts_util_mf.template')
template106 = env106.get_template('reduceupdates_mf.template')
template107 = env107.get_template('partitionupdates_mf.template')
template108 = env108.get_template('processedges_mf.template')
template109 = env109.get_template('mem_access_mf.template')
template110 = env110.get_template('acts_merge_mf.template')
template111 = env111.get_template('mem_convert_and_access_mf.template')
template112 = env112.get_template('acts3_mf.template')
template113 = env113.get_template('algo_funcs_mf.template')
template200 = env200.get_template('acts_all.template')
template201 = env201.get_template('acts_all_h.template')
template202 = env202.get_template('acts_all.template')
template203 = env203.get_template('acts_all_h.template')
template204 = env204.get_template('acts_all.template')
template205 = env205.get_template('acts_all_h.template')
template206 = env206.get_template('acts_all.template')
template207 = env207.get_template('acts_all_h.template')

rendered_file0 = template0.render(context=context)
rendered_file1 = template1.render(context=context)
rendered_file2 = template2.render(context=context)
rendered_file3 = template3.render(context=context)
rendered_file4 = template4.render(context=context)
rendered_file5 = template5.render(context=context)
rendered_file6 = template6.render(context=context)
rendered_file7 = template7.render(context=context)
rendered_file8 = template8.render(context=context)
rendered_file9 = template9.render(context=context)
rendered_file10 = template10.render(context=context)
rendered_file11 = template11.render(context=context)
rendered_file12 = template12.render(context=context)
rendered_file13 = template13.render(context=context)
rendered_file14 = template14.render(context=context)
rendered_file15 = template15.render(context=context)
rendered_file16 = template16.render(context=context)
rendered_file17 = template17.render(context=context)
rendered_file18 = template18.render(context=context)
rendered_file19 = template19.render(context=context)
rendered_file20 = template20.render(context=context)
rendered_file21 = template21.render(context=context)
rendered_file22 = template22.render(context=context)
rendered_file23 = template23.render(context=context)
rendered_file24 = template24.render(context=context)
rendered_file25 = template25.render(context=context)
rendered_file26 = template26.render(context=context)
rendered_file27 = template27.render(context=context)
rendered_file28 = template28.render(context=context)
rendered_file29 = template29.render(context=context)
rendered_file30 = template30.render(context=context)
rendered_file31 = template31.render(context=context)
rendered_file32 = template32.render(context=context)
rendered_file33 = template33.render(context=context)
rendered_file34 = template34.render(context=context)
rendered_file35 = template35.render(context=context)
rendered_file36 = template36.render(context=context)
rendered_file37 = template37.render(context=context)
rendered_file100 = template100.render(context=context)
rendered_file101 = template101.render(context=context)
rendered_file102 = template102.render(context=context)
rendered_file103 = template103.render(context=context)
rendered_file104 = template104.render(context=context)
rendered_file105 = template105.render(context=context)
rendered_file106 = template106.render(context=context)
rendered_file107 = template107.render(context=context)
rendered_file108 = template108.render(context=context)
rendered_file109 = template109.render(context=context)
rendered_file110 = template110.render(context=context)
rendered_file111 = template111.render(context=context)
rendered_file112 = template112.render(context=context)
rendered_file113 = template113.render(context=context)
context['id'] = "P0"
rendered_file200 = template200.render(context=context)
rendered_file201 = template201.render(context=context)
context['id'] = "P1"
rendered_file202 = template202.render(context=context)
rendered_file203 = template203.render(context=context)
context['id'] = "P2"
rendered_file204 = template204.render(context=context)
rendered_file205 = template205.render(context=context)
context['id'] = "S"
rendered_file206 = template206.render(context=context)
rendered_file207 = template207.render(context=context)

with open(out_path0, 'w') as outFile0:
	outFile0.write(rendered_file0)
with open(out_path1, 'w') as outFile1:
	outFile1.write(rendered_file1)
with open(out_path2, 'w') as outFile2:
	outFile2.write(rendered_file2)
with open(out_path3, 'w') as outFile3:
	outFile3.write(rendered_file3) 
with open(out_path4, 'w') as outFile4:
	outFile4.write(rendered_file4) 
with open(out_path5, 'w') as outFile5:
	outFile5.write(rendered_file5)
with open(out_path6, 'w') as outFile6:
	outFile6.write(rendered_file6)
with open(out_path7, 'w') as outFile7:
	outFile7.write(rendered_file7)
with open(out_path8, 'w') as outFile8:
	outFile8.write(rendered_file8)
with open(out_path9, 'w') as outFile9:
	outFile9.write(rendered_file9)
with open(out_path10, 'w') as outFile10:
	outFile10.write(rendered_file10)
with open(out_path11, 'w') as outFile11:
	outFile11.write(rendered_file11)
with open(out_path12, 'w') as outFile12:
	outFile12.write(rendered_file12)
with open(out_path13, 'w') as outFile13:
	outFile13.write(rendered_file13)
with open(out_path14, 'w') as outFile14:
	outFile14.write(rendered_file14)
with open(out_path15, 'w') as outFile15:
	outFile15.write(rendered_file15)
with open(out_path16, 'w') as outFile16:
	outFile16.write(rendered_file16)
with open(out_path17, 'w') as outFile17:
	outFile17.write(rendered_file17)
with open(out_path18, 'w') as outFile18:
	outFile18.write(rendered_file18)
with open(out_path19, 'w') as outFile19:
	outFile19.write(rendered_file19)
with open(out_path20, 'w') as outFile20:
	outFile20.write(rendered_file20)
with open(out_path21, 'w') as outFile21:
	outFile21.write(rendered_file21)
with open(out_path22, 'w') as outFile22:
	outFile22.write(rendered_file22)
with open(out_path23, 'w') as outFile23:
	outFile23.write(rendered_file23)
with open(out_path24, 'w') as outFile24:
	outFile24.write(rendered_file24)
with open(out_path25, 'w') as outFile25:
	outFile25.write(rendered_file25)
with open(out_path26, 'w') as outFile26:
	outFile26.write(rendered_file26)
with open(out_path27, 'w') as outFile27:
	outFile27.write(rendered_file27)
with open(out_path28, 'w') as outFile28:
	outFile28.write(rendered_file28)
with open(out_path29, 'w') as outFile29:
	outFile29.write(rendered_file29)
with open(out_path30, 'w') as outFile30:
	outFile30.write(rendered_file30)
with open(out_path31, 'w') as outFile31:
	outFile31.write(rendered_file31)
with open(out_path32, 'w') as outFile32:
	outFile32.write(rendered_file32)
with open(out_path33, 'w') as outFile33:
	outFile33.write(rendered_file33)
with open(out_path34, 'w') as outFile34:
	outFile34.write(rendered_file34)
with open(out_path35, 'w') as outFile35:
	outFile35.write(rendered_file35)
with open(out_path36, 'w') as outFile36:
	outFile36.write(rendered_file36)
with open(out_path37, 'w') as outFile37:
	outFile37.write(rendered_file37)
with open(out_path100, 'w') as outFile100:
	outFile100.write(rendered_file100)
with open(out_path101, 'w') as outFile101:
	outFile101.write(rendered_file101)
with open(out_path102, 'w') as outFile102:
	outFile102.write(rendered_file102)
with open(out_path103, 'w') as outFile103:
	outFile103.write(rendered_file103) 
with open(out_path104, 'w') as outFile104:
	outFile104.write(rendered_file104) 
with open(out_path105, 'w') as outFile105:
	outFile105.write(rendered_file105)
with open(out_path106, 'w') as outFile106:
	outFile106.write(rendered_file106)
with open(out_path107, 'w') as outFile107:
	outFile107.write(rendered_file107)
with open(out_path108, 'w') as outFile108:
	outFile108.write(rendered_file108)
with open(out_path109, 'w') as outFile109:
	outFile109.write(rendered_file109)
with open(out_path110, 'w') as outFile110:
	outFile110.write(rendered_file110)
with open(out_path111, 'w') as outFile111:
	outFile111.write(rendered_file111)
with open(out_path112, 'w') as outFile112:
	outFile112.write(rendered_file112)
with open(out_path113, 'w') as outFile113:
	outFile113.write(rendered_file113)
with open(out_path200, 'w') as outFile200:
	outFile200.write(rendered_file200)
with open(out_path201, 'w') as outFile201:
	outFile201.write(rendered_file201)
with open(out_path202, 'w') as outFile202:
	outFile202.write(rendered_file202)
with open(out_path203, 'w') as outFile203:
	outFile203.write(rendered_file203) 
with open(out_path204, 'w') as outFile204:
	outFile204.write(rendered_file204) 
with open(out_path205, 'w') as outFile205:
	outFile205.write(rendered_file205)
with open(out_path206, 'w') as outFile206:
	outFile206.write(rendered_file206)
with open(out_path207, 'w') as outFile207:
	outFile207.write(rendered_file207)

print ("successful!")
print ("...")
quit()




