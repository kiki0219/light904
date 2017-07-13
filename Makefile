#KERN=/usr/src/kernels/4.1.13-100.fc21.i686
KERN=/root/linux-3.0.8
CUR=`pwd`
obj-m:=leddriver3.o


all:
	make -C $(KERN) M=$(CUR) modules

clean:
	make -C $(KERN) M=$(CUR) clean
