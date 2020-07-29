// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.1.3 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XTOPKERNEL_H
#define XTOPKERNEL_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xtopkernel_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 Control_BaseAddress;
} XTopkernel_Config;
#endif

typedef struct {
    u32 Control_BaseAddress;
    u32 IsReady;
} XTopkernel;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XTopkernel_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XTopkernel_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XTopkernel_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XTopkernel_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XTopkernel_Initialize(XTopkernel *InstancePtr, u16 DeviceId);
XTopkernel_Config* XTopkernel_LookupConfig(u16 DeviceId);
int XTopkernel_CfgInitialize(XTopkernel *InstancePtr, XTopkernel_Config *ConfigPtr);
#else
int XTopkernel_Initialize(XTopkernel *InstancePtr, const char* InstanceName);
int XTopkernel_Release(XTopkernel *InstancePtr);
#endif

void XTopkernel_Start(XTopkernel *InstancePtr);
u32 XTopkernel_IsDone(XTopkernel *InstancePtr);
u32 XTopkernel_IsIdle(XTopkernel *InstancePtr);
u32 XTopkernel_IsReady(XTopkernel *InstancePtr);
void XTopkernel_EnableAutoRestart(XTopkernel *InstancePtr);
void XTopkernel_DisableAutoRestart(XTopkernel *InstancePtr);

void XTopkernel_Set_kvsourcedramA_V(XTopkernel *InstancePtr, u32 Data);
u32 XTopkernel_Get_kvsourcedramA_V(XTopkernel *InstancePtr);
void XTopkernel_Set_kvdestdramA_V(XTopkernel *InstancePtr, u32 Data);
u32 XTopkernel_Get_kvdestdramA_V(XTopkernel *InstancePtr);
void XTopkernel_Set_kvstatsA(XTopkernel *InstancePtr, u32 Data);
u32 XTopkernel_Get_kvstatsA(XTopkernel *InstancePtr);

void XTopkernel_InterruptGlobalEnable(XTopkernel *InstancePtr);
void XTopkernel_InterruptGlobalDisable(XTopkernel *InstancePtr);
void XTopkernel_InterruptEnable(XTopkernel *InstancePtr, u32 Mask);
void XTopkernel_InterruptDisable(XTopkernel *InstancePtr, u32 Mask);
void XTopkernel_InterruptClear(XTopkernel *InstancePtr, u32 Mask);
u32 XTopkernel_InterruptGetEnabled(XTopkernel *InstancePtr);
u32 XTopkernel_InterruptGetStatus(XTopkernel *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
