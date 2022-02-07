#include "ScrVariable.h"
#include <stdio.h>

// ADDRESSES
#define SCR_VAR_PUB_ADDY    0x032c8580

// VAR_STATUS DEFS
#define VAR_STAT_FREE       0x00
#define VAR_STAT_MOVABLE    0x20
#define VAR_STAT_HEAD       0x40

// VAR_TYPE DEFS
#define VAR_UNDEFINED       0x00
#define VAR_BEGIN_REF       0x01
#define VAR_POINTER         0x01
#define VAR_STRING          0x02
#define VAR_ISTRING         0x03
#define VAR_VECTOR          0x04
#define VAR_END_REF         0x05
#define VAR_FLOAT           0x05
#define VAR_INTEGER         0x06
#define VAR_CODEPOS         0x07
#define VAR_PRECODEPOS      0x08
#define VAR_FUNCTION        0x09
#define VAR_STACK           0x0a
#define VAR_ANIMATION       0x0b
#define VAR_DEV_CODEPOS     0x0c
#define VAR_INCLUDE_CODEPOS 0x0d
#define VAR_THREAD          0x0e
#define VAR_NOTIFY_THREAD   0x0f
#define VAR_TIME_THREAD     0x10
#define VAR_CHLD_THREAD     0x11
#define VAR_OBJECT          0x12
#define VAR_DEAD_ENTITY     0x13
#define VAR_ENTITY          0x14
#define VAR_ARRAY           0x15
#define VAR_DEAD_THREAD     0x16
#define VAR_COUNT           0x17
#define VAR_THREAD_LIST     0x18
#define VAR_ENDON_LIST      0x19

/*
60010 = esi
2aae0000 = eax
08
*/
void DumpScriptThreadsExternal(HANDLE p_handle)
{
    // the offset for a server scriptinstance
    DWORD offset = 0x60000;
    DWORD base_address = 0;
    ReadProcessMemory(p_handle, (LPCVOID)SCR_VAR_PUB_ADDY, &base_address, sizeof(DWORD), NULL);
    DWORD entry_address = base_address + offset + 0x18;
    unsigned int variable_value_w;
    unsigned char variable_value_w_lsb;
    int num_stack_variables;
    for (int i = 0; i < 0xfffd; ++i)
    {
        ReadProcessMemory(p_handle, (LPCVOID)entry_address, &variable_value_w, sizeof(int), NULL);
        variable_value_w_lsb = variable_value_w & 0xff;  // get the least significant byte
        if ((variable_value_w_lsb & 0x60) != VAR_STAT_FREE)
        {
            variable_value_w &= 0x1f;
            variable_value_w_lsb = variable_value_w & 0xff;
            if (variable_value_w_lsb == VAR_STACK)
            {
                ++num_stack_variables;
            }
        }
        entry_address += 0x10;
    }    
    printf("num stack variables: %d\n", num_stack_variables);
    offset = 0x60010;
    entry_address = base_address + offset + 0x8; 
    for (int i = 0; i < 0xfffd; ++i)
    {
        ReadProcessMemory(p_handle, (LPCVOID)entry_address, &variable_value_w, sizeof(int), NULL);
        if ((variable_value_w & 0x60) != 0)
        {
            if ((variable_value_w & 0x1f) == 0xa)
            {

            }
        }
    }
}

int GetFreeVariables(HANDLE p_handle)
{
    int offset = 0x60000;
    DWORD base_address = 0;
    ReadProcessMemory(p_handle, (LPCVOID)SCR_VAR_PUB_ADDY, &base_address, sizeof(int), NULL);
    if (base_address)
    {
        DWORD entry_address = base_address + offset + 0x18;
        unsigned int variable_value_w;
        unsigned char variable_value_w_lsb;
        int num_free_variables = 0;
        for (int i = 0; i < 0xfffd; ++i)
        {
            ReadProcessMemory(p_handle, (LPCVOID)entry_address, &variable_value_w, sizeof(int), NULL);
            variable_value_w_lsb = variable_value_w & 0xff;
            if ((variable_value_w_lsb & 0x60) == VAR_STAT_FREE)
            {
                ++num_free_variables;
            }
            entry_address += 0x10;
        }
        return num_free_variables;
    }
    return -1;
}

int GetTakenVariables(HANDLE p_handle)
{
    int offset = 0x60000;
    DWORD base_address = 0;
    ReadProcessMemory(p_handle, (LPCVOID)SCR_VAR_PUB_ADDY, &base_address, sizeof(int), NULL);
    if (base_address)
    {
        DWORD entry_address = base_address + offset + 0x18;
        unsigned int variable_value_w;
        unsigned char variable_value_w_lsb;
        int num_taken_variables = 0;
        for (int i = 0; i < 0xfffd; ++i)
        {
            ReadProcessMemory(p_handle, (LPCVOID)entry_address, &variable_value_w, sizeof(int), NULL);
            variable_value_w_lsb = variable_value_w & 0xff;
            if ((variable_value_w_lsb & 0x60) != VAR_STAT_FREE)
            {
                ++num_taken_variables;
            }
            entry_address += 0x10;
        }
        return num_taken_variables;
    }
    return -1;
}

int GetStackVariables(HANDLE p_handle)
{
    int offset = 0x60000;
    DWORD base_address = 0;
    ReadProcessMemory(p_handle, (LPCVOID)SCR_VAR_PUB_ADDY, &base_address, sizeof(int), NULL);
    if (base_address)
    {
        DWORD entry_address = base_address + offset + 0x18;
        unsigned int variable_value_w;
        unsigned char variable_value_w_lsb;
        int num_stack_variables = 0;
        for (int i = 0; i < 0xfffd; ++i)
        {
            ReadProcessMemory(p_handle, (LPCVOID)entry_address, &variable_value_w, sizeof(int), NULL);
            variable_value_w_lsb = variable_value_w & 0xff;
            if ((variable_value_w_lsb & 0x60) != VAR_STAT_FREE)
            {
                variable_value_w &= 0x1f;
                variable_value_w_lsb = variable_value_w & 0xff;
                if (variable_value_w_lsb == VAR_STACK)
                {
                    ++num_stack_variables;
                }
            }
            entry_address += 0x10;
        }
        return num_stack_variables;
    }
    return -1;
}

int GetAnimVariables(HANDLE p_handle)
{
    int offset = 0x60000;
    DWORD base_address = 0;
    ReadProcessMemory(p_handle, (LPCVOID)SCR_VAR_PUB_ADDY, &base_address, sizeof(int), NULL);
    if (base_address)
    {
        DWORD entry_address = base_address + offset + 0x18;
        unsigned int variable_value_w;
        unsigned char variable_value_w_lsb;
        int num_stack_variables = 0;
        for (int i = 0; i < 0xfffd; ++i)
        {
            ReadProcessMemory(p_handle, (LPCVOID)entry_address, &variable_value_w, sizeof(int), NULL);
            variable_value_w_lsb = variable_value_w & 0xff;
            if ((variable_value_w_lsb & 0x60) != VAR_STAT_FREE)
            {
                variable_value_w &= 0x1f;
                variable_value_w_lsb = variable_value_w & 0xff;
                if (variable_value_w_lsb == VAR_ANIMATION)
                {
                    ++num_stack_variables;
                }
            }
            entry_address += 0x10;
        }
        return num_stack_variables;
    }
    return -1;
}



int GetFunctions(HANDLE p_handle)
{
    int offset = 0x60000;
    DWORD base_address = 0;
    ReadProcessMemory(p_handle, (LPCVOID)SCR_VAR_PUB_ADDY, &base_address, sizeof(int), NULL);
    if (base_address)
    {
        DWORD entry_address = base_address + offset + 0x18;
        unsigned int variable_value_w;
        unsigned char variable_value_w_lsb;
        int num_func = 0;
        for (int i = 0; i < 0xfffd; ++i)
        {
            ReadProcessMemory(p_handle, (LPCVOID)entry_address, &variable_value_w, sizeof(int), NULL);
            variable_value_w_lsb = variable_value_w & 0xff;
            if ((variable_value_w_lsb & 0x60) != VAR_STAT_FREE)
            {
                variable_value_w &= 0x1f;
                variable_value_w_lsb = variable_value_w & 0xff;
                if (variable_value_w_lsb == VAR_FUNCTION)
                {
                    ++num_func;
                }
            }
            entry_address += 0x10;
        }
        return num_func;
    }
    return -1;
}

/*int GetObjects(HANDLE p_handle)
{
    int offset = 0x60000;
    DWORD base_address = 0;
    ReadProcessMemory(p_handle, (LPCVOID)SCR_VAR_PUB_ADDY, &base_address, sizeof(int), NULL);
    if (base_address)
    {
        DWORD entry_address = base_address + offset + 0x18;
        unsigned int variable_value_w;
        unsigned char variable_value_w_lsb;
        int num_objects = 0;
        for (int i = 0; i < 0xfffd; ++i)
        {
            ReadProcessMemory(p_handle, (LPCVOID)entry_address, &variable_value_w, sizeof(int), NULL);
            variable_value_w_lsb = variable_value_w & 0xff;
            if ((variable_value_w_lsb & 0x60) != VAR_STAT_FREE)
            {
                variable_value_w &= 0x1f;
                variable_value_w_lsb = variable_value_w & 0xff;
                if (variable_value_w_lsb == VAR_OBJECT)
                {
                    ++num_objects;
                }
            }
            entry_address += 0x10;
        }
        return num_objects;
    }
    return -1;
}

int GetEntities(HANDLE p_handle)
{
    int offset = 0x60000;
    DWORD base_address = 0;
    ReadProcessMemory(p_handle, (LPCVOID)SCR_VAR_PUB_ADDY, &base_address, sizeof(int), NULL);
    if (base_address)
    {
        DWORD entry_address = base_address + offset + 0x18;
        unsigned int variable_value_w;
        unsigned char variable_value_w_lsb;
        int num_ents = 0;
        for (int i = 0; i < 0xfffd; ++i)
        {
            ReadProcessMemory(p_handle, (LPCVOID)entry_address, &variable_value_w, sizeof(int), NULL);
            variable_value_w_lsb = variable_value_w & 0xff;
            if ((variable_value_w_lsb & 0x60) != VAR_STAT_FREE)
            {
                variable_value_w &= 0x1f;
                variable_value_w_lsb = variable_value_w & 0xff;
                if (variable_value_w_lsb == VAR_ENTITY)
                {
                    ++num_ents;
                }
            }
            entry_address += 0x10;
        }
        return num_ents;
    }
    return -1;
}

int GetThreads(HANDLE p_handle)
{
    int offset = 0x60000;
    DWORD base_address = 0;
    ReadProcessMemory(p_handle, (LPCVOID)SCR_VAR_PUB_ADDY, &base_address, sizeof(int), NULL);
    if (base_address)
    {
        DWORD entry_address = base_address + offset + 0x18;
        unsigned int variable_value_w;
        unsigned char variable_value_w_lsb;
        int num_threads = 0;
        for (int i = 0; i < 0xfffd; ++i)
        {
            ReadProcessMemory(p_handle, (LPCVOID)entry_address, &variable_value_w, sizeof(int), NULL);
            variable_value_w_lsb = variable_value_w & 0xff;
            if ((variable_value_w_lsb & 0x60) != VAR_STAT_FREE)
            {
                variable_value_w &= 0x1f;
                variable_value_w_lsb = variable_value_w & 0xff;
                if (variable_value_w_lsb == VAR_THREAD)
                {
                    ++num_threads;
                }
            }
            entry_address += 0x10;
        }
        return num_threads;
    }
    return -1;
}*/
