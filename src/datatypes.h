#ifndef DATATYPES_H
#define DATATYPES_H

union Variable_u {
    unsigned int prev;
    unsigned int prev_sibling;
};

typedef struct {
    unsigned int id;
    union Variable_u u;
} Variable;

union ObjectInfo_u {
    unsigned short ent_num;
    unsigned short size;
    unsigned int next_ent_id;
    unsigned int self;
};

typedef struct {
    unsigned short ref_count;
    union ObjectInfo_u u;
} ObjectInfo;

typedef struct {
    char* pos;
    unsigned short size;
    unsigned short buffer_len;
    unsigned int local_id;
    unsigned char time;
    char buffer[1];
} VariableStackBuffer;

union VariableUnion {
    int int_val;
    unsigned int uint_val;
    float float_val;
    unsigned int string_val;
    float* vector_val;
    unsigned char* code_pos_val;
    unsigned int pointer_val;
    VariableStackBuffer* stack_val;
    unsigned int entity_offset;
};

union VariableValueInternal_u {
    unsigned int next;
    union VariableUnion u;
    ObjectInfo o;
};

union VariableValueInternal_w {
    unsigned int status;
    unsigned int type;
    unsigned int name;
    unsigned int class_num;
    unsigned int notify_name;
    unsigned int wait_time;
    unsigned int parent_local_id;
};

union VariableValueInternal_v {
    unsigned int next;
    unsigned int index;
};

typedef struct {
    Variable hash;
    union VariableValueInternal_u u;
    union VariableValueInternal_w w;
    union VariableValueInternal_v v;
    unsigned int next_sibling;
} VariableValueInternal;

typedef struct {
    VariableValueInternal* variable_list;
    char filler[0x7c];
} ScrVarGlob;

#endif