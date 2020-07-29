set lang "C++"
set moduleName "topkernel"
set moduleIsExternC "1"
set rawDecl ""
set globalVariable ""
set PortList ""
set PortName0 "kvsourcedramA"
set BitWidth0 "64"
set ArrayOpt0 ""
set Const0 "0"
set Volatile0 "0"
set Pointer0 "1"
set Reference0 "0"
set Dims0 [list 0]
set Interface0 "wire"
set DataType0 "[list ap_uint 512 ]"
set Port0 [list $PortName0 $Interface0 $DataType0 $Pointer0 $Dims0 $Const0 $Volatile0 $ArrayOpt0]
lappend PortList $Port0
set PortName1 "kvdestdramA"
set BitWidth1 "64"
set ArrayOpt1 ""
set Const1 "0"
set Volatile1 "0"
set Pointer1 "1"
set Reference1 "0"
set Dims1 [list 0]
set Interface1 "wire"
set DataType1 "[list ap_uint 512 ]"
set Port1 [list $PortName1 $Interface1 $DataType1 $Pointer1 $Dims1 $Const1 $Volatile1 $ArrayOpt1]
lappend PortList $Port1
set PortName2 "kvstatsA"
set BitWidth2 "64"
set ArrayOpt2 ""
set Const2 "0"
set Volatile2 "0"
set Pointer2 "1"
set Reference2 "0"
set Dims2 [list 0]
set Interface2 "wire"
set structMem2 ""
set PortName20 "key"
set BitWidth20 "32"
set ArrayOpt20 ""
set Const20 "0"
set Volatile20 "0"
set Pointer20 "0"
set Reference20 "0"
set Dims20 [list 0]
set Interface20 "wire"
set DataType20 "unsigned int"
set Port20 [list $PortName20 $Interface20 $DataType20 $Pointer20 $Dims20 $Const20 $Volatile20 $ArrayOpt20]
lappend structMem2 $Port20
set PortName21 "value"
set BitWidth21 "32"
set ArrayOpt21 ""
set Const21 "0"
set Volatile21 "0"
set Pointer21 "0"
set Reference21 "0"
set Dims21 [list 0]
set Interface21 "wire"
set DataType21 "unsigned int"
set Port21 [list $PortName21 $Interface21 $DataType21 $Pointer21 $Dims21 $Const21 $Volatile21 $ArrayOpt21]
lappend structMem2 $Port21
set structParameter2 [list ]
set structArgument2 [list ]
set NameSpace2 [list ]
set structIsPacked2 "0"
set DataType2 [list "keyvalue_t" "struct " $structMem2 0 0 $structParameter2 $structArgument2 $NameSpace2 $structIsPacked2]
set Port2 [list $PortName2 $Interface2 $DataType2 $Pointer2 $Dims2 $Const2 $Volatile2 $ArrayOpt2]
lappend PortList $Port2
set globalAPint "" 
set returnAPInt "" 
set hasCPPAPInt 1 
set argAPInt "" 
set hasCPPAPFix 0 
set hasSCFix 0 
set hasCBool 0 
set hasCPPComplex 0 
set isTemplateTop 0
set hasHalf 0 
set dataPackList ""
set module [list $moduleName $PortList $rawDecl $argAPInt $returnAPInt $dataPackList]
