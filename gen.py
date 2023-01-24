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
context['EVALUATION_TYPE'] = sys.argv[2]
context['NUM_PEs'] = int(sys.argv[3])
context['TESTKERNEL'] = sys.argv[4]

context['FPGA_IMPL'] = 0
context['NUM_VALID_PEs'] = 6 #1 #6* #12 #context['NUM_PEs'] # 4 NUM_VALID_PEs = 8 AXI interfaces
context['NUM_AXI_CHANNELS_IN_PE'] = 4
context['EDGE_PACK_SIZE_POW'] = 4 #1, 4*
context['EDGE_PACK_SIZE'] = 2**context['EDGE_PACK_SIZE_POW']
context['HBM_AXI_PACK_SIZE'] = context['EDGE_PACK_SIZE'] 

print ('Generating sources... ')
print ('XWARE: ' + str(context['XWARE']))
print ('NUM_PEs: ' + str(context['NUM_PEs']))
print ('NUM_VALID_PEs: ' + str(context['NUM_VALID_PEs']))
print ('NUM_AXI_CHANNELS_IN_PE: ' + str(context['NUM_AXI_CHANNELS_IN_PE']))
print ('EDGE_PACK_SIZE: ' + str(context['EDGE_PACK_SIZE']))
print ('HBM_AXI_PACK_SIZE: ' + str(context['HBM_AXI_PACK_SIZE']))

###

# relref="../"
relref=""

o_path0=relref+"include/common.h"
o_path1=relref+"acts_templates/acts_kernel.cpp"
o_path2=relref+"acts_templates/acts_kernel.h"
o_path3=relref+"acts_templates/acts_async_kernel.cpp"
o_path4=relref+"acts_templates/acts_async_kernel.h"
o_path5=relref+"include/common.h"
o_path6=relref+"include/common.h"
o_path7=relref+"include/common.h"
o_path8=relref+"include/common.h"
o_path9=relref+"include/common.h"
o_path10=relref+"include/common.h"

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

templ_path0=relref+"include/"
templ_path1=relref+"acts_templates/"
templ_path2=relref+"acts_templates/"
templ_path3=relref+"acts_templates/"
templ_path4=relref+"acts_templates/"
templ_path5=relref+"include/"
templ_path6=relref+"include/"
templ_path7=relref+"include/"
templ_path8=relref+"include/"
templ_path9=relref+"include/"
templ_path10=relref+"include/"

###
context['1_seq'] = []
for i in range (0,1):
		context['1_seq'].append(i)
        
context['NUM_VALID_PEs_seq'] = []
for i in range (0,(context['NUM_VALID_PEs'])):
		context['NUM_VALID_PEs_seq'].append(i)
        
context['HBM_AXI_PACK_SIZE_seq'] = []
for i in range (0,(context['HBM_AXI_PACK_SIZE'])):
		context['HBM_AXI_PACK_SIZE_seq'].append(i)
 
context['EDGE_PACK_SIZE_seq'] = []
for i in range (0,(context['EDGE_PACK_SIZE'])):
		context['EDGE_PACK_SIZE_seq'].append(i) 
        
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

template0 = env0.get_template('common_h.template')
template1 = env1.get_template('acts_kernel.template')
template2 = env2.get_template('acts_kernel_h.template')
template3 = env3.get_template('acts_async_kernel.template')
template4 = env4.get_template('acts_async_kernel_h.template')
template5 = env5.get_template('common_h.template')
template6 = env6.get_template('common_h.template')
template7 = env7.get_template('common_h.template')
template8 = env8.get_template('common_h.template')
template9 = env9.get_template('common_h.template')
template10 = env10.get_template('common_h.template')

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

print ("successful!")
print ("...")
quit()




