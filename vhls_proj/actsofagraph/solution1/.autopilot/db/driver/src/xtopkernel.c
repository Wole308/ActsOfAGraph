// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.1.3 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xtopkernel.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XTopkernel_CfgInitialize(XTopkernel *InstancePtr, XTopkernel_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XTopkernel_Start(XTopkernel *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTopkernel_ReadReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_AP_CTRL) & 0x80;
    XTopkernel_WriteReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XTopkernel_IsDone(XTopkernel *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTopkernel_ReadReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XTopkernel_IsIdle(XTopkernel *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTopkernel_ReadReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XTopkernel_IsReady(XTopkernel *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTopkernel_ReadReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XTopkernel_EnableAutoRestart(XTopkernel *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTopkernel_WriteReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XTopkernel_DisableAutoRestart(XTopkernel *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTopkernel_WriteReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_AP_CTRL, 0);
}

void XTopkernel_Set_kvsourcedramA_V(XTopkernel *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTopkernel_WriteReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_KVSOURCEDRAMA_V_DATA, Data);
}

u32 XTopkernel_Get_kvsourcedramA_V(XTopkernel *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTopkernel_ReadReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_KVSOURCEDRAMA_V_DATA);
    return Data;
}

void XTopkernel_Set_kvdestdramA_V(XTopkernel *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTopkernel_WriteReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_KVDESTDRAMA_V_DATA, Data);
}

u32 XTopkernel_Get_kvdestdramA_V(XTopkernel *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTopkernel_ReadReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_KVDESTDRAMA_V_DATA);
    return Data;
}

void XTopkernel_Set_kvstatsA(XTopkernel *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTopkernel_WriteReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_KVSTATSA_DATA, Data);
}

u32 XTopkernel_Get_kvstatsA(XTopkernel *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTopkernel_ReadReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_KVSTATSA_DATA);
    return Data;
}

void XTopkernel_InterruptGlobalEnable(XTopkernel *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTopkernel_WriteReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_GIE, 1);
}

void XTopkernel_InterruptGlobalDisable(XTopkernel *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTopkernel_WriteReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_GIE, 0);
}

void XTopkernel_InterruptEnable(XTopkernel *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XTopkernel_ReadReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_IER);
    XTopkernel_WriteReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_IER, Register | Mask);
}

void XTopkernel_InterruptDisable(XTopkernel *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XTopkernel_ReadReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_IER);
    XTopkernel_WriteReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_IER, Register & (~Mask));
}

void XTopkernel_InterruptClear(XTopkernel *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTopkernel_WriteReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_ISR, Mask);
}

u32 XTopkernel_InterruptGetEnabled(XTopkernel *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTopkernel_ReadReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_IER);
}

u32 XTopkernel_InterruptGetStatus(XTopkernel *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTopkernel_ReadReg(InstancePtr->Control_BaseAddress, XTOPKERNEL_CONTROL_ADDR_ISR);
}

