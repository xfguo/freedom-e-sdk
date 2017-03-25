CFLAGS += -nostartfiles -nostdlib -fno-builtin -g -mcmodel=medany

baremetal: main.c entry.s lock.s hartid.s link.lds
	$(CC) $(CFLAGS) -o $@ $(filter-out %.lds,$^) -T link.lds

clean:
	rm -f baremetal
