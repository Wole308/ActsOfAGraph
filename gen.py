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
context['NUMSUPERCPUTHREADS'] = sys.argv[5]
context['NUMCPUTHREADS'] = sys.argv[6] # FIXME. this is overriden in common.h
context['NUMSUBCPUTHREADS_POW'] = sys.argv[7]
context['LOCKE'] = sys.argv[8]
context['EVALUATION_TYPE'] = sys.argv[9]
context['EVALUATION_PARAM0'] = int(sys.argv[10])
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
    elif context['DATASET'] == "_LARGEDATASET_4M":
        context['MAXNUMSSDPARTITIONS_POW'] = 2
        context['KVDATA_RANGE_POW'] = 22
    elif context['DATASET'] == "_TWITTERDATASET_67M":
        context['MAXNUMSSDPARTITIONS_POW'] = 4 #2
        context['KVDATA_RANGE_POW'] = 26
    elif context['DATASET'] == "_LARGEDATASET_67M":
        context['MAXNUMSSDPARTITIONS_POW'] = 4 #2
        context['KVDATA_RANGE_POW'] = 26
    elif context['DATASET'] == "_LARGEDATASET_268M":
        context['MAXNUMSSDPARTITIONS_POW'] = 4 #2
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
    
EV_IMPACTOFNUMSUBWORKERS = [1, 2, 4, 8, 12, 16]
if context['EVALUATION_TYPE'] == "EV_IMPACTOFNUMSUBWORKERS": 
    context['NUMSUBWORKERS'] = EV_IMPACTOFNUMSUBWORKERS[context['EVALUATION_PARAM0']]
else:
    context['NUMSUBWORKERS'] = 1

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
context['NUMWORKERS'] = ACTSACCEL1_params[0]
context['BUNDLEFACTOR'] = ACTSACCEL1_params[2]
context['NUMPEFUNCS'] = 8
context['NUMWORKERS_APPLYPH'] = 1
context['NUMSUBWORKERS_APPLYPH'] = ACTSACCEL1_params[1] 
context['BUNDLEFACTOR_APPLYPH'] = 1
context['NUMPARTIALRESULTS'] = 4
context['NUMSUBWORKERSPERVECTOR'] = context['VECTOR_SIZE'] / context['NUMSUBWORKERS']
context['KERNELNAME'] = "ACTS" # "TRADITIONAL"
context['NUM_PARTITIONS'] = 2**context['NUM_PARTITIONS_POW']
context['MAXNUMSSDPARTITIONS'] = 2**context['MAXNUMSSDPARTITIONS_POW']
    
print ('Generating sources... ')
print ('XWARE: ' + str(context['XWARE']))
print ('SETUP: ' + str(context['SETUP']))
print ('ALGORITHM: ' + str(context['ALGORITHM']))
print ('DATASET: ' + str(context['DATASET']))
print ('NUMSUPERCPUTHREADS: ' + str(context['NUMSUPERCPUTHREADS']))
print ('NUMCPUTHREADS: ' + str(context['NUMCPUTHREADS']))
print ('NUMSUBCPUTHREADS_POW: ' + str(context['NUMSUBCPUTHREADS_POW']))
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
	
o_path0="acts/acts/acts.cpp"
o_path1="acts/acts/acts.h"
o_path2="acts/acts_lw/actslw.cpp"
o_path3="acts/acts_lw/actslw.h"
o_path4="include/common.h"
o_path5="include/common.h"
o_path6="acts/acts_lw2/actslw.cpp"
o_path7="acts/acts_lw2/actslw.h"
o_path8="acts/acts_lw3/actslw.cpp"
o_path9="acts/acts_lw3/actslw.h"

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

templ_path0="acts/acts/"
templ_path1="acts/acts/"
templ_path2="acts/acts_lw/"
templ_path3="acts/acts_lw/"
templ_path4="include/"
templ_path5="include/"
templ_path6="acts/acts_lw2/"
templ_path7="acts/acts_lw2/"
templ_path8="acts/acts_lw3/"
templ_path9="acts/acts_lw3/"

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
        
context['16_seq'] = []
for i in range (0,16):
		context['16_seq'].append(i)
        
context['8_seq'] = []
for i in range (0,8):
		context['8_seq'].append(i)
        
context['4_seq'] = []
for i in range (0,4):
		context['4_seq'].append(i)
        
context['2_seq'] = []
for i in range (0,2):
		context['2_seq'].append(i)
        
context['1_seq'] = []
for i in range (0,1):
		context['1_seq'].append(i)
        
context['15_seq'] = []
for i in range (0,15):
		context['15_seq'].append(i)
		
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

template0 = env0.get_template('acts.template')
template1 = env1.get_template('acts_h.template')
template2 = env2.get_template('actslw.template')
template3 = env3.get_template('actslw_h.template')
template4 = env4.get_template('common_h.template')
template5 = env5.get_template('common_h.template')
template6 = env6.get_template('actslw.template')
template7 = env7.get_template('actslw_h.template')
template8 = env8.get_template('actslw.template')
template9 = env9.get_template('actslw_h.template')

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

print ("successful!")
print ("...")
quit()




