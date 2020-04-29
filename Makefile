all: calibration check_virtualization check_file_corruption sender receiver
calibration: calibration.c ../cacheutils.h
	gcc -std=gnu11 -O2 -o $@ $@.c
virtualization: check_virtualization.c ../cacheutils.h
	gcc -std=gnu11 -O2 -o $@ $@.c
corruption: check_file_corruption.c ../cacheutils.h
	gcc -std=gnu11 -O2 -o $@ $@.c
sender: sender.c ../cacheutils.h
	gcc -std=gnu11 -O2 -o $@ $@.c
receiver: receiver.c ../cacheutils.h
	gcc -std=gnu11 -O2 -o $@ $@.c
