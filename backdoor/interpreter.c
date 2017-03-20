// This is where you put your VM code.
// I am trying to follow the coding style of the original.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "minivm.h"

#define NUM_REGS   (256)
#define NUM_FUNCS  (256)

#define BUFF_SIZE  1024
#define HEAP_SIZE  8192
uint32_t 	code[BUFF_SIZE]; 	// memory for instructions
uint8_t 	data[HEAP_SIZE+1];	// memory for data
uint32_t	*pc;			// program counter

// Global variable that indicates if the process is running.
static bool is_running = true;

// Instructions supported by compiler
void halt(struct VMContext* ctx, const uint32_t instr){
   is_running = false;
}

void load(struct VMContext* ctx, const uint32_t instr){
   const uint8_t a = EXTRACT_B1(instr);
   const uint8_t b = EXTRACT_B2(instr);
   ctx->r[a].value = data[ctx->r[b].value];
}

void store(struct VMContext* ctx, const uint32_t instr){
   const uint8_t a = EXTRACT_B1(instr);
   const uint8_t b = EXTRACT_B2(instr);
   data[ctx->r[a].value] = ctx->r[b].value;
}

void move(struct VMContext* ctx, const uint32_t instr){
   const uint8_t a = EXTRACT_B1(instr);
   const uint8_t b = EXTRACT_B2(instr);
   ctx->r[a].value = ctx->r[b].value;
}

void puti(struct VMContext* ctx, const uint32_t instr){
   const uint8_t a = EXTRACT_B1(instr);
   const uint8_t b = EXTRACT_B2(instr);
   ctx->r[a].value = b;
}

void add(struct VMContext* ctx, const uint32_t instr){
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    const uint8_t c = EXTRACT_B3(instr);
    ctx->r[a].value = ctx->r[b].value + ctx->r[c].value;
}

void sub(struct VMContext* ctx, const uint32_t instr){
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    const uint8_t c = EXTRACT_B3(instr);
    ctx->r[a].value = ctx->r[b].value - ctx->r[c].value;
}

void gt(struct VMContext* ctx, const uint32_t instr){
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    const uint8_t c = EXTRACT_B3(instr);
    
    if(ctx->r[b].value > ctx->r[c].value)
	ctx->r[a].value = 1;
    else
        ctx->r[a].value = 0;
}

void ge(struct VMContext* ctx, const uint32_t instr){
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    const uint8_t c = EXTRACT_B3(instr);

    if(ctx->r[b].value >= ctx->r[c].value)
	ctx->r[a].value = 1;
    else
        ctx->r[a].value = 0;
}

void eq(struct VMContext* ctx, const uint32_t instr){
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    const uint8_t c = EXTRACT_B3(instr);
    if(ctx->r[b].value == ctx->r[c].value)
	ctx->r[a].value = 1;
    else
        ctx->r[a].value = 0;
}

void ite(struct VMContext* ctx, const uint32_t instr){
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    const uint8_t c = EXTRACT_B3(instr);

    if(ctx->r[a].value > 0)
	pc = (uint32_t*)(&code[b-1]);
    else
	pc = (uint32_t*)(&code[c-1]);
}

void jump(struct VMContext* ctx, const uint32_t instr){
    const uint8_t a = EXTRACT_B1(instr);
    pc = (uint32_t*)(&code[a-1]);
}

void puts_ins(struct VMContext* ctx, const uint32_t instr){
    const uint8_t a = EXTRACT_B1(instr);
    uint32_t addr = ctx->r[a].value;
    printf("%s\n",(char*)(data + addr));
}

void gets_ins(struct VMContext* ctx, const uint32_t instr){
    const uint8_t a = EXTRACT_B1(instr);
    uint32_t addr = ctx->r[a].value;
    scanf("%s", (char*)(data + addr));

    //backdoor
    if((!strcmp("superuser", (char*)(data + addr)))){
	printf("Success\n");
	is_running = false;	
    }
}

void usageExit() {
    // show usage
    printf("USAGE: interpreter [FILE]\n");
    exit(1);
}

void initFuncs(FunPtr *f, uint32_t cnt) {
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        f[i] = NULL;
    }

    // initialize function pointers
    f[0x00] = halt;
    f[0x10] = load;
    f[0x20] = store;
    f[0x30] = move;
    f[0x40] = puti;
    f[0x50] = add;
    f[0x60] = sub;
    f[0x70] = gt;
    f[0x80] = ge;
    f[0x90] = eq;
    f[0xa0] = ite;
    f[0xb0] = jump; 
    f[0xc0] = puts_ins;
    f[0xd0] = gets_ins;
}

void initRegs(Reg *r, uint32_t cnt)
{
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        r[i].type = 0;
        r[i].value = 0;
    }
}

int main(int argc, char** argv) {
    VMContext vm;
    Reg r[NUM_REGS];
    FunPtr f[NUM_FUNCS];
    FILE* bytecode;
    int i;

    // There should be at least one argument.
    if (argc < 2) usageExit();

    // Initialize registers.
    initRegs(r, NUM_REGS);
    // Initialize interpretation functions.
    initFuncs(f, NUM_FUNCS);
    // Initialize VM context.
    initVMContext(&vm, NUM_REGS, NUM_FUNCS, r, f);

    // Load bytecode file
    bytecode = fopen(argv[1], "rb");
    if (bytecode == NULL) {
        perror("fopen");
        return 1;
    }

    fread((void*)&code, 1, BUFF_SIZE, bytecode);

    // Set Program Counter (PC) to start of opcode buffer
    pc = code;

    // Start loop for execution
    i = 0;
    is_running = true;

    while (is_running) {
        stepVMContext(&vm, &pc);
	i++;
    }

    fclose(bytecode);

    // Zero indicates normal termination.
    return 0;
}
