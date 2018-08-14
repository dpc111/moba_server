all:
	@make -f libcommon.mk
	@make install -f libcommon.mk
	@make -f libnet.mk
	@make install -f libnet.mk
	@make -f libserver.mk
	@make install -f libserver.mk
	@make -f libmsg.mk
	@make install -f libmsg.mk
	@make -f battle.mk

install:

clean:
	-${RM} *.so
	-${RM} *.o
	-${RM} *.d
	-${RM} battle
	-${RM} core
