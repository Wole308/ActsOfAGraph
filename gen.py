import argparse
import xml.etree.ElementTree as ET
import os
from jinja2 import Environment, FileSystemLoader
import math
import sys
import array as arr 

ACTSACCEL1_params = arr.array('i', [1, 4, 1])

context = {}
print ('ACTGraph (Courtesy: Jinja 2.0)...')
context['XWARE'] = sys.argv[1]
context['SETUP'] = sys.argv[2]
context['ALGORITHM'] = sys.argv[3] 
context['DATASET'] = sys.argv[4]
context['NUMCPUTHREADS'] = sys.argv[5] # FIXME. this is overriden in common.h
context['LOCKE'] = sys.argv[6]
context['EVALUATION_TYPE'] = sys.argv[7]
context['EVALUATION_PARAM0'] = int(sys.argv[8])
path = '/home/centos/src/project_data/oj2zf/ActsOfAGraph/gen.py'
isFile = os.path.isfile(path)  
print(isFile) 

###

EV_IMPACTOFRANGE = [0, 2, 4, 8, 12, 16]
if context['EVALUATION_TYPE'] == "EV_IMPACTOFRANGE":
    context['MAXNUMSSDPARTITIONS_POW'] = EV_IMPACTOFRANGE[context['EVALUATION_PARAM0']]
else:
    if context['DATASET'] == "_EQUALDATASET_X":
        context['MAXNUMSSDPARTITIONS_POW'] = 2
        context['KVDATA_RANGE_POW'] = 24
    elif context['DATASET'] == "_LARGEDATASET_1M":
        context['MAXNUMSSDPARTITIONS_POW'] = 2
        context['KVDATA_RANGE_POW'] = 20
    elif context['DATASET'] == "_LARGEDATASET_67M":
        context['MAXNUMSSDPARTITIONS_POW'] = 2
        context['KVDATA_RANGE_POW'] = 26
    elif context['DATASET'] == "_LARGEDATASET_268M":
        context['MAXNUMSSDPARTITIONS_POW'] = 2
        context['KVDATA_RANGE_POW'] = 28
    elif context['DATASET'] == "_LARGEDATASET_1B":
        context['MAXNUMSSDPARTITIONS_POW'] = 4
        context['KVDATA_RANGE_POW'] = 30
    elif context['DATASET'] == "_LARGEDATASET_4B":
        context['MAXNUMSSDPARTITIONS_POW'] = 4
        context['KVDATA_RANGE_POW'] = 32

EV_IMPACTOFPARTITIONFANOUT = [3, 4, 5, 6, 7, 8]
if context['EVALUATION_TYPE'] == "EV_IMPACTOFPARTITIONFANOUT":
    context['NUM_PARTITIONS_POW'] = EV_IMPACTOFPARTITIONFANOUT[context['EVALUATION_PARAM0']]
else:
    context['NUM_PARTITIONS_POW'] = 4
    
EV_IMPACTOFNUMSUBWORKERS = [1, 2, 4, 8, 12, 16]
if context['EVALUATION_TYPE'] == "EV_IMPACTOFNUMSUBWORKERS": 
    context['NUMSUBWORKERS'] = EV_IMPACTOFNUMSUBWORKERS[context['EVALUATION_PARAM0']]
else:
    context['NUMSUBWORKERS'] = 4
    
EV_IMPACTOFBANDWIDTH = [1, 2, 4, 6, 8, 8]
if context['EVALUATION_TYPE'] == "EV_IMPACTOFBANDWIDTH": 
    context['VECTOR_SIZE'] = EV_IMPACTOFBANDWIDTH[context['EVALUATION_PARAM0']]
else:
    context['VECTOR_SIZE'] = 8
    
EV_IMPACTOFPLATFORM = [1, 2, 4, 8, 12, 16]
if context['EVALUATION_TYPE'] == "EV_IMPACTOFPLATFORM": 
    context['DUMMY'] = 0
    context['LOCKE'] = "NOLOCKE"
else:
    context['DUMMY'] = 0

###

if (isFile):
    context['PLATFORM'] = "AWS_PLATFORM"
else:
    context['PLATFORM'] = "CRABTREE_PLATFORM"
    
if context['XWARE'] == "SW":
	context['NUMINSTANCES'] = 1
else:
	context['NUMINSTANCES'] = 1 # 4
    
if (context['PLATFORM'] == "AWS_PLATFORM"):
	context['PROJECT_BASEPATH'] = "/home/centos/src/project_data/oj2zf/ActsOfAGraph"
else:
	context['PROJECT_BASEPATH'] = "/home/oj2zf/Documents/ActsOfAGraph"
    
context['DATAWIDTH'] = 512
context['NUMBITSINKVPAIR'] = 64
context['NUMDRAMBANKS'] = 4
context['NUMWORKERS'] = ACTSACCEL1_params[0]
context['BUNDLEFACTOR'] = ACTSACCEL1_params[2]
context['NUMPEFUNCS'] = 8
context['NUMWORKERS_APPLYPH'] = 1
context['NUMSUBWORKERS_APPLYPH'] = ACTSACCEL1_params[1] 
context['BUNDLEFACTOR_APPLYPH'] = 1
context['NUMPARTIALRESULTS'] = 4
context['NUMSUBWORKERSPERVECTOR'] = context['VECTOR_SIZE'] / context['NUMSUBWORKERS']
context['KERNELNAME'] = "ACTS" # "TRADITIONAL"
# context['KERNELNAME'] = "TRADITIONAL"
context['NUM_PARTITIONS'] = 2**context['NUM_PARTITIONS_POW']
context['MAXNUMSSDPARTITIONS'] = 2**context['MAXNUMSSDPARTITIONS_POW']
    
print ('Generating sources... ')
print ('XWARE: ' + str(context['XWARE']))
print ('SETUP: ' + str(context['SETUP']))
print ('ALGORITHM: ' + str(context['ALGORITHM']))
print ('DATASET: ' + str(context['DATASET']))
print ('NUMCPUTHREADS: ' + str(context['NUMCPUTHREADS']))
print ('LOCKE: ' + str(context['LOCKE']))
print ('EVALUATION_TYPE: ' + str(context['EVALUATION_TYPE']))
print ('EVALUATION_PARAM0: ' + str(context['EVALUATION_PARAM0']))
print ('NUMDRAMBANKS: ' + str(context['NUMDRAMBANKS']))
print ('NUMINSTANCES: ' + str(context['NUMINSTANCES']))
print ('NUM_PARTITIONS: ' + str(context['NUM_PARTITIONS']))
print ('NUMWORKERS: ' + str(context['NUMWORKERS'])) 
print ('NUMSUBWORKERS: ' + str(context['NUMSUBWORKERS']))
print ('VECTOR_SIZE: ' + str(context['VECTOR_SIZE']))
print ('BUNDLEFACTOR: ' + str(context['BUNDLEFACTOR'])) 
print ('NUMWORKERS_APPLYPH: ' + str(context['NUMWORKERS_APPLYPH'])) 
print ('NUMSUBWORKERS_APPLYPH: ' + str(context['NUMSUBWORKERS_APPLYPH'])) 
print ('BUNDLEFACTOR_APPLYPH: ' + str(context['BUNDLEFACTOR_APPLYPH'])) 
context['KERNELTYPE'] = "_SINGLEKERNEL"
	
o_path0="src/common.h"
o_path1="src/common.h"
o_path2="src/common.h"
o_path3="src/common.h"
o_path4="src/common.h"
o_path5="src/common.h"
o_path6="src/common.h"
o_path7="src/host_functions.cpp"
o_path8="debugger/host_debugger.cpp"
o_path9="src/common.h"
o_path10="src/common.h"
o_path11="src/common.h"
o_path12="src/common.h"
o_path13="src/common.h"
o_path14="src/common.h"
o_path15="src/hostprocess.cpp"
o_path16="src/common.h"
o_path17="src/common.h"
o_path18="kernels/srkernelprocess.cpp"
o_path19="kernels/srkernelprocess.h"
o_path20="src/grafboost_pr_sw.cpp"
o_path21="src/grafboost_pr_sw.h"
o_path22="src/actgraph_pr_sw.cpp"
o_path23="src/actgraph_pr_sw.h"
o_path24="src/actgraph_pr_sw.cpp"
o_path25="src/actgraph_pr_sw.h"
o_path26="src/actgraph_bfs_sw.cpp"
o_path27="src/actgraph_bfs_sw.h"
o_path28="src/grafboost_bfs_sw.cpp"
o_path29="src/grafboost_bfs_sw.h"
o_path30="src/edge_process.cpp"
o_path31="src/edge_process.h"
o_path32="src/actgraph_bfs_sw.cpp"
o_path33="src/actgraph_bfs_sw.h"
o_path34="utility/utility.cpp"
o_path35="src/actgraph_bc_sw.cpp"
o_path36="src/actgraph_bc_sw.h"
o_path37="src/grafboost_bc_sw.cpp"
o_path38="src/grafboost_bc_sw.h"
o_path39="src/common.h"
o_path40="src/common.h"
o_path41="src/common.h"
o_path42="src/common.h"
o_path43="kernels/enigma.cpp"
o_path44="kernels/enigma.h"
o_path45="src/host_enigma.cpp"
o_path46="src/host_enigma.h"
o_path47="kernels/acts.cpp"
o_path48="kernels/acts.h"
o_path49="src/common.h"
o_path50="src/common.h"
o_path51="kernels/kernelprocess.cpp"
o_path52="kernels/kernelprocess.h"
o_path53="src/common.h"
o_path54="src/common.h"

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
out_path38=os.path.abspath(o_path38)
out_path39=os.path.abspath(o_path39)
out_path40=os.path.abspath(o_path40)
out_path41=os.path.abspath(o_path41)
out_path42=os.path.abspath(o_path42)
out_path43=os.path.abspath(o_path43)
out_path44=os.path.abspath(o_path44)
out_path45=os.path.abspath(o_path45)
out_path46=os.path.abspath(o_path46)
out_path47=os.path.abspath(o_path47)
out_path48=os.path.abspath(o_path48)
out_path49=os.path.abspath(o_path49)
out_path50=os.path.abspath(o_path50)
out_path51=os.path.abspath(o_path51)
out_path52=os.path.abspath(o_path52)
out_path53=os.path.abspath(o_path53)
out_path54=os.path.abspath(o_path54)

templ_path0="templates/src/"
templ_path1="templates/src/"
templ_path2="templates/src/"
templ_path3="templates/src/"
templ_path4="templates/src/"
templ_path5="templates/src/"
templ_path6="templates/src/"
templ_path7="templates/src/"
templ_path8="templates/debugger/"
templ_path9="templates/src/"
templ_path10="templates/src/"
templ_path11="templates/src/"
templ_path12="templates/src/"
templ_path13="templates/src/"
templ_path14="templates/src/"
templ_path15="templates/src/"
templ_path16="templates/src/"
templ_path17="templates/src/"
templ_path18="templates/kernels/"
templ_path19="templates/kernels/"
templ_path20="templates/src/"
templ_path21="templates/src/"
templ_path22="templates/src/"
templ_path23="templates/src/"
templ_path24="templates/src/"
templ_path25="templates/src/"
templ_path26="templates/src/"
templ_path27="templates/src/"
templ_path28="templates/src/"
templ_path29="templates/src/"
templ_path30="templates/src/"
templ_path31="templates/src/"
templ_path32="templates/src/"
templ_path33="templates/src/"
templ_path34="templates/utility/"
templ_path35="templates/src/"
templ_path36="templates/src/"
templ_path37="templates/src/"
templ_path38="templates/src/"
templ_path39="templates/src/"
templ_path40="templates/src/"
templ_path41="templates/src/"
templ_path42="templates/src/"
templ_path43="templates/kernels/"
templ_path44="templates/kernels/"
templ_path45="templates/src/"
templ_path46="templates/src/"
templ_path47="templates/kernels/"
templ_path48="templates/kernels/"
templ_path49="templates/src/"
templ_path50="templates/src/"
templ_path51="templates/kernels/"
templ_path52="templates/kernels/"
templ_path53="templates/src/"
templ_path54="templates/src/"

context['1_seq'] = []
for i in range (0,1):
		context['1_seq'].append(i)

context['VECTOR_SIZE_seq'] = []
for i in range (0,(context['VECTOR_SIZE'])):
		context['VECTOR_SIZE_seq'].append(i)
		
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
		
context['NUMWORKERS_APPLYPH_seq'] = []
for i in range (0,(context['NUMWORKERS_APPLYPH'])):
		context['NUMWORKERS_APPLYPH_seq'].append(i)   

context['NUMSUBWORKERS_seq'] = []
for i in range (0,(context['NUMSUBWORKERS'])):
		context['NUMSUBWORKERS_seq'].append(i)
		
context['NUMSUBWORKERS_APPLYPH_seq'] = []
for i in range (0,(context['NUMSUBWORKERS_APPLYPH'])):
		context['NUMSUBWORKERS_APPLYPH_seq'].append(i)
		
context['BUNDLEFACTOR_seq'] = []
for i in range (0,(context['BUNDLEFACTOR'])):
		context['BUNDLEFACTOR_seq'].append(i)
		
if context['BUNDLEFACTOR'] == 1:
	context['BUNDLEFACTOR_lseq'] = []
	for i in range (0,(context['NUMWORKERS'])):
			context['BUNDLEFACTOR_lseq'].append(0)
else:
	context['BUNDLEFACTOR_lseq'] = []
	for i in range (0,(context['NUMWORKERS'])):
			context['BUNDLEFACTOR_lseq'].append(i)

if context['BUNDLEFACTOR_APPLYPH'] == 1:
	context['BUNDLEFACTOR_APPLYPH_lseq'] = []
	for i in range (0,(context['NUMWORKERS_APPLYPH'])):
			context['BUNDLEFACTOR_APPLYPH_lseq'].append(0)
else:
	context['BUNDLEFACTOR_APPLYPH_lseq'] = []
	for i in range (0,(context['NUMWORKERS_APPLYPH'])):
			context['BUNDLEFACTOR_APPLYPH_lseq'].append(i)

context['NUMPEFUNCS_seq'] = []
for i in range (0,(context['NUMPEFUNCS'])):
		context['NUMPEFUNCS_seq'].append(i)
		
context['MAXNUMSSDPARTITIONS_seq'] = []
for i in range (0,(context['MAXNUMSSDPARTITIONS'])):
		context['MAXNUMSSDPARTITIONS_seq'].append(i)
		
context['NUMWORKERS_FROM2nd_seq'] = []
for i in range (1,(context['NUMWORKERS'])):
		context['NUMWORKERS_FROM2nd_seq'].append(i)   
		
context['NUM_PARTITIONS_seq'] = []
for i in range (0,(context['NUM_PARTITIONS'])):
		context['NUM_PARTITIONS_seq'].append(i)
		
context['NUMPARTIALRESULTS_seq'] = []
for i in range (0,(context['NUMPARTIALRESULTS'])):
		context['NUMPARTIALRESULTS_seq'].append(i)
		
context['NUMPARTITIONS_seq'] = []
for i in range (0,(context['NUM_PARTITIONS'])):
		context['NUMPARTITIONS_seq'].append(i)   
        
context['NUMSUBWORKERSPERVECTOR_seq'] = []
for i in range (0,(context['NUMSUBWORKERSPERVECTOR'])):
		context['NUMSUBWORKERSPERVECTOR_seq'].append(i)
		
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
env38 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path38)), trim_blocks=True, lstrip_blocks=True)
env39 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path39)), trim_blocks=True, lstrip_blocks=True)
env40 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path40)), trim_blocks=True, lstrip_blocks=True)
env41 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path41)), trim_blocks=True, lstrip_blocks=True)
env42 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path42)), trim_blocks=True, lstrip_blocks=True)
env43 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path43)), trim_blocks=True, lstrip_blocks=True)
env44 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path44)), trim_blocks=True, lstrip_blocks=True)
env45 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path45)), trim_blocks=True, lstrip_blocks=True)
env46 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path46)), trim_blocks=True, lstrip_blocks=True)
env47 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path47)), trim_blocks=True, lstrip_blocks=True)
env48 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path48)), trim_blocks=True, lstrip_blocks=True)
env49 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path49)), trim_blocks=True, lstrip_blocks=True)
env50 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path50)), trim_blocks=True, lstrip_blocks=True)
env51 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path51)), trim_blocks=True, lstrip_blocks=True)
env52 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path52)), trim_blocks=True, lstrip_blocks=True)
env53 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path53)), trim_blocks=True, lstrip_blocks=True)
env54 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path54)), trim_blocks=True, lstrip_blocks=True)

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
env38.globals.update(zip=zip)
env39.globals.update(zip=zip)
env40.globals.update(zip=zip)
env41.globals.update(zip=zip)
env42.globals.update(zip=zip)
env43.globals.update(zip=zip)
env44.globals.update(zip=zip)
env45.globals.update(zip=zip)
env46.globals.update(zip=zip)
env47.globals.update(zip=zip)
env48.globals.update(zip=zip)
env49.globals.update(zip=zip)
env50.globals.update(zip=zip)
env51.globals.update(zip=zip)
env52.globals.update(zip=zip)
env53.globals.update(zip=zip)
env54.globals.update(zip=zip)

template0 = env0.get_template('common_h.template')
template1 = env1.get_template('common_h.template')
template2 = env2.get_template('common_h.template')
template3 = env3.get_template('common_h.template')
template4 = env4.get_template('common_h.template')
template5 = env5.get_template('common_h.template')
template6 = env6.get_template('common_h.template')
template7 = env7.get_template('host_functions.template')
template8 = env8.get_template('host_debugger.template')
template9 = env9.get_template('common_h.template')
template10 = env10.get_template('common_h.template')
template11 = env11.get_template('common_h.template')
template12 = env12.get_template('common_h.template')
template13 = env13.get_template('common_h.template')
template14 = env14.get_template('common_h.template')
template15 = env15.get_template('hostprocess.template')
template16 = env16.get_template('common_h.template')
template17 = env17.get_template('common_h.template')
template18 = env18.get_template('srkernelprocess.template')
template19 = env19.get_template('srkernelprocess_h.template')
template20 = env20.get_template('grafboost_pr_sw.template')
template21 = env21.get_template('grafboost_pr_sw_h.template')
template22 = env22.get_template('actgraph_pr_sw.template')
template23 = env23.get_template('actgraph_pr_sw_h.template')
template24 = env24.get_template('actgraph_pr_sw.template')
template25 = env25.get_template('actgraph_pr_sw_h.template')
template26 = env26.get_template('actgraph_bfs_sw.template')
template27 = env27.get_template('actgraph_bfs_sw_h.template')
template28 = env28.get_template('grafboost_bfs_sw.template')
template29 = env29.get_template('grafboost_bfs_sw_h.template')
template30 = env30.get_template('edge_process.template')
template31 = env31.get_template('edge_process_h.template')
template32 = env32.get_template('actgraph_bfs_sw.template')
template33 = env33.get_template('actgraph_bfs_sw_h.template')
template34 = env34.get_template('utility.template')
template35 = env35.get_template('actgraph_bc_sw.template')
template36 = env36.get_template('actgraph_bc_sw_h.template')
template37 = env37.get_template('grafboost_bc_sw.template')
template38 = env38.get_template('grafboost_bc_sw_h.template')
template39 = env39.get_template('common_h.template')
template40 = env40.get_template('common_h.template')
template41 = env41.get_template('common_h.template')
template42 = env42.get_template('common_h.template')
template43 = env43.get_template('enigma.template')
template44 = env44.get_template('enigma_h.template')
template45 = env45.get_template('host_enigma.template')
template46 = env46.get_template('host_enigma_h.template')
template47 = env47.get_template('acts.template')
template48 = env48.get_template('acts_h.template')
template49 = env49.get_template('common_h.template')
template50 = env50.get_template('common_h.template')
template51 = env51.get_template('kernelprocess.template')
template52 = env52.get_template('kernelprocess_h.template')
template53 = env53.get_template('common_h.template')
template54 = env54.get_template('common_h.template')

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
rendered_file38 = template38.render(context=context)
rendered_file39 = template39.render(context=context)
rendered_file40 = template40.render(context=context)
rendered_file41 = template41.render(context=context)
rendered_file42 = template42.render(context=context)
rendered_file43 = template43.render(context=context)
rendered_file44 = template44.render(context=context)
rendered_file45 = template45.render(context=context)
rendered_file46 = template46.render(context=context)
rendered_file47 = template47.render(context=context)
rendered_file48 = template48.render(context=context)
rendered_file49 = template49.render(context=context)
rendered_file50 = template50.render(context=context)
rendered_file51 = template51.render(context=context)
rendered_file52 = template52.render(context=context)
rendered_file53 = template53.render(context=context)
rendered_file54 = template54.render(context=context)

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
with open(out_path38, 'w') as outFile38:
	outFile38.write(rendered_file38)
with open(out_path39, 'w') as outFile39:
	outFile39.write(rendered_file39)
with open(out_path40, 'w') as outFile40:
	outFile40.write(rendered_file40)
with open(out_path41, 'w') as outFile41:
	outFile41.write(rendered_file41)
with open(out_path42, 'w') as outFile42:
	outFile42.write(rendered_file42)
with open(out_path43, 'w') as outFile43:
	outFile43.write(rendered_file43)
with open(out_path44, 'w') as outFile44:
	outFile44.write(rendered_file44)
with open(out_path45, 'w') as outFile45:
	outFile45.write(rendered_file45)
with open(out_path46, 'w') as outFile46:
	outFile46.write(rendered_file46)
with open(out_path47, 'w') as outFile47:
	outFile47.write(rendered_file47)
with open(out_path48, 'w') as outFile48:
	outFile48.write(rendered_file48)
with open(out_path49, 'w') as outFile49:
	outFile49.write(rendered_file49)
with open(out_path50, 'w') as outFile50:
	outFile50.write(rendered_file50)
with open(out_path51, 'w') as outFile51:
	outFile51.write(rendered_file51)
with open(out_path52, 'w') as outFile52:
	outFile52.write(rendered_file52)
with open(out_path53, 'w') as outFile53:
	outFile53.write(rendered_file53)
with open(out_path54, 'w') as outFile54:
	outFile54.write(rendered_file54)

print ("successful!")
print ("...")
quit()




