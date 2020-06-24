// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.1.3 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xtopkernel.h"

extern XTopkernel_Config XTopkernel_ConfigTable[];

XTopkernel_Config *XTopkernel_LookupConfig(u16 DeviceId) {
	XTopkernel_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XTOPKERNEL_NUM_INSTANCES; Index++) {
		if (XTopkernel_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XTopkernel_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XTopkernel_Initialize(XTopkernel *InstancePtr, u16 DeviceId) {
	XTopkernel_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XTopkernel_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XTopkernel_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

