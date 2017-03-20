# Activity 3 - Backdoor

I implemented a simple interpreter and its backdoor.
I referred to some repositories of classmates (`nohkwak` and `ggoboogy`).
`nohkwak` helped me to understand the method to make a program written in Mini-language.

* ./interpreter/interpreter.c
  I used 1k code and 8k data(heap) memory.
  ```
	uint32_t code[ BUFF_SIZE ];
	uint8_t  data[ HEAP_SIZE + 1];
  ```

  I defined local program counter as global variable for `jump` and `ite` instructions.	
  I implemented all 14 instructions.

* ./login/login.mini
  I implemented a simple login program using Mini-language.
  Regardless of the inputed id, this program checks only password whether is `passwd` or not.
  If password is matched, this program says `Success` and terminates.
  Otherwise, this program terminates without any comment.

* ./test/test.mini
  I implemented a simple program that repeats the name inputed by user for the number of count inputed by user.

* ./test/test.md
  This file explains what test program is supposed to do.

* ./backdoor
  This folder is originated from `interpreter` folder. 
  I added a backdoor to the interpreter.
  IF user inputs `superuser`, show `Success` and terminate its function.
  
---
I could remind a computer architecture that fetches and executes instructions.
Based on mini-VM, I learned the way to implement instructions.
I also learned a basic concept of backdoor and the way to implement a simple backdoor.
On the view of the developer, a backdoor (whether it is intended or not) can be possible and thus I need to study more about the countermeasures against the backdoor.
