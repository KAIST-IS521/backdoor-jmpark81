# Test program

### Description
This program shows the inputed name for the inputed counts.
First, enter the number of counts and the name to be displayed.
Then, the name is displayed for the entered number.

> Input : user's two inputs (number of count, name) 
> Output : name is shown repeatedly at given count.

### Pseudocode 
```c
#include<stdio.h>

int main(void){
  int count = 0;
  char name[16];
  int i;

  printf("Input: \n");
  scanf("%d", &count);
  scanf("%s", name);

  printf("Output: \n");
  for(i=0; i<count; i++){
        printf("%s\n", name);
  }

  return 0;
}
```

### Example result
```
Input:
5
Jaemin
Output

Jaemin
Jaemin
Jaemin
Jaemin
Jaemin
```
