// This is where you put your VM code.
// I am trying to follow the coding style of the original.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "minivm.h"

#define NUM_REGS   (256)
#define NUM_FUNCS  (256)

// Global variable that indicates if the process is running.
static bool is_running = true;

// Instructions supported by compiler
void halt(struct VMContext* ctx, const uint32_t instr){
   is_running = false;
}

void load(struct VMContext* ctx, const uint32_t instr){
}

void store(struct VMContext* ctx, const uint32_t instr){
}

void move(struct VMContext* ctx, const uint32_t instr){
}

void puti(struct VMContext* ctx, const uint32_t instr){
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
}

void ge(struct VMContext* ctx, const uint32_t instr){
}

void eq(struct VMContext* ctx, const uint32_t instr){
}

void ite(struct VMContext* ctx, const uint32_t instr){
}

void jump(struct VMContext* ctx, const uint32_t instr){
}

void puts(struct VMContext* ctx, const uint32_t instr){
}

void gets(struct VMContext* ctx, const uint32_t instr){
}


void usageExit() {
    // TODO: show usage
    printf("USAGE: interpreter [FILE]\n");
    exit(1);
}

void initFuncs(FunPtr *f, uint32_t cnt) {
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        f[i] = NULL;
    }

    // TODO: initialize function pointers
    // f[0x00] = halt;
    // f[0x10] = load;
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
    uint32_t* pc;

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

    while (is_running) {
        // TODO: Read 4-byte bytecode, and set the pc accordingly
        stepVMContext(&vm, &pc);
    }

    fclose(bytecode);

    // Zero indicates normal termination.
    return 0;
}