all:
	gcc ../main.c ../glad_gl.c -I ../inc -Ofast -lglfw -lm -o pacgal

install:
	cp pacgal $(DESTDIR)

uninstall:
	rm $(DESTDIR)/pacgal

clean:
	rm pacgal