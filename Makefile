all: client server
client: client.o clipboard.o x_manager.o networking.o
	gcc -o run_client client.o clipboard.o x_manager.o networking.o -lX11 -lXtst 
client.o: client.c client/client.h networking/networking.h 
	gcc -c client.c networking/networking.h client/client.h -lX11 -lXtst
clipboard.o: client/clipboard.c client/client.h
	gcc -c client/clipboard.c client/client.h -lX11 -lXtst
x_manager.o: client/x_manager.c client/client.h
	gcc -c client/x_manager.c client/client.h -lX11 -lXtst
networking.o: networking/networking.c networking/networking.h
	gcc -c networking/networking.c networking/networking.h
server: server.o networking.o
	gcc -o run_server server.o networking.o
server.o: server.c networking/networking.h
	gcc -c server.c networking/networking.h
clean:
	rm -f *~
	rm -f *.o
	rm -f run_server run_client x_in_test x_out_test clipboard_test
test: all
	gcc -o x_in_test test/x_manager_test.c x_manager.o -lX11 -lXtst
	gcc -o x_out_test test/x_fake_mousepress_test.c x_manager.o -lX11 -lXtst
	gcc -o clipboard_test test/clipboard_test.c clipboard.o x_manager.o -lX11 -lXtst

