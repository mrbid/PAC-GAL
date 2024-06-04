all:
	cc main.c -I inc -Ofast -lglfw -lm -o release/PacGal_linux
	strip --strip-unneeded release/PacGal_linux
	upx --lzma --best release/PacGal_linux

win:
	i686-w64-mingw32-gcc main.c -Ofast -I inc -L. -lglfw3dll -lm -o release/PacGal_windows.exe
	strip --strip-unneeded release/PacGal_windows.exe
	upx --lzma --best release/PacGal_windows.exe

test:
	tcc main.c -I inc -Ofast -lglfw -lm -o /tmp/PacGal_test
	/tmp/PacGal_test
	rm /tmp/PacGal_test

clean:
	rm -f release/PacGal_linux
	rm -f release/PacGal_windows.exe
