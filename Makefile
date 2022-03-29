all:
	$(MAKE) -C avl all && $(MAKE) -C data_gen all

test: all
	$(MAKE) -C tests test

clean:
	$(MAKE) -C avl clean && $(MAKE) -C tests clean
