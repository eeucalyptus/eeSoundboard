MEMORY
{
	rom (rx) : ORIGIN = 0x08000000, LENGTH = 512K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
}

/* Include the common ld script. */
INCLUDE ../libopencm3/lib/cortex-m-generic.ld