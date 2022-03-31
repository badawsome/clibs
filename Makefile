export LIBS = commons avl

all:
	@for lib in ${LIBS} ; do \
		$(MAKE) -C $${lib} all; \
	done

test: all
	$(MAKE) -C tests test

clean:
	@for lib in ${LIBS} ; do \
    	$(MAKE) -C $${lib} clean; \
    done && $(MAKE) -C tests clean
