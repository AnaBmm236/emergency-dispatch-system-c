build:
	gcc -g dispatch.c -o dispatch
run: build
	./dispatch
clean:
	rm -f dispatch
