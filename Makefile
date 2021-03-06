CC = gcc

BUILD_DIR = build/
SRC_DIR = src/
H_DIR = headers/

CFLAGS = -O0 -Wall -g -I$(H_DIR)
LDFLAGS = 

PROG0_DIR = procTestA/
PROG1_DIR = procTestB/
PROG2_DIR = canal/
PROG3_DIR = procTestC/
PROG4_DIR = procTestD/

PROG0 = $(PROG0_DIR)myProcTestA
PROG1 = $(PROG1_DIR)myProcTestB
PROG2 = $(PROG2_DIR)myCanal
PROG3 = $(PROG3_DIR)myProcTestC
PROG4 = $(PROG4_DIR)myProcTestD


all: $(PROG0) $(PROG1) $(PROG2) $(PROG3) $(PROG4)
	@(cp $(PROG0) ./)
	@(cp $(PROG1) ./)
	@(cp $(PROG2) ./)
	@(cp $(PROG3) ./)
	@(cp $(PROG4) ./)

exe:
	make exeA &
	make exeB

A: $(PROG0)
	nice -n 200 ./myProcTestA
	
B: $(PROG1)
	nice -n 200 ./myProcTestB

Al: $(PROG0)
	nice -n 200 ./myProcTestA -l
	
Bl: $(PROG1)
	nice -n 200 ./myProcTestB -l

Ad: $(PROG0)
	nice -n 200 ./myProcTestA -d
	
Bd: $(PROG1)
	nice -n 200 ./myProcTestB -d

Af: $(PROG3)
	nice -n 200 ./myProcTestC

Bf: $(PROG4)
	nice -n 200 ./myProcTestD 

### Compilation of "canal fiable" dependencies
$(PROG0): $(PROG0_DIR)$(SRC_DIR)*.c 
	@(cd $(PROG0_DIR) && $(MAKE))
$(PROG1): $(PROG1_DIR)$(SRC_DIR)*.c
	@(cd $(PROG1_DIR) && $(MAKE))
$(PROG2): $(PROG2_DIR)$(SRC_DIR)*.c
	@(cd $(PROG2_DIR) && $(MAKE))

### Compilation of "détecteur de fautes" dependencies
$(PROG3): $(PROG3_DIR)$(SRC_DIR)*.c 
	@(cd $(PROG3_DIR) && $(MAKE))
$(PROG4): $(PROG4_DIR)$(SRC_DIR)*.c
	@(cd $(PROG4_DIR) && $(MAKE))

.PHONY: clean 

clean: 
	@(cd $(PROG0_DIR) && $(MAKE) $@)
	@rm myProcTestA
	@(cd $(PROG1_DIR) && $(MAKE) $@)
	@rm myProcTestB
	@(cd $(PROG2_DIR) && $(MAKE) $@)
	@rm myCanal
	@(cd $(PROG3_DIR) && $(MAKE) $@)
	@rm myProcTestC
	@(cd $(PROG4_DIR) && $(MAKE) $@)
	@rm myProcTestD
	@rm $(PROG1_DIR)data/receive.txt
	@touch $(PROG1_DIR)data/receive.txt


