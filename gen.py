import argparse
import xml.etree.ElementTree as ET
import os
from jinja2 import Environment, FileSystemLoader
import math
import sys
import array as arr 

context = {}
print ('ACTGraph (Courtesy: Jinja 2.0)...')
context['XWARE'] = sys.argv[1]
context['SETUP'] = sys.argv[2]
context['ALGORITHM'] = sys.argv[3] 
context['DATASET'] = sys.argv[4]
context['NUMSUPERCPUTHREADS'] = sys.argv[5]
context['NUMCPUTHREADS'] = sys.argv[6] # FIXME. this is overriden in common.h
context['NUMSUBCPUTHREADS'] = int(sys.argv[7])
context['NUM_PARTITIONS_POW'] = int(sys.argv[8])
context['LOCKE'] = sys.argv[9]
context['EVALUATION_TYPE'] = sys.argv[10]
context['EVALUATION_PARAM0'] = int(sys.argv[11])
context['NUM_PEs'] = int(sys.argv[12])
path = '/home/centos/src/project_data/oj2zf/ActsOfAGraph/gen.py'
isFile = os.path.isfile(path)  
print(isFile) 

###

if ((context['EVALUATION_TYPE'] != "EV_CREATENDGRAPH") and ((context['DATASET'] == "_RMAT_RANGE0") or (context['DATASET'] == "_RMAT_RANGE1") or (context['DATASET'] == "_RMAT_RANGE2") or (context['DATASET'] == "_RMAT_RANGE3") or (context['DATASET'] == "_RMAT_RANGE4") or (context['DATASET'] == "_RMAT_RANGE5") or (context['DATASET'] == "_RMAT_RANGE6") or (context['DATASET'] == "_RMAT_RANGE7"))):                                     
    context['NUMSUBCPUTHREADS']=1
    
###

context['VECTOR_SIZE'] = 8
context['VECTOR2_SIZE'] = 16
context['DUMMY'] = 0
context['NUMSUBWORKERS'] = 1 # 3#4

context['NUM_EDGE_BANKS'] = 0
context['EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM'] = 0

# context['NUM_EDGE_BANKS'] = 1
# context['EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM'] = 1

# if (context['NUM_PEs'] >= 0) and (context['NUM_PEs'] < 16):
   # context['NUM_EDGE_BANKS'] = 1
   # context['EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM'] = 1
# elif (context['NUM_PEs'] >= 16) and (context['NUM_PEs'] < 64):
   # context['NUM_EDGE_BANKS'] = 0
   # context['EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM'] = 0
# else:
   # print ('gen.py: NOT YET IMPLEMENTED 32... EXITING...')
   # quit()
    
# if context['NUM_PEs'] == 1:
   # context['NUM_EDGE_BANKS'] = 5
   # context['EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM'] = 1
# elif context['NUM_PEs'] == 2:
   # context['NUM_EDGE_BANKS'] = 3
   # context['EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM'] = 1
# elif context['NUM_PEs'] == 3:
   # context['NUM_EDGE_BANKS'] = 2
   # context['EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM'] = 1
# elif context['NUM_PEs'] == 4:
   # context['NUM_EDGE_BANKS'] = 2
   # context['EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM'] = 1
# elif (context['NUM_PEs'] >= 5) and (context['NUM_PEs'] < 16):
   # context['NUM_EDGE_BANKS'] = 1
   # context['EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM'] = 1
# elif (context['NUM_PEs'] >= 16):
   # context['NUM_EDGE_BANKS'] = 0
   # context['EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM'] = 0
# else:
   # print ('gen.py: NOT YET IMPLEMENTED 32... EXITING...')
   # quit()

context['NUM_PEs_seq'] = []
for i in range (0,context['NUM_PEs']):
		context['NUM_PEs_seq'].append(i)
    
###

EV_PERFORMANCEOFALGORITHM = [0, 1, 2, 3, 4]
if context['EVALUATION_TYPE'] == "EV_PERFORMANCEOFALGORITHM":
    context['VECTOR_SIZE'] = 8
        
EV_IMPACTOFRANGE = [0, 2, 4, 8, 12, 16]
if context['EVALUATION_TYPE'] == "EV_IMPACTOFRANGE":
    context['VECTOR_SIZE'] = 8

EV_IMPACTOFPARTITIONFANOUT = [3, 4, 5, 6, 7, 8]
if context['EVALUATION_TYPE'] == "EV_IMPACTOFPARTITIONFANOUT":
    context['NUM_PARTITIONS_POW'] = EV_IMPACTOFPARTITIONFANOUT[context['EVALUATION_PARAM0']]
    
EV_IMPACTOFBANDWIDTH = [1, 2, 4, 6, 8, 8]
if context['EVALUATION_TYPE'] == "EV_IMPACTOFBANDWIDTH": 
    context['VECTOR_SIZE'] = EV_IMPACTOFBANDWIDTH[context['EVALUATION_PARAM0']]
    
EV_IMPACTOFPLATFORM = [1, 2, 4, 8, 12, 16]
if context['EVALUATION_TYPE'] == "EV_IMPACTOFPLATFORM": 
    context['LOCKE'] = "NOLOCKE"
    
EV_IMPACTOFNUMSUBWORKERS = [1, 2, 4, 8, 12, 16]
if context['EVALUATION_TYPE'] == "EV_IMPACTOFNUMSUBWORKERS": 
    context['NUMSUBWORKERS'] = EV_IMPACTOFNUMSUBWORKERS[context['EVALUATION_PARAM0']]
    
EV_CREATENDGRAPH = [0, 1, 2, 3, 4]
if context['EVALUATION_TYPE'] == "EV_CREATENDGRAPH":
    context['VECTOR_SIZE'] = 8
    
if context['DATASET'] == "_ORKUT_3M_106M": # small dataset
    context['KVDATA_RANGE_POW'] = 22
elif context['DATASET'] == "_HOLLYWOOD_1M_57M":
    context['KVDATA_RANGE_POW'] = 21
elif context['DATASET'] == "_KRON20_1M_45M":
    context['KVDATA_RANGE_POW'] = 20
elif context['DATASET'] == "_KRON21_2M_91M":
    context['KVDATA_RANGE_POW'] = 21
elif context['DATASET'] == "_KRON22_4M_200M":
    context['KVDATA_RANGE_POW'] = 22
    
elif context['DATASET'] == "_RMAT22_SPARSE0":
    context['KVDATA_RANGE_POW'] = 22
elif context['DATASET'] == "_RMAT22_SPARSE1":
    context['KVDATA_RANGE_POW'] = 22
elif context['DATASET'] == "_RMAT22_SPARSE2":
    context['KVDATA_RANGE_POW'] = 22
elif context['DATASET'] == "_RMAT22_SPARSE3":
    context['KVDATA_RANGE_POW'] = 22
elif context['DATASET'] == "_RMAT22_SPARSE4":
    context['KVDATA_RANGE_POW'] = 22
elif context['DATASET'] == "_RMAT22_SPARSE5":
    context['KVDATA_RANGE_POW'] = 22
elif context['DATASET'] == "_RMAT22_SPARSE6":
    context['KVDATA_RANGE_POW'] = 22
elif context['DATASET'] == "_RMAT22_SPARSE7":
    context['KVDATA_RANGE_POW'] = 22
    
elif context['DATASET'] == "_RMAT_RANGE0":
    context['KVDATA_RANGE_POW'] = 20
elif context['DATASET'] == "_RMAT_RANGE1":
    context['KVDATA_RANGE_POW'] = 21
elif context['DATASET'] == "_RMAT_RANGE2":
    context['KVDATA_RANGE_POW'] = 22
elif context['DATASET'] == "_RMAT_RANGE3":
    context['KVDATA_RANGE_POW'] = 23
elif context['DATASET'] == "_RMAT_RANGE4":
    context['KVDATA_RANGE_POW'] = 24
elif context['DATASET'] == "_RMAT_RANGE5":
    context['KVDATA_RANGE_POW'] = 25
elif context['DATASET'] == "_RMAT_RANGE6":
    context['KVDATA_RANGE_POW'] = 26
elif context['DATASET'] == "_RMAT_RANGE7":
    context['KVDATA_RANGE_POW'] = 27
    
elif context['DATASET'] == "_INDOCHINA_7M_194M":
    context['KVDATA_RANGE_POW'] = 23
elif context['DATASET'] == "_RGG_4M_32M":
    context['KVDATA_RANGE_POW'] = 22
elif context['DATASET'] == "_RGG_17M_132M":
    context['KVDATA_RANGE_POW'] = 25
elif context['DATASET'] == "_ROADNET_2M_3M":
    context['KVDATA_RANGE_POW'] = 21
elif context['DATASET'] == "_FLICKR_1M_10M":
    context['KVDATA_RANGE_POW'] = 20
    
elif context['DATASET'] == "_TWITTER_67M": # large dataset
    context['KVDATA_RANGE_POW'] = 26
elif context['DATASET'] == "_MOLIERE2016_33M":
    context['KVDATA_RANGE_POW'] = 25
elif context['DATASET'] == "_LARGEDATASET_67M":
    context['KVDATA_RANGE_POW'] = 26
elif context['DATASET'] == "_LARGEDATASET_268M":
    context['KVDATA_RANGE_POW'] = 28
elif context['DATASET'] == "_LARGEDATASET_1B":
    context['KVDATA_RANGE_POW'] = 30
elif context['DATASET'] == "_LARGEDATASET_4B":
    context['KVDATA_RANGE_POW'] = 32
else:
    context['KVDATA_RANGE_POW'] = 0

###

if (isFile):
    context['PLATFORM'] = "AWS_PLATFORM"
else:
    context['PLATFORM'] = "CRABTREE_PLATFORM"
    
if context['XWARE'] == "SW":
	context['NUMINSTANCES'] = 1
elif context['XWARE'] == "HW":
	context['NUMINSTANCES'] = 1
elif context['XWARE'] == "SWEMU":
	context['NUMINSTANCES'] = 1
else:
	context['NUMINSTANCES'] = 1
    
if (context['PLATFORM'] == "AWS_PLATFORM"):
	context['PROJECT_BASEPATH'] = "/home/centos/src/project_data/oj2zf/ActsOfAFastGraph"
else:
	context['PROJECT_BASEPATH'] = "/home/oj2zf/Documents/ActsOfAFastGraph"
    
context['DATAWIDTH'] = 512
context['NUMBITSINKVPAIR'] = 64
context['NUMDRAMBANKS'] = 4
context['NUMWORKERS'] = 1
context['NUMPEFUNCS'] = 8
context['NUMSUBWORKERSPERVECTOR'] = context['VECTOR_SIZE'] / context['NUMSUBWORKERS']
context['KERNELNAME'] = "ACTS"
# context['NUM_PARTITIONS'] = 2**context['NUM_PARTITIONS_POW']
context['NUMCOMPUTEUNITS'] = context['NUMSUBCPUTHREADS']
    
print ('Generating sources... ')
print ('XWARE: ' + str(context['XWARE']))
print ('SETUP: ' + str(context['SETUP']))
print ('ALGORITHM: ' + str(context['ALGORITHM']))
print ('DATASET: ' + str(context['DATASET']))
print ('NUMSUPERCPUTHREADS: ' + str(context['NUMSUPERCPUTHREADS']))
print ('NUMCPUTHREADS: ' + str(context['NUMCPUTHREADS']))
print ('NUMSUBCPUTHREADS: ' + str(context['NUMSUBCPUTHREADS']))
print ('NUMSUBCPUTHREADS: ' + str(context['NUMSUBCPUTHREADS']))
print ('LOCKE: ' + str(context['LOCKE']))
print ('EVALUATION_TYPE: ' + str(context['EVALUATION_TYPE']))
print ('EVALUATION_PARAM0: ' + str(context['EVALUATION_PARAM0']))
print ('NUM_PEs: ' + str(context['NUM_PEs']))
print ('NUMDRAMBANKS: ' + str(context['NUMDRAMBANKS']))
print ('NUMINSTANCES: ' + str(context['NUMINSTANCES']))
# print ('NUM_PARTITIONS: ' + str(context['NUM_PARTITIONS']))
print ('NUMWORKERS: ' + str(context['NUMWORKERS'])) 
print ('NUMSUBWORKERS: ' + str(context['NUMSUBWORKERS']))
print ('VECTOR_SIZE: ' + str(context['VECTOR_SIZE']))
context['KERNELTYPE'] = "_SINGLEKERNEL"

relref=""
# relref="../"
	
o_path0=relref+"acts/acts/acts.cpp"
o_path1=relref+"acts/acts/acts.h"
o_path2=relref+"acts/acts/actsproc.cpp"
o_path3=relref+"acts/acts/actsproc.h"
o_path4=relref+"include/common.h"
o_path5=relref+"include/common.h"
o_path6=relref+"include/common.h"
o_path7=relref+"include/common.h"
o_path8=relref+"acts/acts/actssync.cpp"
o_path9=relref+"acts/acts/actssync.h"

o_path10=relref+"acts/acts/top_usrcv_udstv.cpp"
o_path11=relref+"acts/acts/top_usrcv_udstv.h"
o_path12=relref+"acts/acts/top_nusrcv_nudstv.cpp"
o_path13=relref+"acts/acts/top_nusrcv_nudstv.h"
o_path14=relref+"acts/acts/processedges_splitdstvxs.cpp"
o_path15=relref+"acts/acts/processedges_splitdstvxs.h"
o_path16=relref+"acts/acts/processedgesu.cpp"
o_path17=relref+"acts/acts/processedgesu.h"
o_path18=relref+"acts/acts/reduceupdates.cpp"
o_path19=relref+"acts/acts/reduceupdates.h"
o_path20=relref+"acts/acts/acts_util.cpp"
o_path21=relref+"acts/acts/acts_util.h"
o_path22=relref+"acts/acts/mem_access.cpp"
o_path23=relref+"acts/acts/mem_access.h"
o_path24=relref+"acts/acts/actsproc.cpp"
o_path25=relref+"acts/acts/actsproc.h"
o_path26=relref+"acts/acts/partitionupdates.cpp"
o_path27=relref+"acts/acts/partitionupdates.h"
o_path28=relref+"acts/acts/acts_merge.cpp"
o_path29=relref+"acts/acts/acts_merge.h"
o_path30=relref+"acts/acts/mem_access_splitdstvxs.cpp"
o_path31=relref+"acts/acts/mem_access_splitdstvxs.h"
o_path32=relref+"acts/acts/acts_merge_splitdstvxs.cpp"
o_path33=relref+"acts/acts/acts_merge_splitdstvxs.h"
o_path34=relref+"acts/acts/acts_all.cpp"
o_path35=relref+"acts/acts/acts_all.h"

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

templ_path0=relref+"acts/acts_templates"
templ_path1=relref+"acts/acts_templates"
templ_path2=relref+"acts/acts_templates"
templ_path3=relref+"acts/acts_templates"
templ_path4=relref+"include/"
templ_path5=relref+"include/"
templ_path6=relref+"include/"
templ_path7=relref+"include/"
templ_path8=relref+"acts/acts_templates"
templ_path9=relref+"acts/acts_templates"
templ_path10=relref+"acts/acts_templates"
templ_path11=relref+"acts/acts_templates"
templ_path12=relref+"acts/acts_templates"
templ_path13=relref+"acts/acts_templates"
templ_path14=relref+"acts/acts_templates"
templ_path15=relref+"acts/acts_templates"
templ_path16=relref+"acts/acts_templates"
templ_path17=relref+"acts/acts_templates"
templ_path18=relref+"acts/acts_templates"
templ_path19=relref+"acts/acts_templates"
templ_path20=relref+"acts/acts_templates"
templ_path21=relref+"acts/acts_templates"
templ_path22=relref+"acts/acts_templates"
templ_path23=relref+"acts/acts_templates"
templ_path24=relref+"acts/acts_templates"
templ_path25=relref+"acts/acts_templates"
templ_path26=relref+"acts/acts_templates"
templ_path27=relref+"acts/acts_templates"
templ_path28=relref+"acts/acts_templates"
templ_path29=relref+"acts/acts_templates"
templ_path30=relref+"acts/acts_templates"
templ_path31=relref+"acts/acts_templates"
templ_path32=relref+"acts/acts_templates"
templ_path33=relref+"acts/acts_templates"
templ_path34=relref+"acts/acts_templates"
templ_path35=relref+"acts/acts_templates"

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
        
context['DRAMBANKS_charseq'] = []
for i in range (0,(context['NUMDRAMBANKS'])):
		c = chr(ord('A') + i)
		context['DRAMBANKS_charseq'].append(c)

context['NUMDRAMBANKS_seq'] = []
for i in range (0,(context['NUMDRAMBANKS'])):
		context['NUMDRAMBANKS_seq'].append(i)
		
context['INSTANCES_charseq'] = []
for i in range (0,(context['NUMINSTANCES'])):
		c = chr(ord('A') + i)
		context['INSTANCES_charseq'].append(c)

context['NUMINSTANCES_seq'] = []
for i in range (0,(context['NUMINSTANCES'])):
		context['NUMINSTANCES_seq'].append(i)

context['NUMWORKERS_seq'] = []
for i in range (0,(context['NUMWORKERS'])):
		context['NUMWORKERS_seq'].append(i)   

context['NUMSUBWORKERS_seq'] = []
for i in range (0,(context['NUMSUBWORKERS'])):
		context['NUMSUBWORKERS_seq'].append(i)
        
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
        
context['COMPUTEUNITS_seq'] = []
for i in range (0,(context['NUMCOMPUTEUNITS'])):
		context['COMPUTEUNITS_seq'].append(i)
        
context['COMPUTEUNITSPLUS1_seq'] = []
for i in range (0,(context['NUMCOMPUTEUNITS']+1)):
		context['COMPUTEUNITSPLUS1_seq'].append(i)
        
context['COMPUTEUNITSPLUS2_seq'] = []
for i in range (0,(context['NUMCOMPUTEUNITS']+2)):
		context['COMPUTEUNITSPLUS2_seq'].append(i)
        
context['COMPUTEUNITS_1seq'] = []
for i in range (1,(context['NUMCOMPUTEUNITS'])):
		context['COMPUTEUNITS_1seq'].append(i)
        
context['NUMSUBCPUTHREADS_seq'] = []
for i in range (0,(context['NUMSUBCPUTHREADS'])):
		context['NUMSUBCPUTHREADS_seq'].append(i)
        
context['NUMSUBCPUTHREADS_DIV2_seq'] = []
for i in range (0,(context['NUMSUBCPUTHREADS']/2)):
		context['NUMSUBCPUTHREADS_DIV2_seq'].append(i)
        
context['NUMSUBCPUTHREADS_DIV4_seq'] = []
for i in range (0,(context['NUMSUBCPUTHREADS']/4)):
		context['NUMSUBCPUTHREADS_DIV4_seq'].append(i)
        
context['NUMSUBCPUTHREADS_DIV8_seq'] = []
for i in range (0,(context['NUMSUBCPUTHREADS']/8)):
		context['NUMSUBCPUTHREADS_DIV8_seq'].append(i)
        
# SYNC
context['NUM_EDGE_BANKS_seq'] = []
for i in range (0,(context['NUM_EDGE_BANKS'])):
		context['NUM_EDGE_BANKS_seq'].append(i)
        
context['NUMSYNCTHREADS'] = 3
# context['NUMSYNCTHREADS'] = context['NUM_PEs']
    
context['KKL'] = (context['NUMSYNCTHREADS']/4) * 4
context['KKM'] = context['NUMSYNCTHREADS'] - context['KKL']
if context['KKM'] == 0:
    context['KKM'] = 4

context['SYNCTHREADS_seq'] = []
for i in range (0,(context['NUMSYNCTHREADS'])):
		context['SYNCTHREADS_seq'].append(i)
        
# context['NUMSYNCTHREADS_DIV_4'] = context['NUMSYNCTHREADS'] / 4
context['NUMSYNCTHREADS_DIV_4'] = (context['NUMSYNCTHREADS'] + (4-1)) / 4 # CRITICAL NEWCHANGE.
context['NUMSYNCTHREADS_DIV_4_seq'] = []
for i in range (0,(context['NUMSYNCTHREADS_DIV_4'])):
		context['NUMSYNCTHREADS_DIV_4_seq'].append(i)
        
context['NUMSYNCTHREADS_DIV_4__PLUS1'] = context['NUMSYNCTHREADS_DIV_4'] + 1
context['NUMSYNCTHREADS_DIV_4__PLUS1_seq'] = []
for i in range (0,(context['NUMSYNCTHREADS_DIV_4__PLUS1'])):
		context['NUMSYNCTHREADS_DIV_4__PLUS1_seq'].append(i)
        
context['NUMSYNCTHREADS_DIV_4__PLUS2'] = context['NUMSYNCTHREADS_DIV_4'] + 2
context['NUMSYNCTHREADS_DIV_4__PLUS2_seq'] = []
for i in range (0,(context['NUMSYNCTHREADS_DIV_4__PLUS2'])):
		context['NUMSYNCTHREADS_DIV_4__PLUS2_seq'].append(i)
		
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

template0 = env0.get_template('acts.template')
template1 = env1.get_template('acts_h.template')
template2 = env2.get_template('actsproc.template')
template3 = env3.get_template('actsproc_h.template')
template4 = env4.get_template('common_h.template')
template5 = env5.get_template('common_h.template')
template6 = env6.get_template('common_h.template')
template7 = env7.get_template('common_h.template')
template8 = env8.get_template('actssync.template')
template9 = env9.get_template('actssync_h.template')
template10 = env10.get_template('top_usrcv_udstv.template')
template11 = env11.get_template('top_usrcv_udstv_h.template')
template12 = env12.get_template('top_nusrcv_nudstv.template')
template13 = env13.get_template('top_nusrcv_nudstv_h.template')
template14 = env14.get_template('processedges_splitdstvxs.template')
template15 = env15.get_template('processedges_splitdstvxs_h.template')
template16 = env16.get_template('processedgesu.template')
template17 = env17.get_template('processedgesu_h.template')
template18 = env18.get_template('reduceupdates.template')
template19 = env19.get_template('reduceupdates_h.template')
template20 = env20.get_template('acts_util.template')
template21 = env21.get_template('acts_util_h.template')
template22 = env22.get_template('mem_access.template')
template23 = env23.get_template('mem_access_h.template')
template24 = env24.get_template('actsproc.template')
template25 = env25.get_template('actsproc_h.template')
template26 = env26.get_template('partitionupdates.template')
template27 = env27.get_template('partitionupdates_h.template')
template28 = env28.get_template('acts_merge.template')
template29 = env29.get_template('acts_merge_h.template')
template30 = env30.get_template('mem_access_splitdstvxs.template')
template31 = env31.get_template('mem_access_splitdstvxs_h.template')
template32 = env32.get_template('acts_merge_splitdstvxs.template')
template33 = env33.get_template('acts_merge_splitdstvxs_h.template')
template34 = env34.get_template('acts_all.template')
template35 = env35.get_template('acts_all_h.template')

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
# with open(out_path36, 'w') as outFile36:
	# outFile36.write(rendered_file36)
# with open(out_path37, 'w') as outFile37:
	# outFile37.write(rendered_file37)

print ("successful!")
print ("...")
quit()




