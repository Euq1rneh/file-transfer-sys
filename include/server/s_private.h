#ifndef S_PRIVATE_
#define S_PRIVATE_

typedef struct Operation
{
    int op_code;
    int has_params;
    char *param;
} Operation;

#endif