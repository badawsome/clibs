all:
	$(MAKE) -C avl all

test: all
	$(MAKE) -C tests test

clean:
	$(MAKE) -C avl clean && $(MAKE) -C tests clean
